#include <windows.h>
#include <commctrl.h>
#include <vector>
#include <iostream>
#include <cmath>
#include "include/aux_functions.h"
#include "include/json.hpp"
#include "include/movingEntity_2D.h"
#include "include/polarForm_2D.h"
#include "include/vector_2D.h"

using namespace std;

long double PI;              // PI constant
long double orbVel_modifier; // This is a percentual modifier for the orbital velocity
long double G;               // Gravitational constant
long double hitBox;          // Hitbox size
polarForm_2D oCoords;        // Origin coordinates (center of the main screen)
polarForm_2D offset;         // Offset that a icon has between its (0, 0) and its center
vector_2D massRange;         // The range of mass that the icons should have
long double massSun;         // Mass of the center icon icon
int physics_per_second;      // How many times the physics should be calculated in a second
vector_2D system_size;       // Size of the system that the icons should spawn in (in pixels) and centered on the origin point

/**
 * @brief Loads the data from the config into the variables
 *
 * @param cfg_path
 * @return true/false depending on whether the data was loaded successfully or not
 */
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
        oCoords = toPolarCoords(vector_2D(cfg["oCoords"][0], cfg["oCoords"][1]));
        offset = toPolarCoords(vector_2D(cfg["offset"][0], cfg["offset"][1]));

        massRange = vector_2D(cfg["massRange"][0], cfg["massRange"][1]);
        massSun = cfg["massSun"];

        physics_per_second = cfg["physics_per_second"];

        system_size = {cfg["system_size"][0], cfg["system_size"][1]};

        return true;
    }
    else
    {
        cout << "Config loading failed." << endl;
        return false;
    }
}

/**
 * @brief Calculates the gravity force between to movingEntity_2D objects
 *
 * @param entity1
 * @param entity2
 * @return polarForm_2D returns the force between the two entities
 */
polarForm_2D gravity(movingEntity_2D entity1, movingEntity_2D entity2)
{
    long double fG_r = G * (entity1.mass * entity2.mass) / pow((entity1.pos - entity2.pos).r, 2);
    long double fG_theta = (entity1.pos - entity2.pos).theta;

    return polarForm_2D(fG_r, fG_theta);
}

/**
 * @brief Desktop object that contains the icons and functions to manipulate them
 */
class desktop
{
private:
    HWND hd;

    HANDLE he;
    DWORD Pi;

    POINT *pC;

    void _ListView_SetItemPosition(int i, const polarForm_2D &pos)
    {
        vector_2D ListView = tovector_2D(pos);
        ListView_SetItemPosition(hd, i, (int)ListView.x, (int)ListView.y);
    }

public:
    int iconCount;
    vector<movingEntity_2D> icons;

    /**
     * @brief Construct a new desktop object initalizing the icons
     */
    desktop()
    {

        hd = FindWindow("Progman", NULL);
        hd = FindWindowEx(hd, 0, "SHELLDLL_DefView", NULL);
        hd = FindWindowEx(hd, 0, "SysListView32", NULL);

        GetWindowThreadProcessId(hd, &Pi);
        he = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, false, Pi);

        POINT *pC = (POINT *)VirtualAllocEx(he, NULL, sizeof(POINT), MEM_COMMIT, PAGE_READWRITE);
        WriteProcessMemory(he, pC, &pC, sizeof(POINT), NULL);

