// WinAPI.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WinAPI.h"

#include "Game.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND hWnd;                                      // 창 핸들 전역으로 초기화.
Game game;
// TODO. 
RECT rect;
Player player;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    // 1. 윈도우 창 정보 등록
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    // 2. 윈도우 창 생성
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg = {};
    game.init(hWnd, rect);

    // 기본 메시지 루프입니다:
    while (msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            int a = 0;
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL; // 메뉴바 지움
    wcex.lpszClassName  = L"kohmeso";
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
// 
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   // 창 크기 설정
   rect = { 0, 0, GWinSizeX, GWinSizeY }; // 800, 600
   ::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

   hWnd = CreateWindowW(L"kohmeso", L"Client", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ::ShowWindow(hWnd, nCmdShow);
   ::UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
        case WM_TIMER:
        {
            game.update(hWnd, wParam);

            RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
        }
        break;
        case WM_KEYDOWN:  
        {
            // TODO. Game 객체로 프레임으로 입력 처리를 받아서, 자연스러운 입력 구현
            if (!GameState::Get().gameOver && !GameState::Get().waiting) 
            {
                bool wasd[4];
                wasd[0] = GetAsyncKeyState('W') & 0x8000;
                wasd[1] = GetAsyncKeyState('A') & 0x8000;
                wasd[2] = GetAsyncKeyState('S') & 0x8000;
                wasd[3] = GetAsyncKeyState('D') & 0x8000;

                bool arrow[4];
                arrow[0] = GetAsyncKeyState(VK_UP) & 0x8000;
                arrow[1] = GetAsyncKeyState(VK_LEFT) & 0x8000;
                arrow[2] = GetAsyncKeyState(VK_DOWN) & 0x8000;
                arrow[3] = GetAsyncKeyState(VK_RIGHT) & 0x8000;

                switch (wParam)
                {
                case 'W':
                case 'A':
                case 'S':
                case 'D':
                    player.move(wasd);
                    break;
                case VK_UP:
                case VK_LEFT:
                case VK_DOWN:
                case VK_RIGHT:
                    player.loadingBullets(arrow);
                    break;
                default:
                    break;
                }
            }
           
            RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
        }
        break;
        case WM_PAINT:
        {
            // 앞면 버퍼. 실제 화면
            hdc = BeginPaint(hWnd, &ps);
            // 그리기
            game.render(hdc, hInst);
            // 실제 화면에 최종 제출
            EndPaint(hWnd, &ps);
        }
        break;
        case WM_DESTROY:
        {
            
            PostQuitMessage(0);
        }
        break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
        return 0;
}

