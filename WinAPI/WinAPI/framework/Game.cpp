#include "Game.h"
#include "Resource.h"

namespace monkeyEngine
{
    void Game::Run()
    {
        init();

        MSG msg = {};

        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        LARGE_INTEGER count;
        QueryPerformanceCounter(&count);

        __int64 currentTime, previousTime;
        currentTime = count.QuadPart;
        previousTime = currentTime;

        float deltaTime = 0.f;

        while (isRun)
        {
            QueryPerformanceCounter(&count);
            currentTime = count.QuadPart;
            float deltaTime = static_cast<float>(currentTime - previousTime) /
                               static_cast<float>(frequency.QuadPart);

            if (deltaTime < 1.f / 120.f) { continue; }
            previousTime = currentTime;

            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // 메세지 큐 처리
            {
                if (msg.message == WM_QUIT)
                {
                    isRun = false;
                    break;
                }
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else // 메세지가 없을 경우 게임 업데이트
            {
                input();
                update(deltaTime);
                render();
            }
        }

        OnClose();
    }

    void Game::init()
    {
        Create(L"kohmeso", L"Client", 400, 400);

        RECT rcClient = {};
        GetClientRect(m_hWnd, &rcClient);

        width = rcClient.right - rcClient.left;
        height = rcClient.bottom - rcClient.top;

        m_hFrontDC = GetDC(m_hWnd);

        srand(static_cast<unsigned int>(time(nullptr)));
    }

    void Game::input()
    {
        if (GameState::Get().gameOver || GameState::Get().waiting) { return; }

        wasd[0] = GetAsyncKeyState('W') & 0x8000;
        wasd[1] = GetAsyncKeyState('A') & 0x8000;
        wasd[2] = GetAsyncKeyState('S') & 0x8000;
        wasd[3] = GetAsyncKeyState('D') & 0x8000;

        arrow[0] = GetAsyncKeyState(VK_UP) & 0x8000;
        arrow[1] = GetAsyncKeyState(VK_LEFT) & 0x8000;
        arrow[2] = GetAsyncKeyState(VK_DOWN) & 0x8000;
        arrow[3] = GetAsyncKeyState(VK_RIGHT) & 0x8000;

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) { isRun = false; }
    }

    void Game::update(float deltaTime)
    {
        if (wasd[0] || wasd[1] || wasd[2] || wasd[3]) { player.move(wasd); }
        if (arrow[0] || arrow[1] || arrow[2] || arrow[3]) { player.loadingBullets(arrow); }

        monsterMoveTimer += deltaTime;
        monsterSpawnTimer += deltaTime;
        bulletShootTimer += deltaTime;

        if (monsterMoveTimer >= 0.3f)
        {
            monsterMoveTimer = 0.0f;
            monster.move(m_hWnd);
        }

        if (monsterSpawnTimer >= 1.5f)
        {
            monsterSpawnTimer = 0.0f;
            monster.spawn();
        }

        if (bulletShootTimer >= 0.15f)
        {
            bulletShootTimer = 0.0f;
            player.shoot();
        }

        if (GameState::Get().waiting)
        {
            GameState::Get().waiting = false;
            GameState::Get().gameOver = true;
        }
    }

    void Game::render()
    {
        renderBegin();
        renderPlay();

        BitBlt(m_hFrontDC, 0, 0, width, height, back, 0, 0, SRCCOPY);

        renderEnd();
    }

