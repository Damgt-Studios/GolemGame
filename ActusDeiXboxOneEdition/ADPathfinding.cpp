#include "pch.h"
#include "ADPathfinding.h"

void ADAI::ADPathfinding::CreatePointGrid(std::vector<SimpleVertex>* _planeVertices)
{
	for (int r = 0; r < tileMap.yDivisions; r++)
	{
		for (int c = 0; c < tileMap.xDivisions; c++)
		{
			pointGrid.push_back({ XMFLOAT3(c * tileMap.cellSize.x, 0, r * tileMap.cellSize.y), true });
		}
	}

	//Get Heighmap for visuals
	for (int i = 0; i < _planeVertices->size(); i++)
	{
		UINT pointColumn = (((*_planeVertices)[i].Position.x - tileMap.planeLows.x) * tileMap.mapSize.x) / tileMap.cellSize.x;
		UINT pointRow = (((*_planeVertices)[i].Position.z - tileMap.planeLows.y) * tileMap.mapSize.y) / tileMap.cellSize.y;
		if (pointColumn < tileMap.xDivisions && pointRow < tileMap.yDivisions)
		{
			if (std::fmodf((((*_planeVertices)[i].Position.x - tileMap.planeLows.x) * tileMap.mapSize.x), tileMap.cellSize.x) > (tileMap.cellSize.x / 2.f))
			{
				if (pointColumn < tileMap.columns)
					++pointColumn;
			}

			if (std::fmodf((((*_planeVertices)[i].Position.z - tileMap.planeLows.y) * tileMap.mapSize.y), tileMap.cellSize.y) > (tileMap.cellSize.y / 2.f))
			{
				if (pointRow < tileMap.rows)
					++pointRow;
			}
			pointGrid[(pointColumn + (tileMap.xDivisions * pointRow))].position.y = (*_planeVertices)[i].Position.y;
		}
	}

	//DO COLLISIONS against pointGrid.
	int OBJ_COUNT = ResourceManager::GetGameObjectCount();
	ADResource::ADGameplay::GameObject** OBJS = ResourceManager::GetGameObjectPtr();

	for (int i = 0; i < OBJ_COUNT; i++)
	{
		if (OBJS[i]->colliderPtr != nullptr)
		{
			if (OBJS[i]->physicsType == ADResource::ADGameplay::STATIC)
			{
				if (OBJS[i]->colliderPtr->type != ADPhysics::ColliderType::Plane)
				{
					XMFLOAT4 colliderQuad((OBJS[i]->colliderPtr->Pos.x - (tileMap.planeLows.x * tileMap.mapSize.x)) - OBJS[i]->colliderPtr->GetWidth() * 100,
						(OBJS[i]->colliderPtr->Pos.z - (tileMap.planeLows.y * tileMap.mapSize.y)) - OBJS[i]->colliderPtr->GetLength() * 100,
						(OBJS[i]->colliderPtr->Pos.x - (tileMap.planeLows.x * tileMap.mapSize.x)) + OBJS[i]->colliderPtr->GetWidth() * 100,
						(OBJS[i]->colliderPtr->Pos.z - (tileMap.planeLows.y * tileMap.mapSize.y)) + OBJS[i]->colliderPtr->GetLength() * 100);

					for (int r = 0; r < tileMap.yDivisions; r++)
					{
						for (int c = 0; c < tileMap.xDivisions; c++)
						{
							if (colliderQuad.x < pointGrid[(c + (tileMap.xDivisions * r))].position.x &&
								colliderQuad.y < pointGrid[(c + (tileMap.xDivisions * r))].position.z &&
								colliderQuad.z > pointGrid[(c + (tileMap.xDivisions * r))].position.x &&
								colliderQuad.w > pointGrid[(c + (tileMap.xDivisions * r))].position.z)
							{
								pointGrid[(c + (tileMap.xDivisions * r))].walkable = false;
							}
						}
					}
				}
			}
		}
	}
}

void ADAI::ADPathfinding::CleanUpPointGrid()
{

}

