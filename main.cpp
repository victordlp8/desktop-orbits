#include <windows.h>
#include <commctrl.h>
#include "Vec2.h"
#include <vector>
#include <iostream>
#include <time.h>

using namespace std;

const long double maxVel = 1;
const long double G = 6.6743e-11; // Gravitational constant
const long double hitBox = 35;    // Hit box size
Vec2 oCoords(-1, -1);
const static Vec2 offset(-24, -28);
const Vec2 range(0, 2e+7);
// const Vec2 offset(0, 0);

int random(Vec2 minMax) // range : [min, max]
{
    static bool first = true;
    if (first)
    {
        srand(time(NULL)); // seeding for the first time only!
        first = false;
    }
    return minMax.x + rand() % (((int)minMax.y + 1) - (int)minMax.x);
}

class movingEntity
{
public:
    Vec2 pos;
    Vec2 vel;

    long double mass; // mass of entity in KB

    movingEntity()
    {
        pos = oCoords + offset;
        vel = Vec2(random(Vec2(-maxVel, maxVel)), random(Vec2(-maxVel, maxVel)));
        mass = random(range);
    }

    movingEntity(Vec2 p)
    {
        pos = p;
        vel = Vec2(random(Vec2(-maxVel, maxVel)), random(Vec2(-maxVel, maxVel)));
        mass = random(range);
    }

    movingEntity(Vec2 p, Vec2 v, long double m)
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

    void setPos(const Vec2 &p)
    {
        pos = p;
    }

    void setVel(const Vec2 &v)
    {
        vel = v;
    }

    void push(Vec2 aceleration, long double dt = 1)
    {
        if (aceleration.x != 0)
        {
            vel.x += aceleration.x * dt;
        }
        if (aceleration.y != 0)
        {
            vel.y += aceleration.y * dt;
        }
    }

    // void update(HWND hd, long double dt = 1)
    // {
    //     pos += vel * dt;
    //     ListView_SetItemPosition(hd, 1, (int)pos.x + vel.x + 1920, (int)pos.y + vel.y);
    // }
};

Vec2 gravity(movingEntity entity1, movingEntity entity2)
{
    long double fG = G * (entity1.mass * entity2.mass) / pow(entity1.pos.length(entity2.pos), 2);

    return (entity2.pos - entity1.pos) * fG;
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
        if (oCoords.x == -1)
        {
            oCoords = Vec2(abs(info.rcMonitor.left), abs(info.rcMonitor.top));
        }
    }
}

void getOriginCoords()
{

    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);
}

// class desktopIcon
// {
// public:
//     string name;
//     movingEntity entity;
//     Vec2 offset;

