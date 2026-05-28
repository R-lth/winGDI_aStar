#pragma once

#include <random>
#include "pch.h"
#include "wndBase.h"

#include "../content/Player.h"
#include "../content/Monster.h"

namespace monkeyEngine
{
    class Game : protected IWndBase
    {
    public:
        Game() = default;
        ~Game() override = default;

        void Run();

    private:
        void OnResize(int width, int height) override;
        void OnClose() override;

    private:
        void init();
        void input();
        void update(float deltaTime);
        void render();

        void updateMonsterMove();
        void updateMonsterSpawn();
        void updateBulletShoot();

        void renderBegin();
        void renderPlay();
        void renderEnd();

    private:
        Player player;
        Monster monster;
        AStar aStar;

    private:
        HDC back = nullptr;
        HDC scr = nullptr;
        HBITMAP bmp = nullptr;
        HBITMAP originalBmp = nullptr;

        HBITMAP groundBmp[2] = {};
        HBITMAP blackBmp = nullptr;
        HBITMAP uiTextBmp[7] = {};

        HBITMAP playerBmp[4] = {};
        HBITMAP bulletBmp = nullptr;
        HBITMAP monsterBmp[2] = {};
        HBITMAP deadBmp = nullptr;

    private:
        int width = 400;
        int height = 400;

        const int cell = 20;
        const int n = 20;

        const vector<POINT> uiSpawn
        {
            {9, 6}, {10, 6}, {11, 6}, {12, 6},
            {10, 7}, {11, 7},
            {10, 8}, {11, 8}
        };

        bool mFilp = false;
        bool isRun = true;

    private:
        HDC m_hFrontDC = nullptr;

        float monsterMoveTimer = 0.0f;
        float monsterSpawnTimer = 0.0f;
        float bulletShootTimer = 0.0f;
    };
}