void ADAI::ADPathfinding::SetNeighbors(ADAI::ADPathfinding::SearchNode* searchNode)
{
	PathingNode* pathNode = searchNode->tile;

	//above left
	SearchNode* targetDestination = GetTile(pathNode->column - 1, pathNode->row - 1);
	if (targetDestination != nullptr && searchNode->tile->directions[0])
	{
		searchNode->neighbors.push_back(targetDestination);
		searchNode->neighborDist.push_back(DistanceCalculation(pathNode, targetDestination->tile) * (targetDestination->tile->terrainWeight));
	}
	//Up
	targetDestination = GetTile(pathNode->column, pathNode->row - 1);
	if (targetDestination != nullptr && searchNode->tile->directions[1] && targetDestination->tile->walkable)
	{
		searchNode->neighbors.push_back(targetDestination);
		searchNode->neighborDist.push_back(DistanceCalculation(pathNode, targetDestination->tile) * (targetDestination->tile->terrainWeight));
	}
	//Up Right
	targetDestination = GetTile(pathNode->column + 1, pathNode->row - 1);
	if (targetDestination != nullptr && searchNode->tile->directions[2])
	{
		searchNode->neighbors.push_back(targetDestination);
		searchNode->neighborDist.push_back(DistanceCalculation(pathNode, targetDestination->tile) * (targetDestination->tile->terrainWeight));
	}
	//left
	targetDestination = GetTile(pathNode->column - 1, pathNode->row);
	if (targetDestination != nullptr && searchNode->tile->directions[3] && targetDestination->tile->walkable)
	{
		searchNode->neighbors.push_back(targetDestination);
		searchNode->neighborDist.push_back(DistanceCalculation(pathNode, targetDestination->tile) * (targetDestination->tile->terrainWeight));
	}
	//right
	targetDestination = GetTile(pathNode->column + 1, pathNode->row);
	if (targetDestination != nullptr && searchNode->tile->directions[4] && targetDestination->tile->walkable)
	{
		searchNode->neighbors.push_back(targetDestination);
		searchNode->neighborDist.push_back(DistanceCalculation(pathNode, targetDestination->tile) * (targetDestination->tile->terrainWeight));
	}
	//down left
	targetDestination = GetTile(pathNode->column - 1, pathNode->row + 1);
	if (targetDestination != nullptr && searchNode->tile->directions[5])
	{
		searchNode->neighbors.push_back(targetDestination);
		searchNode->neighborDist.push_back(DistanceCalculation(pathNode, targetDestination->tile) * (targetDestination->tile->terrainWeight));
	}
	//down
	targetDestination = GetTile(pathNode->column, pathNode->row + 1);
	if (targetDestination != nullptr && searchNode->tile->directions[6] && targetDestination->tile->walkable)
	{
		searchNode->neighbors.push_back(targetDestination);
		searchNode->neighborDist.push_back(DistanceCalculation(pathNode, targetDestination->tile) * (targetDestination->tile->terrainWeight));
	}

	//down right
	targetDestination = GetTile(pathNode->column + 1, pathNode->row + 1);
	if (targetDestination != nullptr && searchNode->tile->directions[7])
	{
		searchNode->neighbors.push_back(targetDestination);
		searchNode->neighborDist.push_back(DistanceCalculation(pathNode, targetDestination->tile) * (targetDestination->tile->terrainWeight));
	}
}

float ADAI::ADPathfinding::DistanceCalculation(PathingNode* _a)
{
	float xDist = abs(_a->position.x - target->position.x);
	float yDist = abs(_a->position.z - target->position.z);
	return sqrt((xDist * xDist) + (yDist * yDist));
}

float ADAI::ADPathfinding::DistanceCalculation(PathingNode* _a, PathingNode* _b)
{
	float xDist = abs(_a->position.x - _b->position.x);
	float yDist = abs(_a->position.z - _b->position.z);
	return sqrt((xDist * xDist) + (yDist * yDist));
}

ADAI::ADPathfinding::ADPathfinding()
{

}

