#pragma once

#include <random>
#include "pch.h"
#include "wndBase.h"

#include "../content/Player.h"
#include "../content/Monster.h"

namespace monkeyEngine
{
    class Game : private IWndBase
    {
    public:
        Game() = default;
        ~Game() override = default;

        // 복사 생성자, 대입 불가
        Game(const Game&) = delete;
        Game& operator=(const Game&) = delete;
        // 이동 생성자, 대입 불가
        Game(Game&&) = delete;
        Game& operator=(Game&&) = delete;

        void Run();

    private:
        void OnResize(int width, int height) override;
        void OnClose() override;

        void renderDataLoad();
        void renderDataRelease();

    private:
        void init();
        void input();
        void update(float deltaTime);
        void render();

    private:
        bool isRun = true;
        bool wasd[4] = {};
        bool arrow[4] = {};
        
    private:
        float timer = 0.f;

    private:
        // todo. 액터 단위로
        Player player;
        Monster monster;
        AStar aStar;
        bool mFilp = false;

    private:
        HDC m_hFrontDC = nullptr;
        HDC m_hBackDC = nullptr;
        HDC m_hSrcDC = nullptr;

    private:
        HBITMAP m_hBackBitmap = nullptr;
        HBITMAP m_hDefaultBitmap = nullptr;

    private:
        HBITMAP groundBmp[2] = {};
        HBITMAP blackBmp = nullptr;
        HBITMAP uiTextBmp[7] = {};

        HBITMAP playerBmp[4] = {};
        HBITMAP bulletBmp = nullptr;
        HBITMAP monsterBmp[2] = {};
        HBITMAP deadBmp = nullptr;

    private:
        static const int cell = 20;
        static const int n = 20;

        const vector<POINT> uiSpawn
        {
            {9, 6}, {10, 6}, {11, 6}, {12, 6},
            {10, 7}, {11, 7},
            {10, 8}, {11, 8}
        };
    };
}

