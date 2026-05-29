#include "Player.h"

void Player::move(bool wasd[4])
{
    POINT next = PlayerState::Get().playerPos;

    if (wasd[0] && wasd[1])
    {
        next.x -= 1;
        next.y -= 1;
        // 
        PlayerState::Get().pHoriz = false;
        PlayerState::Get().pVert = true;
    }
    else if (wasd[0] && wasd[3])
    {
        next.x += 1;
        next.y -= 1;
        //
        PlayerState::Get().pHoriz = false;
        PlayerState::Get().pVert = true;
    }
    else if (wasd[2] && wasd[1])
    {
        next.x -= 1;
        next.y += 1;
        //
        PlayerState::Get().pHoriz = false;
        PlayerState::Get().pVert = false;
    }
    else if (wasd[2] && wasd[3])
    {
        next.x += 1;
        next.y += 1;
        // 
        PlayerState::Get().pHoriz = false;
        PlayerState::Get().pVert = false;
    }
    else if (wasd[0])
    {
        next.y -= 1;
        PlayerState::Get().pHoriz = false;
        PlayerState::Get().pVert = true;
    }
    else if (wasd[1])
    {
        next.x -= 1;
        PlayerState::Get().pHoriz = true;
        PlayerState::Get().pFilp = !PlayerState::Get().pFilp;
    }
    else if (wasd[2])
    {
        next.y += 1;
        PlayerState::Get().pHoriz = false;
        PlayerState::Get().pVert = false;
    }
    else if (wasd[3])
    {
        next.x += 1;
        PlayerState::Get().pHoriz = true;
        PlayerState::Get().pFilp = !PlayerState::Get().pFilp;
    }

    if (!okToGo(next)) 
    {
        return;
    }
    
    using IT = std::map<int, POINT>::iterator;
    for (IT it = GameState::Get().monsterPos.begin(); it != GameState::Get().monsterPos.end();)
    {
        int id = it->first;
        deque<POINT> path = aStar.findPath(GameState::Get().monsterPos.at(id), PlayerState::Get().playerPos, GameState::Get().grid);

        // 플레이어 기준으로 몬스터 경로 갱신
        if (id >= 0 && id < GameState::Get().pathInfo.size() && !path.empty())
        {
            GameState::Get().pathInfo[id] = path;
        }

        it = ::next(it);
    }

    PlayerState::Get().playerPos = next;
}

void Player::loadingBullets(bool arrow[4])
{
    ShootDir dir = ShootDir::None;
    POINT bullet = PlayerState::Get().playerPos;

    if (arrow[0] && arrow[1])
    {
        bullet.x -= 1;
        bullet.y -= 1;
        dir = ShootDir::UpLeft;
    }
    else if (arrow[0] && arrow[3])
    {
        bullet.x += 1;
        bullet.y -= 1;
        dir = ShootDir::UpRight;
    }
    else if (arrow[2] && arrow[1])
    {
        bullet.x -= 1;
        bullet.y += 1;
        dir = ShootDir::DownLeft;
    }
    else if (arrow[2] && arrow[3])
    {
        bullet.x += 1;
        bullet.y += 1;
        dir = ShootDir::DownRight;
    }
    else if (arrow[0])
    {
        bullet.y -= 1;
        dir = ShootDir::Up;
    }
    else if (arrow[1])
    {
        bullet.x -= 1;
        dir = ShootDir::Left;
    }
    else if (arrow[2])
    {
        bullet.y += 1;
        dir = ShootDir::Down;
    }
    else if (arrow[3])
    {
        bullet.x += 1;
        dir = ShootDir::Right;
    }

    if (okToGo(bullet))
    {
        PlayerState::Get().gun.push_back({ dir, bullet });
    }
}

void Player::shoot()
{
    using It_gun = std::list<std::pair<ShootDir, POINT>>::iterator;
    using It_mp = std::map<int, POINT>::iterator;

    for (It_gun it_g = PlayerState::Get().gun.begin(); it_g != PlayerState::Get().gun.end();)
    {
        POINT bullet = it_g->second;

        switch (it_g->first)
        {
        case ShootDir::UpLeft:
            bullet.x -= 1;
            bullet.y -= 1;
            break;
        case ShootDir::UpRight:
            bullet.x += 1;
            bullet.y -= 1;
            break;
        case ShootDir::DownLeft:
            bullet.x -= 1;
            bullet.y += 1;
            break;
        case ShootDir::DownRight:
            bullet.x += 1;
            bullet.y += 1;
            break;
        case ShootDir::Up:
            bullet.y -= 1;
            break;
        case ShootDir::Left:
            bullet.x -= 1;
            break;
        case ShootDir::Down:
            bullet.y += 1;
            break;
        case ShootDir::Right:
            bullet.x += 1;
            break;
        default:
            break;
        }

        if (!okToGo(bullet))
        {
            it_g = PlayerState::Get().gun.erase(it_g);
            continue;
        }

        // 피격
        bool hit = false;
        for (It_mp it_m = GameState::Get().monsterPos.begin(); it_m != GameState::Get().monsterPos.end();)
        {
            if (bullet.x == it_m->second.x && bullet.y == it_m->second.y)
            {
                hit = true;
                it_m = GameState::Get().monsterPos.erase(it_m);
                it_g = PlayerState::Get().gun.erase(it_g);
                break; // 몬스터 루프만 탈출
            }

            it_m = ::next(it_m);
        }

        if (!hit)
        {
            it_g->second = bullet;
            it_g = ::next(it_g);
        }
    }
}

bool Player::okToGo(POINT pos)
{
    if (pos.x < 0 || pos.x >= 20 || pos.y < 0 || pos.y >= 20) 
    {
        return false;
    }

    return (GameState::Get().grid[pos.y][pos.x] == 0);
}