ADAI::ADPathfinding::~ADPathfinding()
{
	for (auto it = searching_map.begin(), itEnd = searching_map.end(); it != itEnd; ++it)
	{
		delete it->second;
	}
	for (auto it = visited_map.begin(), itEnd = visited_map.end(); it != itEnd; ++it)
	{
		delete it->second;
	}
}

void ADAI::ADPathfinding::UpdatePlayerNode(float _x, float _z, float _mapWidth, float _mapHeight)
{
	float x = _x - (tileMap.planeLows.x * _mapWidth);
	float z = _z - (tileMap.planeLows.y * _mapHeight);
	int column = int(x/ tileMap.cellSize.x);
	int row = int(z/ tileMap.cellSize.y);

	if (tileMap.nodeGrid.size() > ((row * tileMap.columns) + column))
	{
		tileMap.nodeGrid[((row * tileMap.columns) + column)]->displayState = 1;
		for (int i = 0; i < searching_map[tileMap.nodeGrid[((row * tileMap.columns) + column)]]->neighbors.size(); i++)
		{
			searching_map[tileMap.nodeGrid[((row * tileMap.columns) + column)]]->neighbors[i]->tile->displayState = 2;
		}
	}

}

std::vector<ADAI::PathingNode*>* ADAI::ADPathfinding::GetPlaneNodes()
{
	return &tileMap.nodeGrid;
}


