#include "Game.h"
#include "Resource.h"

void Game::init(HWND hWnd, RECT rect)
{
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;

    // 몬스터 생성 위치 관련 랜덤 값을 위한 함수
    srand(time(NULL));

    // 타이머
    SetTimer(hWnd, 1, 150 * 2, NULL);
    SetTimer(hWnd, 2, 1500, NULL);
    SetTimer(hWnd, 3, 150, NULL);
}

void Game::update(HWND hWnd, WPARAM wParam)
{
    switch (wParam)
    {
    case 1:
    {
        monster.move(hWnd);
    }
    break;
    case 2:
    {
        monster.spawn();
    }
    break;
    case 3:
    {
        player.shoot();
    }
    break;
    // 충돌 시
    case 4:
    {
        KillTimer(hWnd, 4);

        if (GameState::Get().waiting)
        {
            GameState::Get().waiting = false;
            GameState::Get().gameOver = true;

            // 다시 그려야 할 영역 무효화
            RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
            // 바로 업데이트
            UpdateWindow(hWnd);
        }
    }
    break;
    default:
        break;
    }
}

void Game::render(HDC hdc, HINSTANCE hInst)
{
    // 로드
    renderBegin(hdc, hInst);
    // 그리기
    renderPlay();
    // 실제 화면 출력
    BitBlt(hdc, 0, 0, width, height, back, 0, 0, SRCCOPY);
    // 해제
    renderEnd();
}

void Game::renderBegin(HDC hdc, HINSTANCE hInst)
{
    back = CreateCompatibleDC(hdc);
    scr = CreateCompatibleDC(hdc);
    bmp = CreateCompatibleBitmap(hdc, width, height);
    originalBmp = (HBITMAP)SelectObject(back, bmp);

    // 땅
    groundBmp[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_AISLE));
    groundBmp[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BRICK));
    // 
    blackBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BLACK));
    // UI 텍스터
    uiTextBmp[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH1));
    uiTextBmp[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH2));
    uiTextBmp[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH3));
    uiTextBmp[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH4));
    uiTextBmp[4] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH5));
    uiTextBmp[5] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH6));
    uiTextBmp[6] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CH7));

    // 플레이어
    playerBmp[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOAL));
    playerBmp[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOALLEFT));
    playerBmp[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOALRIGHT));
    playerBmp[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GOALUP));
    // 총
    bulletBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BULLET));
    // 몬스터
    monsterBmp[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CHARACTER1));
    monsterBmp[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CHARACTER2));
    // 죽음
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
            // 몬스터
            HBITMAP mSprite = mFilp ? monsterBmp[0] : monsterBmp[1];
            mFilp = !mFilp;
            SelectObject(scr, mSprite);

            for (const pair<int, POINT>& monster : GameState::Get().monsterPos)
            {
                POINT pos = monster.second;
                BitBlt(back, pos.x * cell, pos.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
            }

            // 총알
            SelectObject(scr, bulletBmp);
            using It = list<pair<ShootDir, POINT>>::iterator;
            for (It it = PlayerState::Get().gun.begin(); it != PlayerState::Get().gun.end();)
            {
                POINT bullet = it->second;
                BitBlt(back, bullet.x * cell, bullet.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
                it = next(it);
            }

            // 플레이어
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
            BitBlt(back, PlayerState::Get().playerPos.x * cell, PlayerState::Get().playerPos.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
        }
        else
        {
            // 몬스터 죽음
            SelectObject(scr, deadBmp);
            for (const pair<int, POINT>& it : GameState::Get().monsterPos)
            {
                POINT pos = it.second;
                BitBlt(back, pos.x * cell, pos.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
            }

            // 총알 지우기
            SelectObject(scr, groundBmp[0]);
            using It = list<pair<ShootDir, POINT>>::iterator;
            for (It it = PlayerState::Get().gun.begin(); it != PlayerState::Get().gun.end();)
            {
                POINT bullet = it->second;
                BitBlt(back, bullet.x * cell, bullet.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
                it = next(it);
            }

            // 플레이어 죽음
            SelectObject(scr, deadBmp);
            BitBlt(back, PlayerState::Get().playerPos.x * cell, PlayerState::Get().playerPos.y * cell, cell, cell, scr, 0, 0, SRCCOPY);
        }
    }
    else
    {
        for (int y = 0; y < n; ++y)
        {
            for (int x = 0; x < n; ++x)
            {
                SelectObject(scr, blackBmp);
                BitBlt(back, x * cell, y * cell, cell, cell, scr, 0, 0, SRCCOPY);
            }
        }

        for (int i = 0; i < uiSpawn.size(); ++i)
        {
            if (i < 5)
            {
                SelectObject(scr, uiTextBmp[i]);
            }
            else if (i == 5) 
            {
                SelectObject(scr, uiTextBmp[3]);
            }
            else if (i > 5)
            {
                SelectObject(scr, uiTextBmp[i - 1]);
            }

            int x = uiSpawn[i].x;
            int y = uiSpawn[i].y;
            BitBlt(back, x * cell, y * cell, cell, cell, scr, 0, 0, SRCCOPY);
        }
    }
}

void Game::renderEnd()
{
    // 메모리 누수 방지
    SelectObject(back, originalBmp);
    // 메모리 해제
    for (int i = 0; i < 2; ++i)
    {
        DeleteObject(groundBmp[i]);
    }

    DeleteObject(blackBmp);

    for (int i = 0; i < 7; ++i) 
    {
        DeleteObject(uiTextBmp[i]);
    }

    for (int i = 0; i < 4; ++i) 
    {
        DeleteObject(playerBmp[i]);
    }

    DeleteObject(bulletBmp);

    for (int i = 0; i < 2; ++i) 
    {
        DeleteObject(monsterBmp[i]);
    }

    DeleteObject(deadBmp);
    /////////////////
    DeleteObject(bmp);
    DeleteObject(originalBmp);
    DeleteDC(back);
    DeleteDC(scr);
}