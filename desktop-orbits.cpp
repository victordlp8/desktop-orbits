#include <windows.h>
#include <commctrl.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <cmath>
#include <random>
#include "include/polarCoords2.h"
#include "include/Vec2.h"
#include "include/json.hpp"

using json = nlohmann::json;
using namespace std;

inline polarCoords2 toPolarCoords(const Vec2 &other)
{
    return polarCoords2(other.module(), other.angle());
}

inline Vec2 toVec2(const polarCoords2 &other)
{
    return Vec2(other.r * cos(other.theta), other.r * sin(other.theta));
}

bool loadJson(json &json_data, const char *fileName)
{
    ifstream json_file(fileName);
    string json_string, line;
    if (json_file.is_open())
    {
        while (getline(json_file, line))
        {
            json_string += line + '\n';
        }
        json_file.close();

        json_data = json::parse(json_string);

        return true;
    }
    else
    {
        return false;
    }
}

long double PI;
long double orbVel_modifier;
long double G;      // Gravitational constant
long double hitBox; // Hit box size
polarCoords2 oCoords;
polarCoords2 offset;
Vec2 massRange;
long double massSun;
int physics_per_second;

const char *cfg_path = "config.json";
bool loadConfig(const char *cfg_path)
{
    json cfg;
    if (loadJson(cfg, cfg_path))
    {
        cout << "Config loaded successfully." << endl;

        PI = atan(1) * 4;
        orbVel_modifier = cfg["orbVel_modifier"];
        G = cfg["G_const"];
        hitBox = cfg["hitBox"];
        oCoords = toPolarCoords(Vec2(cfg["oCoords"][0], cfg["oCoords"][1]));
        offset = toPolarCoords(Vec2(cfg["offset"][0], cfg["offset"][1]));


        massRange = Vec2(cfg["massRange"][0], cfg["massRange"][1]);
        massSun = cfg["massSun"];

        physics_per_second = cfg["physics_per_second"];

        return true;
    }
    else
    {
        cout << "Config loading failed." << endl;
        return false;
    }
}

random_device rd;  // Will be used to obtain a seed for the random number engine
mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
long double random(Vec2 minMax) // range : [min, max]
{
    uniform_real_distribution<long double> dis(minMax.x, minMax.y);
    gen.seed(time(NULL) + rand());
    return dis(gen);
}

class movingEntity
{
public:
    polarCoords2 pos;
    polarCoords2 vel;

    long double mass; // mass of entity in KB

    movingEntity()
    {
        pos = offset;
        vel = polarCoords2(0, 0);
        mass = random(massRange);
    }

    movingEntity(polarCoords2 p)
    {
        pos = p;
        vel = polarCoords2(0, 0);
        mass = random(massRange);
    }

    movingEntity(polarCoords2 p, polarCoords2 v, long double m)
    {
        pos = p;
        vel = v;
        mass = m;
    }

    movingEntity(const movingEntity &e)
    {
        pos = e.pos;
        vel = e.vel;
        mass = e.mass;
    }

    void setPos(const polarCoords2 &p)
    {
        pos = p;
    }

    void setVel(const polarCoords2 &v)
    {
        vel = v;
    }

    void pushAcceleration(polarCoords2 acceleration)
    {
        vel += acceleration;
    }

    void pushForce(polarCoords2 force)
    {
        vel += force / mass;
    }

    // long double bounceAngle(const movingEntity &other) const
    // {
    //     long double aEntry = vel.angle();
    //     long double aBetween = pos.angle(other.pos);
    //     long double aExit = PI - aEntry + (PI / 2 - aBetween);

    //     return aExit;
    // }

    // void update(HWND hd, long double dt = 1)
    // {
    //     pos += vel * dt;
    //     ListView_SetItemPosition(hd, 1, (int)pos.x + vel.x + 1920, (int)pos.y + vel.y);
    // }
};

polarCoords2 gravity(movingEntity entity1, movingEntity entity2)
{
    long double fG_r = G * (entity1.mass * entity2.mass) / pow((entity1.pos - entity2.pos).r, 2);
    long double fG_theta = (entity1.pos - entity2.pos).theta;

    return polarCoords2(fG_r, fG_theta);
}

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,
                              HDC hdcMonitor,
                              LPRECT lprcMonitor,
                              LPARAM dwData)
{
    MONITORINFO info;
    info.cbSize = sizeof(info);
    if (GetMonitorInfo(hMonitor, &info))
    {
        if (oCoords.r <= 1 && oCoords.r != 0)
        {
            oCoords = toPolarCoords(Vec2(abs(info.rcMonitor.left), abs(info.rcMonitor.top)));
            oCoords += offset;
            oCoords += toPolarCoords(Vec2(1920 / 2, 1080 / 2));
        }
    }
}

void getOriginCoords()
{
    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);
}

class desktop
{
private:
    HWND hd;

    HANDLE he;
    DWORD Pi;

    POINT *pC;

public:
    int iconCount;
    vector<movingEntity> icons;

    desktop()
    {
        getOriginCoords();

        hd = FindWindow("Progman", NULL);
        hd = FindWindowEx(hd, 0, "SHELLDLL_DefView", NULL);
        hd = FindWindowEx(hd, 0, "SysListView32", NULL);

        GetWindowThreadProcessId(hd, &Pi);
        he = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, false, Pi);

