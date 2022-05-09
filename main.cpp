#include <windows.h>
#include <commctrl.h>
#include "Vec2.h"
#include <vector>
#include <iostream>

using namespace std;

class movingEntity
{
public:
    Vec2 pos;
    Vec2 vel;

    float mass;

    movingEntity()
    {
        pos = Vec2(0, 0);
        vel = Vec2(0, 0);
        mass = 1;
    }

    movingEntity(Vec2 p)
    {
        pos = p;
        vel = Vec2(0, 0);
        mass = 1;
    }

    movingEntity(Vec2 p, Vec2 v, float m)
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

    void push(Vec2 aceleration, float dt = 1)
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

    void update(HWND hd, float dt = 1)
    {
        pos += vel * dt;
        ListView_SetItemPosition(hd, 1, (int)pos.x + vel.x + 1920, (int)pos.y + vel.y);
    }
};

const float g = 9810.0f; // Gravity of Earth in m/s²
Vec2 gravity(movingEntity A, movingEntity B)
{
    return (B.pos - A.pos) / g;
}

class desktopIcon
{
public:
    string name;
    movingEntity entity;
    Vec2 offset;

    desktopIcon(string n)
    {
        name = n;
        entity = movingEntity();
        entity.mass = 1; // Hay que conseguir la masa del icono
        offset = Vec2(0, 0);
    }
};

// class desktop
// {
// private:
//     vector<desktopIcon> icons;

// public:
//     desktop(string desktopPath)
//     {
//         for (auto & entry : fs::directory_iterator(desktopPath))
//         {
//             if (entry.is_regular_file())
//             {
//                 desktopIcon icon(entry.path().filename().string());
//                 icons.push_back(icon);
//             }
//         }
//     }

//     void update(HWND hd, float dt = 1)
//     {
//         for (auto & icon : icons)
//         {
//             icon.entity.update(hd, dt);
//         }
//     }
// };

int main()
{
    HWND hd;

    HANDLE he;
    DWORD Pi;

    hd = FindWindowA("Progman", NULL);
    hd = FindWindowEx(hd, 0, "SHELLDLL_DefView", NULL);
    hd = FindWindowEx(hd, 0, "SysListView32", NULL);

    ListView_SetItemPosition(hd, 1, 100, 100);

    GetWindowThreadProcessId(hd, &Pi);
    he = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, false, Pi);

    POINT *pC = (POINT *)VirtualAllocEx(he, NULL, sizeof(POINT), MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(he, pC, &pC, sizeof(POINT), NULL);

    int count;
    count = SendMessage(hd, LVM_GETITEMCOUNT, 0, 0);
    cout << count << endl;

    /*POINT pt;
    ListView_GetItemPosition(hd, 0, pC);
    ReadProcessMemory(he, pC, &pt, sizeof(POINT), NULL);
    cout<<"X : "<<pt.x<<" Y : "<<pt.y<<endl;*/

    POINT pt;
    ListView_GetItemPosition(hd, 1, pC);
    ReadProcessMemory(he, pC, &pt, sizeof(POINT), NULL);
    movingEntity icon(Vec2(pt.x, pt.y));

    movingEntity cursor;
    while (true)
    {
        POINT px;
        GetCursorPos(&px);
        cursor.setPos(Vec2(px.x, px.y));
        icon.push(gravity(icon, cursor));

        // cout << "-----------------------" << endl;
        // cout << "X: " << cursor.pos.x << " Y: " << cursor.pos.y << endl;
        // cout << "Position: " << icon.pos.x << " " << icon.pos.y << endl;
        // cout << "Aceleration: " << aceleration.x << " " << aceleration.y << endl;
        // cout << "Velocity: " << icon.vel.x << " " << icon.vel.y << endl;

        icon.update(hd);

        Sleep(10);
    }

    VirtualFreeEx(he, pC, 0, MEM_RELEASE);
}