//     desktopIcon(string n)
//     {
//         name = n;
//         entity = movingEntity();
//         entity.mass = 1; // Hay que conseguir la masa del icono
//         offset = Vec2(0, 0);
//     }
// };

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
    // Vec2 desktopSize;

    // desktop(string desktopPath)
    // {
    //     for (auto & entry : fs::directory_iterator(desktopPath))
    //     {
    //         if (entry.is_regular_file())
    //         {
    //             desktopIcon icon(entry.path().filename().string());
    //             icons.push_back(icon);
    //         }
    //     }
    // }

    desktop()
    {
        getOriginCoords();

        // desktopSize = Vec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
        // desktopSize = getDesktopSize();

        // hd = FindWindow("Program Manager", NULL);
        hd = FindWindow("Progman", NULL);
        hd = FindWindowEx(hd, 0, "SHELLDLL_DefView", NULL);
        hd = FindWindowEx(hd, 0, "SysListView32", NULL);

        // hd = GetDesktopWindow();

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
            icons.push_back(movingEntity(Vec2(offset.x + pt.x, offset.y + pt.y)));

            // cout << pt.x << " " << pt.y << endl;
            // cout << icons[i].pos.x << " " << icons[i].pos.y << endl;
            // cout << icons[i].vel.x << " " << icons[i].vel.y << endl;
        }
    }

    // Vec2 getDesktopSize()
    // {
    //     HWND hDesktop;
    //     RECT rDesktop;
    //     hDesktop = GetDesktopWindow();
    //     GetWindowRect(hDesktop, &rDesktop);
    //     return Vec2(rDesktop.right, rDesktop.bottom);
    // }

    void moveIcon(int i)
    {
        icons[i].pos += icons[i].vel;
        ListView_SetItemPosition(hd, i, (int)icons[i].pos.x - offset.x, (int)icons[i].pos.y - offset.y);
    }

    void moveIcon(int i, Vec2 pos)
    {
        icons[i].pos = pos;
        ListView_SetItemPosition(hd, i, (int)icons[i].pos.x - offset.x, (int)icons[i].pos.y - offset.y);
    }

    void massIcon(int i, long double m)
    {
        icons[i].mass = m;
    }

    void velIcon(int i, Vec2 v)
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

    void physics(long double dt = 1)
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
                    if (icons[i].pos.length(icons[j].pos) >= hitBox)
                    {
                        Vec2 aceleration = gravity(icons[i], icons[j]);
                        icons[i].push(aceleration, dt);
                    }
                    else
                    {
                        if (j != 0)
                        {
                            Vec2 movementQuantity = icons[i].vel * icons[i].mass + icons[j].vel * icons[j].mass;
                            icons[i].vel -= movementQuantity / icons[i].mass;
                            icons[j].vel -= movementQuantity / icons[j].mass;
                        }
                        else
                        {
                            icons[i].vel = icons[i].vel * -1;
                        }
                    }
                }
            }
        }
    }

    void _VirtualFreeEx()
    {
        VirtualFreeEx(he, pC, 0, MEM_RELEASE);
    }

    void setIconPos(int index, Vec2 p)
    {
        moveIcon(index, p);
    }

    Vec2 getIconPos(int index)
    {
        return icons[index].pos;
    }

    void setIconVel(int index, Vec2 v)
    {
        icons[index].vel = v;
    }

    Vec2 getIconVel(int index)
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
    /*POINT pt;
    ListView_GetItemPosition(hd, 0, pC);
    ReadProcessMemory(he, pC, &pt, sizeof(POINT), NULL);
    cout<<"X : "<<pt.x<<" Y : "<<pt.y<<endl;*/

    desktop d;
    // d.addObject(movingEntity());
    cout << "Icon count: " << d.iconCount << endl;
    cout << "Coordenadas origin: " << oCoords.x << ", " << oCoords.y << endl;

    for (int i = 0; i < d.iconCount; i++)
    {
        d.moveIcon(i, Vec2((int)(oCoords.x + offset.x + random(Vec2(0, 1920))), (int)(oCoords.y + offset.y + random(Vec2(0, 1080)))));
        // d.setIconPos(i, Vec2(1920/2 + oCoords.x + offset.x + i * 50, 1080/2 + oCoords.y + offset.y + i * 30));
        // d.setIconPos(i, offset);
    }
    d.update();

    // char c;
    // cout << "Waiting for a letter to start..." << endl;
    // cin >> c;

    // d.icons[d.iconCount].mass = range.y * 100;
    d.moveIcon(0, Vec2((int)(oCoords.x + offset.x + 1920 / 2), (int)(oCoords.y + offset.y + 1080 / 2)));
    d.massIcon(0, range.y / 10);

    // POINT px;

    while (true)
    {
        // GetCursorPos(&px);
        // d.icons[d.iconCount].setPos(Vec2(px.x, px.y));
        d.velIcon(0, Vec2(0, 0));
        d.physics();

        // cout << "Cursor position: " << d.icons[d.iconCount].pos.x << ", " << d.icons[d.iconCount].pos.y << " and mass " << d.icons[d.iconCount].mass << endl;

        // cout << "-----------------------" << endl;
        // cout << "X: " << cursor.pos.x << " Y: " << cursor.pos.y << endl;
        // cout << "Position: " << icon.pos.x << " " << icon.pos.y << endl;
        // cout << "Aceleration: " << aceleration.x << " " << aceleration.y << endl;
        // cout << "Velocity: " << icon.vel.x << " " << icon.vel.y << endl;

        d.update();

        Sleep(10);
    }

    d._VirtualFreeEx();
}
