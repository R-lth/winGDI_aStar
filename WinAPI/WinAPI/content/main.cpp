
#include "../framework/Game.h"

int main()
{
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE); // 콘솔 창 숨김 todo. 디버깅 시에는 켜기

    monkeyEngine::Game game;
    game.Run();

    return 0;
}