    void Game::renderBegin()
    {
        back = CreateCompatibleDC(m_hFrontDC);
        scr = CreateCompatibleDC(m_hFrontDC);

        bmp = CreateCompatibleBitmap(m_hFrontDC, width, height);
        originalBmp = static_cast<HBITMAP>(SelectObject(back, bmp));

        HINSTANCE hInst = GetModuleHandle(nullptr);

        groundBmp[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_AISLE));
        groundBmp[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BRICK));

        blackBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BLACK));

        uiTextBmp[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH1));
        uiTextBmp[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH2));
        uiTextBmp[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH3));
        uiTextBmp[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH4));
        uiTextBmp[4] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH5));
        uiTextBmp[5] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH6));
        uiTextBmp[6] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH7));

        playerBmp[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOAL));
        playerBmp[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOALLEFT));
        playerBmp[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOALRIGHT));
        playerBmp[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOALUP));

        bulletBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BULLET));

        monsterBmp[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CHARACTER1));
        monsterBmp[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CHARACTER2));

        deadBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_DEAD));
    }

    void Game::renderPlay()
    {
        if (!GameState::Get().gameOver)
        {
            for (int y = 0; y < n; ++y)
            {
                for (int x = 0; x < n; ++x)
                {
                    HBITMAP tile = GameState::Get().grid[y][x] ? groundBmp[1] : groundBmp[0];
                    SelectObject(scr, tile);
                    BitBlt(back, x * cell, y * cell, cell, cell, scr, 0, 0, SRCCOPY);
                }
            }

            if (!GameState::Get().waiting)
            {
                HBITMAP mSprite = mFilp ? monsterBmp[0] : monsterBmp[1];
                mFilp = !mFilp;

                SelectObject(scr, mSprite);

                for (const pair<int, POINT>& monster : GameState::Get().monsterPos)
                {
                    POINT pos = monster.second;
                    BitBlt(back, pos.x * cell, pos.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
                }

                SelectObject(scr, bulletBmp);

                for (auto it = PlayerState::Get().gun.begin(); it != PlayerState::Get().gun.end(); ++it)
                {
                    POINT bullet = it->second;
                    BitBlt(back, bullet.x * cell, bullet.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
                }

                if (PlayerState::Get().pHoriz)
                {
                    HBITMAP pSprite = PlayerState::Get().pFilp ? playerBmp[1] : playerBmp[2];
                    SelectObject(scr, pSprite);
                }
                else
                {
                    HBITMAP pSprite = PlayerState::Get().pVert ? playerBmp[3] : playerBmp[0];
                    SelectObject(scr, pSprite);
                }

                BitBlt(
                    back,
                    PlayerState::Get().playerPos.x * cell,
                    PlayerState::Get().playerPos.y * cell,
                    cell,
                    cell,
                    scr,
                    0,
                    0,
                    SRCCOPY
                );
            }
            else
            {
                SelectObject(scr, deadBmp);

                for (const pair<int, POINT>& it : GameState::Get().monsterPos)
                {
                    POINT pos = it.second;
                    BitBlt(back, pos.x * cell, pos.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
                }

                SelectObject(scr, groundBmp[0]);

                for (auto it = PlayerState::Get().gun.begin(); it != PlayerState::Get().gun.end(); ++it)
                {
                    POINT bullet = it->second;
                    BitBlt(back, bullet.x * cell, bullet.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
                }

                SelectObject(scr, deadBmp);

                BitBlt(
                    back,
                    PlayerState::Get().playerPos.x * cell,
                    PlayerState::Get().playerPos.y * cell,
                    cell,
                    cell,
                    scr,
                    0,
                    0,
                    SRCCOPY
                );
            }
        }
        else
        {
            SelectObject(scr, blackBmp);

            for (int y = 0; y < n; ++y)
            {
                for (int x = 0; x < n; ++x)
                {
                    BitBlt(back, x * cell, y * cell, cell, cell, scr, 0, 0, SRCCOPY);
                }
            }

            for (int i = 0; i < static_cast<int>(uiSpawn.size()); ++i)
            {
                if (i < 5)
                    SelectObject(scr, uiTextBmp[i]);
                else if (i == 5)
                    SelectObject(scr, uiTextBmp[3]);
                else
                    SelectObject(scr, uiTextBmp[i - 1]);

                int x = uiSpawn[i].x;
                int y = uiSpawn[i].y;

                BitBlt(back, x * cell, y * cell, cell, cell, scr, 0, 0, SRCCOPY);
            }
        }
    }

    void Game::renderEnd()
    {
        SelectObject(back, originalBmp);

        for (int i = 0; i < 2; ++i)
            DeleteObject(groundBmp[i]);

        DeleteObject(blackBmp);

        for (int i = 0; i < 7; ++i)
            DeleteObject(uiTextBmp[i]);

        for (int i = 0; i < 4; ++i)
            DeleteObject(playerBmp[i]);

        DeleteObject(bulletBmp);

        for (int i = 0; i < 2; ++i)
            DeleteObject(monsterBmp[i]);

        DeleteObject(deadBmp);

        DeleteObject(bmp);

        DeleteDC(back);
        DeleteDC(scr);

        back = nullptr;
        scr = nullptr;
        bmp = nullptr;
        originalBmp = nullptr;
    }

    // todo.
    void Game::OnResize(int width, int height)
    {
        IWndBase::OnResize(width, height);

        this->width = width;
        this->height = height;
    }

    void Game::OnClose()
    {
        isRun = false;

        ReleaseDC(m_hWnd, m_hFrontDC);
        m_hFrontDC = nullptr;

        Destroy();
    }
}