void ADAI::ADPathfinding::Initialize(std::vector<SimpleVertex>* _planeVertices, XMFLOAT2 _mapSize, float _agentSize, float _agentToWallGap)
{
	done = true;
	tileMap.Initializing(_planeVertices, _mapSize, _agentSize, _agentToWallGap);
	CreatePointGrid(_planeVertices);

	for (int r = 0; r < tileMap.rows; r++)
	{
		for (int c = 0; c < tileMap.columns; c++)
		{
			PathingNode* mapNode = new PathingNode();
			mapNode->position = XMFLOAT3(pointGrid[(r *  (tileMap.xDivisions)) + c].position.x + (tileMap.cellSize.x / 2.f),
										((pointGrid[(r * (tileMap.xDivisions)) + c].position.y + pointGrid[((r + 1) * (tileMap.xDivisions)) + c].position.y +
										pointGrid[(r *   (tileMap.xDivisions)) + (c + 1)].position.y + pointGrid[((r + 1) * (tileMap.xDivisions)) + (c + 1)].position.y)/4),
										pointGrid[(r *   (tileMap.xDivisions)) + c].position.z + (tileMap.cellSize.y / 2.f));
			mapNode->column = c;
			mapNode->row = r;

			for (int i = 0; i < 8; i++)
			{
				mapNode->directions[i] = true;
			}
			mapNode->terrainWeight = 1;
			int sideCount = 0;
			if (r > 0 && c > 0)
			{
				if (pointGrid[(r * (tileMap.xDivisions)) + c].walkable) //((r-1) * (dividersX)) + (c-1)].walkable)
				{ 
					mapNode->directions[0] = true;
					sideCount++; 
				}
				else
				{
					mapNode->directions[0] = false;
				}
			}
			else
			{
				mapNode->directions[0] = false;
			}

			if (r > 0 && c < tileMap.columns-1)
			{
				if (pointGrid[(r * (tileMap.xDivisions)) + (c + 1)].walkable)
				{ 
					sideCount++;
					mapNode->directions[2] = true;
				}
				else
				{
					mapNode->directions[2] = false;
				}
			}
			else
			{
				mapNode->directions[2] = false;
			}

			if (r < tileMap.rows-1 && c > 0)
			{
				if (pointGrid[((r + 1) * (tileMap.xDivisions)) + (c)].walkable)
				{ 
					mapNode->directions[5] = true;
					sideCount++; 
				}
				else
				{
					mapNode->directions[5] = false;
				}
			}
			else
			{
				mapNode->directions[5] = false;
			}

			if (r < tileMap.rows-1 && c < tileMap.columns-1)
			{
				if (pointGrid[((r + 1) * (tileMap.xDivisions)) + (c + 1)].walkable)
				{ 
					mapNode->directions[7] = true;
					sideCount++;
				}
				else
				{
					mapNode->directions[7] = false;
				}
			}
			else
			{
				mapNode->directions[7] = false;
			}


			mapNode->threeSided = (sideCount == 3);
			mapNode->walkable = (sideCount > 2);

			tileMap.nodeGrid.push_back(mapNode);
			searching_map[mapNode] = new SearchNode(mapNode);
		}
	}

	for (int r = 0; r < tileMap.rows; r++)
	{
		for (int c = 0; c < tileMap.columns; c++)
		{
			if (tileMap.nodeGrid[(r * (tileMap.columns)) + c]->threeSided)// && tileMap.nodeGrid[(r * (tileMap.xDivisions)) + c]->walkable)
			{
				if (tileMap.nodeGrid[(r * (tileMap.columns)) + c]->directions[0] == false)
				{
					if (!tileMap.nodeGrid[((r-1) * (tileMap.columns)) + c]->walkable)
					{
						tileMap.nodeGrid[(r * (tileMap.columns)) + c]->directions[1] = false;
					}
					if (!tileMap.nodeGrid[(r * (tileMap.columns)) + (c-1)]->walkable)
					{
						tileMap.nodeGrid[(r * (tileMap.columns)) + c]->directions[3] = false;
					}
				}
				else if (tileMap.nodeGrid[(r * (tileMap.columns)) + c]->directions[2] == false)
				{
					if (!tileMap.nodeGrid[((r - 1) * (tileMap.columns)) + c]->walkable)
					{
						tileMap.nodeGrid[(r * (tileMap.columns)) + c]->directions[1] = false;
					}
					if (!tileMap.nodeGrid[(r * (tileMap.columns)) + (c + 1)]->walkable)
					{
						tileMap.nodeGrid[(r * (tileMap.columns)) + c]->directions[4] = false;
					}
				}
				else if (tileMap.nodeGrid[(r * (tileMap.columns)) + c]->directions[5] == false)
				{
					if (!tileMap.nodeGrid[(r * (tileMap.columns)) + (c - 1)]->walkable)
					{
						tileMap.nodeGrid[(r * (tileMap.columns)) + c]->directions[3] = false;
					}
					if (!tileMap.nodeGrid[((r+1) * (tileMap.columns)) + c]->walkable)
					{
						tileMap.nodeGrid[(r * (tileMap.columns)) + c]->directions[6] = false;
					}
				}
				else if (tileMap.nodeGrid[(r * (tileMap.columns)) + c]->directions[7] == false)
				{
					if (!tileMap.nodeGrid[(r * (tileMap.columns)) + (c + 1)]->walkable)
					{
						tileMap.nodeGrid[(r * (tileMap.columns)) + c]->directions[4] = false;
					}
					if (!tileMap.nodeGrid[((r + 1) * (tileMap.columns)) + c]->walkable)
					{
						tileMap.nodeGrid[(r * (tileMap.columns)) + c]->directions[6] = false;
					}
				}
			}
		}
	}

	for (auto it = searching_map.begin(), itEnd = searching_map.end(); it != itEnd; ++it)
	{
		SetNeighbors(it->second);
	}
}


void ADAI::ADPathfinding::enter(int startColumn, int startRow, int goalColumn, int goalRow)
{
	//Setup the search process
	ClearDebug();
	done = false;
	solution.clear();
	visited_map.clear();
	pHeap.clear();
	target = GetTile(goalColumn, goalRow)->tile;
	PlannerNode* first = new PlannerNode();
	first->parent = NULL;
	first->searchNode = GetTile(startColumn, startRow);
	first->givenCost = 0;
	first->heuristicCost = DistanceCalculation(first->searchNode->tile);
	first->finalCost = first->givenCost + first->heuristicCost * hWeight;
	//pQueue.(first);
	std::make_heap(pHeap.begin(), pHeap.end(), CompareCost());
	pHeap.push_back(first);
	visited_map[GetTile(startColumn, startRow)] = first;
}

