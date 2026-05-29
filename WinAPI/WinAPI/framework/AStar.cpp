#include "AStar.h"

deque<POINT> AStar::findPath(POINT start, POINT goal, const vector<vector<int>> grid)
{
	priority_queue<Node, vector<Node>, Compare> pq;
	map<POINT, POINT> visited;

	// 초기화
	Node startNode;
	startNode.current = start;
	startNode.parent = { -1, -1 };
	startNode.gCost = 0;
	startNode.hCost = heuristic(start, goal);

	// 시작
	pq.push(startNode);

	// 탐색
	while (!pq.empty())
	{
		// 최소 값
		Node node = pq.top();
		pq.pop();

		// 이미 방문
		if (visited.find(node.current) != visited.end())
		{
			continue;
		}

		// 이제 방문
		visited.insert({ node.current, node.parent });

		// 목적지 도착
		if (node.current.x == goal.x && node.current.y == goal.y) 
		{
			// 실제로 목적지 노드가 들어감. → 역추적
			return getPath(node.current, visited);
		}

		// 이웃 탐색
		for (const pair<POINT, float>& dir : direction)
		{
			int nextY = node.current.y + dir.first.y;
			int nextX = node.current.x + dir.first.x;
			POINT next = { nextX, nextY };
			
			int n = 20;
			
			if (nextX < 0 || nextX >= n || nextY < 0 || nextY >= n) 
			{
				continue;
			}

			if (grid[nextY][nextX]) 
			{
				continue;
			}

			if (visited.find(next) != visited.end())
			{
				continue;
			}

			Node neighbor;
			neighbor.current = next;
			neighbor.parent = node.current;
			neighbor.gCost = node.gCost + dir.second;
			neighbor.hCost = heuristic(next, goal);

			pq.push(neighbor);
		}
	}

	// 경로 없음
	return {};
}

deque<POINT> AStar::getPath(POINT current, map<POINT, POINT> visited)
{
	deque<POINT> path = {};
	POINT backtracking = current;

	while (backtracking.x != -1 && backtracking.y != -1) 
	{
		path.push_front(backtracking);

		if (visited.find(backtracking) != visited.end()) 
		{
			backtracking = visited[backtracking];
		}
		else 
		{
			// 문제 상황
			break;
		}
	}

	return path;
}

float AStar::heuristic(POINT next, POINT goal)
{
	int diffX = next.x - goal.x;
	int diffY = next.y - goal.y;
	return static_cast<float>(sqrt(diffX * diffX + diffY * diffY));
}
