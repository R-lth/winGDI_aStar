#include "Monster.h"

void Monster::spawn()
{
    int i = rand() % middleOfCorner.size();

    POINT monster = middleOfCorner[i];

    if (okToGo(monster)  && !collideWithPlayer(monster))
    {
        deque<POINT> path = aStar.findPath(monster, PlayerState::Get().playerPos, GameState::Get().grid);

        if (!path.empty())
        {
            GameState::Get().pathInfo.emplace_back(path);
            int id = GameState::Get().pathInfo.size() - 1;
            GameState::Get().monsterPos.insert({ id, monster });
        }
    }
}

void Monster::move(HWND hWnd)
{
    using IT = std::map<int, POINT>::iterator;
    for (IT it = GameState::Get().monsterPos.begin(); it != GameState::Get().monsterPos.end();)
    {
        const int id = it->first;
        POINT next = it->second;

        if (id < 0 || id >= GameState::Get().pathInfo.size())
        {
            it = std::next(it);
            continue;
        }

        // 경로 소비
        // 맵의 at()은 key에 해당하는 값에 접근하는 함수로, []과 달리 키가 존재하지 않으면 접근하지 않는다.
        // []은 키가 없을 시 자동으로 삽입한다.
        while (!GameState::Get().pathInfo[id].empty() &&
            GameState::Get().monsterPos.at(id).x == next.x && GameState::Get().monsterPos.at(id).y == next.y)
        {
            next = GameState::Get().pathInfo[id].front();
            GameState::Get().pathInfo[id].pop_front();
        }

        if (!okToGo(GameState::Get().monsterPos.at(id))) 
        {
            it = std::next(it);
            continue;
        }

        if (collideWithOtherMonsters(id, GameState::Get().monsterPos.at(id)))
        {
            it = std::next(it);
            continue;
        }

        if (collideWithPlayer(GameState::Get().monsterPos.at(id)))
        {
            GameState::Get().waiting = true;
            KillTimer(hWnd, 1);
            KillTimer(hWnd, 2);
            KillTimer(hWnd, 3);
            SetTimer(hWnd, 4, 2000, NULL);
            break;
        }
        else
        {
            // 위치 갱신
            it->second = next;
            // 경로 재계산
            deque<POINT> path = aStar.findPath(it->second, PlayerState::Get().playerPos, GameState::Get().grid);
            if (id >= 0 && id < GameState::Get().pathInfo.size() && !path.empty())
            {
                GameState::Get().pathInfo[id] = path;
            }
            //
            it = std::next(it);
        }
    }
}

bool Monster::okToGo(POINT pos) 
{
    if (pos.x < 0 || pos.x >= 20 || pos.y < 0 || pos.y >= 20)
    {
        return false;
    }

    return (GameState::Get().grid[pos.y][pos.x] == 0);
}

bool Monster::collideWithPlayer(POINT pos)
{
    return (pos.x == PlayerState::Get().playerPos.x && pos.y == PlayerState::Get().playerPos.y);
}

bool Monster::collideWithOtherMonsters(int id, POINT pos)
{
    for (const pair<int, POINT>& it : GameState::Get().monsterPos)
    {
        if (id == it.first)
        {
            continue;
        }

        POINT other = it.second;
        if (pos.x == other.x && pos.y == other.y)
        {
            return true;
        }
    }

    return false;
}