void ADAI::ADPathfinding::update(long timeslice)
{
	do
	{
		if (pHeap.size() == 0)
		{
			done = true;
			return;
		}
		//Do search for xtime then stop and let thread continue
		current = pHeap.front();
		std::pop_heap(pHeap.begin(), pHeap.end(), CompareCost()); pHeap.pop_back();
		//current = pQueue.top();
		//pQueue.pop();

		current->searchNode->tile->displayState = 1;
		if (current->searchNode->tile == target)
		{
			solution.clear();
			retracer = current;
			while (retracer->parent != NULL)
			{
				solution.push_back(retracer->searchNode->tile);
				retracer->searchNode->tile->displayState = 2;
				retracer = retracer->parent;
			}
			solution.push_back(retracer->searchNode->tile);

			done = true;
			return;
		}

		//if (timeslice == 0)
		//{
		//	previousTrace.clear();
		//	//retracer = current;
		//	//while (retracer->parent != NULL)
		//	//{
		//	//	previousTrace.push_back(retracer->searchNode->tile);
		//	//	retracer = retracer->parent;
		//	//}
		//	previousTrace.push_back(retracer->searchNode->tile);
		//}

		int neighborIndex = 0;
		auto end = current->searchNode->neighbors.end();
		for (std::vector<SearchNode*>::iterator i = current->searchNode->neighbors.begin(); i != end; ++i)
		{
			float tempGivenCost = current->givenCost + current->searchNode->neighborDist[neighborIndex];
			neighborIndex++;
			if (visited_map[*i] != NULL)
			{
				PlannerNode* node = visited_map[*i];
				if (tempGivenCost < node->givenCost)
				{
					//pHeap.erase();
					//pQueue.remove[node];

					node->givenCost = tempGivenCost;
					node->finalCost = node->givenCost + node->heuristicCost * hWeight;
					node->parent = current;
					std::make_heap(pHeap.begin(), pHeap.end(), CompareCost());
					//std::sort_heap(pHeap.begin(), pHeap.end(), CompareCost());
					//pQueue.push(node);
					//pHeap.push_back(node);
				}
			}
			else
			{
				PlannerNode* node = new PlannerNode();
				node->searchNode = *i;
				node->parent = current;
				node->givenCost = tempGivenCost;
				node->heuristicCost = DistanceCalculation(node->searchNode->tile);
				node->finalCost = node->givenCost + node->heuristicCost * hWeight;
				visited_map[*i] = node;
				node->searchNode->tile->displayState = 1;
				//pQueue.push(node);
				pHeap.push_back(node);
			}
		}
	} while (timeslice > 0);

}

void ADAI::ADPathfinding::exit()
{
	if (visited_map.size() > 0)
	{
		for (int x = 0; x < tileMap.columns; ++x)
		{
			for (int y = 0; y < tileMap.rows; ++y)
			{
				SearchNode* tl = GetTile(x, y);
				if(visited_map[tl] != nullptr)
					delete visited_map[tl];
			}
		}
	}
	visited_map.clear();
	pHeap.clear();
	//while (pHeap.size() > 0)
	//{
	//	std::pop_heap(pHeap.begin(), pHeap.end()); pHeap.pop_back();
	//}
	//while (pQueue.size() > 0)
	//{
	//	pQueue.pop();
	//}

	solution.clear();
	previousTrace.clear();
}

void ADAI::ADPathfinding::shutdown()
{
	int skipped = 0;
	if (tileMap.nodeGrid.size() > 0)
	{
		for (auto it = searching_map.begin(), itEnd = searching_map.end(); it != itEnd; ++it)
		{
			delete it->second;
		}
	}

	tileMap.nodeGrid.clear();
	pHeap.clear();
	//while (pHeap.size() > 0)
	//{
	//	std::pop_heap(pHeap.begin(), pHeap.end()); pHeap.pop_back();
	//}
	//while (pQueue.size() > 0)
	//{
	//	pQueue.pop();
	//}
	solution.clear();
	previousTrace.clear();
	done = false;
}

bool ADAI::ADPathfinding::isDone() const
{
	return done;
}

std::vector<const ADAI::PathingNode*> const ADAI::ADPathfinding::getSolution() const
{
	return solution;
}