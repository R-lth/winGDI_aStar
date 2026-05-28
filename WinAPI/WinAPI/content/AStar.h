#pragma once

#include "../framework/pch.h"

using namespace std;

struct Node
{
	POINT current;
	POINT parent;
	float gCost;
	float hCost;
};

inline const bool operator< (const POINT& a, const POINT& b)
{
	return (a.x == b.x) ? a.y < b.y : a.x < b.x;
}

struct Compare
{
	bool operator() (const Node& a, const Node& b)
	{
		return (a.gCost + a.hCost) > (b.gCost + b.hCost);
	}
};

class AStar
{
public:
	deque<POINT> findPath(POINT start, POINT goal, const vector<vector<int>> grid);

private:
	deque<POINT> getPath(POINT character, map<POINT, POINT> visited);
	float heuristic(POINT next, POINT goal);

private:
	const std::vector<pair<POINT, float>> direction =
	{
		{ {0, 1}, 1.0f }, { {0, -1}, 1.0f }, { {1, 0}, 1.0f }, { { -1, 0}, 1.0f},
		{ {1, 1}, 1.414f }, { {1, -1}, 1.414f }, { {-1, 1}, 1.414f}, { { -1, -1}, 1.414f}
	};
};