#pragma once

#include "../framework/pch.h"

class Player
{
public:
	void move(bool wasd[4]);
	void loadingBullets(bool arrow[4]);
	void shoot();

private:
	bool okToGo(POINT pos);

private:
	AStar aStar;
};

