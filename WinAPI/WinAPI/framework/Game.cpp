#include "Game.h"
#include "Resource.h"

// 가상 함수 재정의
void Game::OnResize(int width, int height)
{
    __super::OnResize(width, height);
}

void Game::OnClose()
{
    renderDataRelease();

    if (m_hBackBitmap)
        DeleteObject(m_hBackBitmap);

    if (m_hSrcDC)
        DeleteDC(m_hSrcDC);

    if (m_hBackDC)
        DeleteDC(m_hBackDC);

    if (m_hFrontDC)
        ReleaseDC(m_hWnd, m_hFrontDC);

    m_hBackDC = nullptr;
    m_hFrontDC = nullptr;
    m_hSrcDC = nullptr;
    m_hBackBitmap = nullptr;
    m_hDefaultBitmap = nullptr;

    __super::Destroy();
}

// 렌더 데이터
void Game::renderDataLoad()
{
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

void Game::renderDataRelease()
{
    SelectObject(m_hBackDC, m_hDefaultBitmap);

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
}

// 게임 로직
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

        if (deltaTime < 1.f / 120.f) { continue; } // 프레임 안정화
        previousTime = currentTime;

        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // 메세지 큐 처리
        {
            if (msg.message == WM_QUIT) { isRun = false; }
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
    const wchar_t* className = L"Game";
    const wchar_t* windowName = L"Client";
    if (false == __super::Create(className, windowName, m_width, m_height)) { return; }

    RECT rect = {};
    GetClientRect(m_hWnd, &rect);
    // m_width = rcClient.right - rcClient.left;
    // m_height = rcClient.bottom - rcClient.top;

    m_hFrontDC = GetDC(m_hWnd);
    m_hBackDC = CreateCompatibleDC(m_hFrontDC);
    m_hSrcDC = CreateCompatibleDC(m_hFrontDC);

    m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);
    m_hDefaultBitmap = static_cast<HBITMAP>(SelectObject(m_hBackDC, m_hBackBitmap));
    renderDataLoad();

    srand(static_cast<unsigned int>(time(nullptr)));
}

void Game::input()
{
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
    timer0 += deltaTime;
    timer1 += deltaTime;
    timer2 += deltaTime;
    timer3 += deltaTime;

    if (timer0 >= 0.05f)
    {
        if (wasd[0] || wasd[1] || wasd[2] || wasd[3]) { player.move(wasd); }
        if (arrow[0] || arrow[1] || arrow[2] || arrow[3]) { player.loadingBullets(arrow); }

        timer0 = 0.f;
    }

    if (timer1 >= 0.15f)
    {
        player.shoot();

        timer1 = 0.f;
    }

    if (timer2 >= 0.3f)
    {
        monster.move(m_hWnd);

        timer2 = 0.f;
    }

    if (timer3 >= 1.5f)
    {
        monster.spawn();

        timer3 = 0.f;
    }

    if (GameState::Get().waiting)
    {
        deathSceneTimer += deltaTime;

        if (deathSceneTimer >= 1.5f)
        {
            GameState::Get().waiting = false;
            GameState::Get().gameOver = true;
            deathSceneTimer = 0.f;
        }
    }
}

void Game::render()
{
    if (!GameState::Get().gameOver)
    {
        for (int y = 0; y < n; ++y)
        {
            for (int x = 0; x < n; ++x)
            {
                HBITMAP tile = GameState::Get().grid[y][x] ? groundBmp[1] : groundBmp[0];
                SelectObject(m_hSrcDC, tile);
                BitBlt(m_hBackDC, x * cell, y * cell, cell, cell, m_hSrcDC, 0, 0, SRCCOPY);
            }
        }

        if (!GameState::Get().waiting)
        {
            HBITMAP mSprite = mFilp ? monsterBmp[0] : monsterBmp[1];
            mFilp = !mFilp;

            SelectObject(m_hSrcDC, mSprite);

            for (const pair<int, POINT>& monster : GameState::Get().monsterPos)
            {
                POINT pos = monster.second;
                BitBlt(m_hBackDC, pos.x * cell, pos.y * cell, cell, cell, m_hSrcDC, 0, 0, SRCCOPY);
            }

            SelectObject(m_hSrcDC, bulletBmp);

            for (auto it = PlayerState::Get().gun.begin(); it != PlayerState::Get().gun.end(); ++it)
            {
                POINT bullet = it->second;
                BitBlt(m_hBackDC, bullet.x * cell, bullet.y * cell, cell, cell, m_hSrcDC, 0, 0, SRCCOPY);
            }

            if (PlayerState::Get().pHoriz)
            {
                HBITMAP pSprite = PlayerState::Get().pFilp ? playerBmp[1] : playerBmp[2];
                SelectObject(m_hSrcDC, pSprite);
            }
            else
            {
                HBITMAP pSprite = PlayerState::Get().pVert ? playerBmp[3] : playerBmp[0];
                SelectObject(m_hSrcDC, pSprite);
            }

            BitBlt(
                m_hBackDC,
                PlayerState::Get().playerPos.x * cell,
                PlayerState::Get().playerPos.y * cell,
                cell,
                cell,
                m_hSrcDC,
                0,
                0,
                SRCCOPY
            );
        }
        else
        {
            SelectObject(m_hSrcDC, deadBmp);

            for (const pair<int, POINT>& it : GameState::Get().monsterPos)
            {
                POINT pos = it.second;
                BitBlt(m_hBackDC, pos.x * cell, pos.y * cell, cell, cell, m_hSrcDC, 0, 0, SRCCOPY);
            }

            SelectObject(m_hSrcDC, groundBmp[0]);

            for (auto it = PlayerState::Get().gun.begin(); it != PlayerState::Get().gun.end(); ++it)
            {
                POINT bullet = it->second;
                BitBlt(m_hBackDC, bullet.x * cell, bullet.y * cell, cell, cell, m_hSrcDC, 0, 0, SRCCOPY);
            }

            SelectObject(m_hSrcDC, deadBmp);

            BitBlt(
                m_hBackDC,
                PlayerState::Get().playerPos.x * cell,
                PlayerState::Get().playerPos.y * cell,
                cell,
                cell,
                m_hSrcDC,
                0,
                0,
                SRCCOPY
            );
        }
    }
    else
    {
        SelectObject(m_hSrcDC, blackBmp);

        for (int y = 0; y < n; ++y)
        {
            for (int x = 0; x < n; ++x)
            {
                BitBlt(m_hBackDC, x * cell, y * cell, cell, cell, m_hSrcDC, 0, 0, SRCCOPY);
            }
        }

        for (int i = 0; i < static_cast<int>(uiSpawn.size()); ++i)
        {
            if (i < 5)
                SelectObject(m_hSrcDC, uiTextBmp[i]);
            else if (i == 5)
                SelectObject(m_hSrcDC, uiTextBmp[3]);
            else
                SelectObject(m_hSrcDC, uiTextBmp[i - 1]);

            int x = uiSpawn[i].x;
            int y = uiSpawn[i].y;

            BitBlt(m_hBackDC, x * cell, y * cell, cell, cell, m_hSrcDC, 0, 0, SRCCOPY);
        }
    }

    BitBlt(m_hFrontDC, 0, 0, m_width, m_height, m_hBackDC, 0, 0, SRCCOPY);
}