        iconCount = SendMessage(hd, LVM_GETITEMCOUNT, 0, 0);
        for (int i = 0; i < iconCount; i++)
            icons.push_back(movingEntity_2D());
    }

    /**
     * @brief Move the icon to the new position that the velocity dictates
     *
     * @param i Index of the icon
     */
    void moveIcon(int i)
    {
        icons[i].pos += icons[i].vel;
        _ListView_SetItemPosition(i, icons[i].pos);
    }

    /**
     * @brief Sets the position of the icon to the new position
     *
     * @param i Index of the icon
     * @param p New position of the icon
     */
    void setIconPos(int i, polarForm_2D p)
    {
        icons[i].pos = p;
        _ListView_SetItemPosition(i, icons[i].pos);
    }

    /**
     * @brief Sets the velocity of the icon to the new velocity
     *
     * @param i Index of the icon
     * @param v New velocity of the icon
     */
    void setIconVel(int i, polarForm_2D v)
    {
        icons[i].vel = v;
    }

    /**
     * @brief Sets the mass of the icon to the new mass
     *
     * @param i Index of the icon
     * @param m New mass of the icon
     */
    void setIconMass(int i, long double m)
    {
        icons[i].mass = m;
    }

    /**
     * @brief Add a new icon to the list of icons
     *
     * @param e movingEntity_2D object that contains the information of the icon
     */
    void addObject(const movingEntity_2D &e)
    {
        icons.push_back(e);
        iconCount++;
    }

    /**
     * @brief Updates all the icons in the desktop with the new positions that their velocities dictates
     */
    void update()
    {
        for (int i = 0; i < iconCount; i++)
        {
            moveIcon(i);
        }
    }

    /**
     * @brief Calculates the physics of all icons in the desktop
     */
    void physics()
    {
        movingEntity_2D entity1, entity2;
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
                    entity1 = icons[i];
                    entity2 = icons[j];
                    if ((entity1.pos - entity2.pos).r >= 2 * hitBox)
                    {
                        polarForm_2D force = gravity(entity1, entity2);
                        entity1.pushForce(force);
                    }
                    else
                    {
                        // long double aExit_i = icons[i].bounceAngle(icons[j]);
                        // long double vEntry_i = icons[i].vel.module();
                        // vector_2D vExit_i(vEntry_i * cos(aExit_i), vEntry_i * sin(aExit_i));
                        // icons[i].vel = vExit_i;

                        // if (true)
                        // {
                        //     continue;

                        //     // long double aExit_j = icons[j].bounceAngle(icons[i]);
                        //     // long double vEntry_j = icons[j].vel.module();
                        //     // vector_2D vExit_j(vEntry_j * cos(aExit_j), vEntry_j * sin(aExit_j));
                        //     // icons[j].vel = vExit_j;

                        //     // vector_2D movementQuantity = icons[i].vel * icons[i].mass + icons[j].vel * icons[j].mass;
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

    /**
     * @brief Some things that have to be done to the desktop to work properly
     */
    void doNeeds()
    {
        VirtualFreeEx(he, pC, 0, MEM_RELEASE);
    }
};

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
            oCoords = toPolarCoords(vector_2D(abs(info.rcMonitor.left), abs(info.rcMonitor.top)));
            oCoords += offset;
            oCoords += toPolarCoords(vector_2D(1920 / 2, 1080 / 2));
        }
    }
}

/**
 * @brief Gets the origin coords or (0, 0) of the whole environment
 */
void getOriginCoords()
{
    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);
}

const char *cfg_path = "config.json";
int main()
{
    loadConfig(cfg_path);

    getOriginCoords();
    desktop d;

    d.setIconPos(0, oCoords);
    d.setIconMass(0, massSun);

    cout << "--------------------------------------------" << endl;
    cout << "Icon count: " << d.iconCount << endl;
    cout << "oCoords: " << oCoords << endl;
    cout << "offset: " << offset << endl;
    cout << "--------------------------------------------" << endl;

    long double velocity, angle;
    for (int i = 1; i < d.iconCount; i++)
    {
        d.setIconPos(i, oCoords + toPolarCoords({random({-system_size.x / 2, system_size.x / 2}), random({-system_size.y / 2, system_size.y / 2})}));

        // Smart orbit velocity calculation
        velocity = sqrt(-(G * d.icons[0].mass / (d.icons[i].pos - d.icons[0].pos).r)) * (1 + random({-orbVel_modifier, orbVel_modifier}));
        angle = (d.icons[i].pos - d.icons[0].pos).theta + PI / 2;
        if (random({-1, 1}) < 0)
            angle += PI;

        d.setIconVel(i, polarForm_2D(velocity, angle));

        d.setIconMass(i, random(massRange));
    }
    d.update();

    while (true)
    {
        d.setIconVel(0, polarForm_2D(0, 0));
        d.physics();

        d.update();

        Sleep(1000 / physics_per_second);
    }

    d.doNeeds();
}