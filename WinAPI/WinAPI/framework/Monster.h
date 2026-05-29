#pragma once

#include "../framework/pch.h"

using namespace std;

class Monster
{
public:
	void spawn();
	void move(HWND hWnd);

private:
    bool okToGo(POINT pos);
    bool collideWithPlayer(POINT pos);
    bool collideWithOtherMonsters(int id, POINT pos);

private:
    const vector<POINT> middleOfCorner =
    {
        {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12},
        {19, 7}, {19, 8}, {19, 9}, {19, 10}, {19, 11}, {19, 12},
        {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0},
        {7, 19}, {8, 19}, {9, 19}, {10, 19}, {11, 19}, {12, 19}
    };

    AStar aStar;
};

