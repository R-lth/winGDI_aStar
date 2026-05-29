#pragma once

#include "pch.h"
#include "Singleton.h"

using namespace std;

class GameState : public Singleton<GameState>
{
public:
    GameState() 
    {
        grid.resize(20, vector<int>(20, 0));

        for (int i = 0; i < 20; ++i)
        {
            grid[0][i] = 1;
            grid[19][i] = 1;
            grid[i][0] = 1;
            grid[i][19] = 1;
        }

        for (int i = 7; i < 13; ++i)
        {
            grid[0][i] = 0;
            grid[19][i] = 0;
            grid[i][0] = 0;
            grid[i][19] = 0;
        }
    }

    // TODO. setter·getter
public:
    bool gameOver = false;
    bool waiting = false;
    
    vector<vector<int>> grid;
    vector<deque<POINT>> pathInfo;
    map<int, POINT> monsterPos;
};