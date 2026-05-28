#pragma once

#include <random>
#include "pch.h"
#include "../content/Player.h"
#include "../content/Monster.h"

using namespace std;

class Game
{
public:
	void init(HWND hWnd, RECT rect);
	void update(HWND hWnd, WPARAM wParam);
	void render(HDC hdc, HINSTANCE hInst);

	void renderBegin(HDC hdc, HINSTANCE hInst);
	void renderPlay();
	void renderEnd();

private:
    Player player;
    Monster monster;
    AStar aStar;

private:
    HDC back, scr;
    HBITMAP bmp, originalBmp;

    HBITMAP groundBmp[2]; // Aisle, Brick
    HBITMAP blackBmp;
    HBITMAP uiTextBmp[7]; // Ch1 대, Ch2 초, Ch3 원, Ch4 의, Ch5 왕, Ch6 모, Ch7 험; 

    HBITMAP playerBmp[4]; // Goal, GoalLeft, GoalRight, GoalUp
    HBITMAP bulletBmp;
    HBITMAP monsterBmp[2]; // Character1, Character2
    HBITMAP deadBmp;

private:
    int width;
    int height;
    const int cell = 20;
    const int n = 20;

    const vector<POINT> uiSpawn
    {
        // index로 char 판단
        // 대초원의
        // 왕의
        // 모험
        {9, 6}, {10, 6}, {11, 6}, {12, 6},
        {10, 7}, {11, 7},
        {10, 8}, {11, 8}
    };

    bool mFilp = false;
};