        POINT *pC = (POINT *)VirtualAllocEx(he, NULL, sizeof(POINT), MEM_COMMIT, PAGE_READWRITE);
        WriteProcessMemory(he, pC, &pC, sizeof(POINT), NULL);

        iconCount = SendMessage(hd, LVM_GETITEMCOUNT, 0, 0);

        POINT pt;
        for (int i = 0; i < iconCount; i++)
        {
            ListView_GetItemPosition(hd, i, pC);
            ReadProcessMemory(he, pC, &pt, sizeof(POINT), NULL);
            icons.push_back(movingEntity(offset + toPolarCoords(Vec2(pt.x, pt.y))));
        }
    }

    void _ListView_SetItemPosition(int i, const polarCoords2 &pos)
    {
        Vec2 ListView = toVec2(pos);
        ListView_SetItemPosition(hd, i, (int)ListView.x, (int)ListView.y);
    }

    void moveIcon(int i)
    {
        icons[i].pos += icons[i].vel;
        _ListView_SetItemPosition(i, icons[i].pos);
    }

    void moveIcon(int i, polarCoords2 p)
    {
        icons[i].pos = p;
        _ListView_SetItemPosition(i, icons[i].pos);
    }

    void massIcon(int i, long double m)
    {
        icons[i].mass = m;
    }

    void velIcon(int i, polarCoords2 v)
    {
        icons[i].vel = v;
    }

    void update()
    {
        for (int i = 0; i < iconCount; i++)
        {
            moveIcon(i);
        }
    }

    void physics()
    {
        for (int i = 0; i < iconCount; i++)
        {
            for (int j = 0; j < iconCount; j++)
            {
                if (i == 0)
                {
                    continue;
                }
                else if (i != j)
                {
                    movingEntity entity1 = icons[i];
                    movingEntity entity2 = icons[j];
                    if ((entity1.pos - entity2.pos).r >= 2 * hitBox)
                    {
                        polarCoords2 force = gravity(entity1, entity2);
                        entity1.pushForce(force);
                    }
                    else
                    {
                        // long double aExit_i = icons[i].bounceAngle(icons[j]);
                        // long double vEntry_i = icons[i].vel.module();
                        // Vec2 vExit_i(vEntry_i * cos(aExit_i), vEntry_i * sin(aExit_i));
                        // icons[i].vel = vExit_i;

                        // if (true)
                        // {
                        //     continue;

                        //     // long double aExit_j = icons[j].bounceAngle(icons[i]);
                        //     // long double vEntry_j = icons[j].vel.module();
                        //     // Vec2 vExit_j(vEntry_j * cos(aExit_j), vEntry_j * sin(aExit_j));
                        //     // icons[j].vel = vExit_j;

                        //     // Vec2 movementQuantity = icons[i].vel * icons[i].mass + icons[j].vel * icons[j].mass;
                        //     // icons[i].vel -= movementQuantity / icons[i].mass;
                        //     // icons[j].vel -= movementQuantity / icons[j].mass;
                        // }
                        // else
                        // {
                        //     icons[i].vel = icons[i].vel * -1;
                        // }
                    }
                    icons[i] = entity1;
                    icons[j] = entity2;
                }
            }
        }
    }

    void _VirtualFreeEx()
    {
        VirtualFreeEx(he, pC, 0, MEM_RELEASE);
    }

    void setIconPos(int index, polarCoords2 p)
    {
        moveIcon(index, p);
    }

    polarCoords2 getIconPos(int index)
    {
        return icons[index].pos;
    }

    void setIconVel(int index, polarCoords2 v)
    {
        icons[index].vel = v;
    }

    polarCoords2 getIconVel(int index)
    {
        return icons[index].vel;
    }

    void addObject(const movingEntity &e)
    {
        icons.push_back(e);
        iconCount++;
    }
};

int main()
{
    loadConfig(cfg_path);

    desktop d;

    d.moveIcon(0, oCoords);
    d.massIcon(0, massSun);

    cout << "--------------------------------------------" << endl;
    cout << "Icon count: " << d.iconCount << endl;
    cout << "oCoords: " << oCoords << endl;
    cout << "offset: " << offset << endl;
    cout << "--------------------------------------------" << endl;

    for (int i = 1; i < d.iconCount; i++)
    {
        d.moveIcon(i, oCoords + toPolarCoords(Vec2(random(Vec2(-(1920 + 1080)/4, (1920 + 1080)/4)), random(Vec2(-1080/2, 1080/2)))));

        // Smart orbit velocity calculation
        long double velocity = sqrt(-(G * d.icons[0].mass / (d.icons[i].pos - d.icons[0].pos).r)) * (1 + random({-orbVel_modifier, orbVel_modifier}));
        long double angle = (d.icons[i].pos - d.icons[0].pos).theta + PI/2;
        if (random({-1, 1}) < 0) angle += PI;

        d.velIcon(i, polarCoords2(velocity, angle));
    }
    d.update();

    while (true)
    {
        d.velIcon(0, polarCoords2(0, 0));
        d.physics();

        d.update();

        Sleep(1000 / physics_per_second);
    }

    d._VirtualFreeEx();
}
