#pragma once

#include "pch.h"
#include "Singleton.h"

using namespace std;

enum class ShootDir 
{
    UpLeft,
    UpRight,
    DownLeft,
    DownRight,
    Up,
    Left,
    Down,
    Right,
    None
};

// TODO. GameState 하위로 관리할지에 대해서 고민해 보기
class PlayerState : public Singleton<PlayerState> 
{
public:
    PlayerState()
    {
        gun.resize(8);
    }

    // TODO. setter·getter
public:
    POINT playerPos = { 10, 10 };

    bool pHoriz = false;
    bool pVert = false;
    bool pFilp = false;
    
    list<pair<ShootDir, POINT>> gun;
};