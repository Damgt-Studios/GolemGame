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
		//UINT pointColumn = (((*_planeVertices)[i].Position.x - (tileMap.mapSize.x / 2.f)) * tileMap.mapSize.x) / tileMap.cellSize.x;
		//UINT pointRow = (((*_planeVertices)[i].Position.z - tileMap.planeLows.y) * tileMap.mapSize.y) / tileMap.cellSize.y;
		UINT pointColumn = ((*_planeVertices)[i].Position.x + (tileMap.mapSize.x / 2.f)) / tileMap.cellSize.x;
		UINT pointRow = ((*_planeVertices)[i].Position.y + (tileMap.mapSize.y / 2.f)) / tileMap.cellSize.y;
		if (pointColumn < tileMap.xDivisions && pointRow < tileMap.yDivisions)
		{
			if (std::fmodf((((*_planeVertices)[i].Position.x + (tileMap.mapSize.x / 2)) * tileMap.mapSize.x), tileMap.cellSize.x) > (tileMap.cellSize.x / 2.f))
			{
				if (pointColumn < tileMap.columns)
					++pointColumn;
			}

			if (std::fmodf((((*_planeVertices)[i].Position.z + (tileMap.mapSize.y / 2)) * tileMap.mapSize.y), tileMap.cellSize.y) > (tileMap.cellSize.y / 2.f))
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
				if (OBJS[i]->colliderPtr->type == ADPhysics::ColliderType::Aabb)
				{
					XMFLOAT4 colliderQuad(((OBJS[i]->colliderPtr->Pos.x + tileMap.mapSize.x / 2.f) - OBJS[i]->colliderPtr->GetWidth() + tileMap.agentToWallGap / 2.f),
						((OBJS[i]->colliderPtr->Pos.z + tileMap.mapSize.y / 2.f) - (OBJS[i]->colliderPtr->GetLength() + tileMap.agentToWallGap) / 2.f),
						((OBJS[i]->colliderPtr->Pos.x + tileMap.mapSize.x / 2.f) + (OBJS[i]->colliderPtr->GetWidth() + tileMap.agentToWallGap) / 2.f),
						((OBJS[i]->colliderPtr->Pos.z + tileMap.mapSize.y / 2.f) + (OBJS[i]->colliderPtr->GetLength() + tileMap.agentToWallGap) / 2.f));
					
					for (int r = 0; r < tileMap.yDivisions; r++)
					{
						for (int c = 0; c < tileMap.xDivisions; c++)
						{
							XMFLOAT3 point = pointGrid[(c + (tileMap.xDivisions * r))].position;
							if (colliderQuad.x < point.x &&
								colliderQuad.y < point.z &&
								colliderQuad.z > point.x &&
								colliderQuad.w > point.z)
							{
								pointGrid[(c + (tileMap.xDivisions * r))].walkable = false;
							}


							//XMFLOAT3 Difference;
							//Difference = XMFLOAT3(OBJS[i]->colliderPtr->Pos.x - point.x, OBJS[i]->colliderPtr->Pos.y - point.y, OBJS[i]->colliderPtr->Pos.z - point.z);

							//(fabs(VectorDot(Difference, Axis)) >
							//	(fabs(VectorDot((Float3Multiply(OBJS[i]->colliderPtr->AxisX, box1.HalfSize.x)), Axis)) +
							//		fabs(VectorDot((Float3Multiply(box1.AxisY, box1.HalfSize.y)), Axis)) +
							//		fabs(VectorDot((Float3Multiply(box1.AxisZ, box1.HalfSize.z)), Axis)) +
							//		fabs(VectorDot(box1.AxisX, Axis)) +
							//		fabs(VectorDot(box1.AxisY, Axis)) +
							//		fabs(VectorDot(box1.AxisZ, Axis))));



							//XMFLOAT3 rot = OBJS[i]->GetRotationDegrees();
							////XMStoreFloat3(&rot, );
							//float newy = std::sin(XMConvertToRadians(rot.y)) * (point.z - OBJS[i]->colliderPtr->Pos.z) + std::cos(XMConvertToRadians(rot.y)) * (point.x - OBJS[i]->colliderPtr->Pos.x);
							//float newx = std::cos(XMConvertToRadians(rot.y)) * (point.x - OBJS[i]->colliderPtr->Pos.x) - std::sin(XMConvertToRadians(rot.y)) * (point.z - OBJS[i]->colliderPtr->Pos.z);

							//if (colliderQuad.x < newx &&
							//	colliderQuad.y < newy &&
							//	colliderQuad.z > newx &&
							//	colliderQuad.w > newy)
							//{
							//	pointGrid[(c + (tileMap.xDivisions * r))].walkable = false;
							//}		

					
						}
					}
				}
				else if(OBJS[i]->colliderPtr->type == ADPhysics::ColliderType::Obb)
				{
					ADPhysics::OBB* box1 = dynamic_cast<ADPhysics::OBB*>(OBJS[i]->colliderPtr);
					if (box1)
					{
						XMFLOAT3 colliderVec((OBJS[i]->colliderPtr->Pos.x + tileMap.mapSize.x / 2.f),
							(0),
							((OBJS[i]->colliderPtr->Pos.z + tileMap.mapSize.y / 2.f)));

						for (int r = 0; r < tileMap.yDivisions; r++)
						{
							for (int c = 0; c < tileMap.xDivisions; c++)
							{
								XMFLOAT3 point = pointGrid[(c + (tileMap.xDivisions * r))].position; 
								XMVECTOR diff = XMLoadFloat3(&point) - XMLoadFloat3(&colliderVec);
								if (std::fabs(VectorDot(diff, XMLoadFloat3(&box1->AxisX))) <= box1->HalfSize.x &&
									std::fabs(VectorDot(diff, XMLoadFloat3(&box1->AxisZ))) <= box1->HalfSize.z)
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

	for (int i = 0; i < extraCollidables->size(); i++)
	{
		if ((*extraCollidables)[i]->colliderPtr != nullptr)
		{
			if ((*extraCollidables)[i]->physicsType == ADResource::ADGameplay::STATIC)
			{
				if ((*extraCollidables)[i]->colliderPtr->type != ADPhysics::ColliderType::Plane)
				{
					XMFLOAT4 colliderQuad((((*extraCollidables)[i]->colliderPtr->Pos.x + tileMap.mapSize.x / 2.f) - (*extraCollidables)[i]->colliderPtr->GetWidth() + tileMap.agentToWallGap / 2.f),
						(((*extraCollidables)[i]->colliderPtr->Pos.z + tileMap.mapSize.y / 2.f) - ((*extraCollidables)[i]->colliderPtr->GetLength() + tileMap.agentToWallGap) / 2.f),
						(((*extraCollidables)[i]->colliderPtr->Pos.x + tileMap.mapSize.x / 2.f) + ((*extraCollidables)[i]->colliderPtr->GetWidth() + tileMap.agentToWallGap) / 2.f),
						(((*extraCollidables)[i]->colliderPtr->Pos.z + tileMap.mapSize.y / 2.f) + ((*extraCollidables)[i]->colliderPtr->GetLength() + tileMap.agentToWallGap) / 2.f));

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
	pointGrid.clear();
}

void ADAI::ADPathfinding::SetNeighbors(ADAI::SearchNode* searchNode)
{
	PathingNode* pathNode = searchNode->tile;

	//above left
	SearchNode* targetDestination = GetTile(pathNode->column - 1, pathNode->row - 1);
	if (targetDestination != nullptr && searchNode->tile->directions[0])
	{
		if (targetDestination->tile->walkable)
		{
			searchNode->neighbors.push_back(targetDestination);
			searchNode->neighborDist.push_back(DistanceCalculation(pathNode, targetDestination->tile) * (targetDestination->tile->terrainWeight));
		}
	}
	//Up
	targetDestination = GetTile(pathNode->column, pathNode->row - 1);
	if (targetDestination != nullptr && searchNode->tile->directions[1] && targetDestination->tile->walkable)
	{
		if (targetDestination->tile->walkable)
		{
			searchNode->neighbors.push_back(targetDestination);
			searchNode->neighborDist.push_back(DistanceCalculation(pathNode, targetDestination->tile) * (targetDestination->tile->terrainWeight));
		}
	}
	//Up Right
	targetDestination = GetTile(pathNode->column + 1, pathNode->row - 1);
	if (targetDestination != nullptr && searchNode->tile->directions[2])
	{
		if (targetDestination->tile->walkable)
		{
			searchNode->neighbors.push_back(targetDestination);
			searchNode->neighborDist.push_back(DistanceCalculation(pathNode, targetDestination->tile) * (targetDestination->tile->terrainWeight));
		}
	}
	//left
	targetDestination = GetTile(pathNode->column - 1, pathNode->row);
	if (targetDestination != nullptr && searchNode->tile->directions[3] && targetDestination->tile->walkable)
	{
		if (targetDestination->tile->walkable)
		{
			searchNode->neighbors.push_back(targetDestination);
			searchNode->neighborDist.push_back(DistanceCalculation(pathNode, targetDestination->tile) * (targetDestination->tile->terrainWeight));
		}
	}
	//right
	targetDestination = GetTile(pathNode->column + 1, pathNode->row);
	if (targetDestination != nullptr && searchNode->tile->directions[4] && targetDestination->tile->walkable)
	{
		if (targetDestination->tile->walkable)
		{
			searchNode->neighbors.push_back(targetDestination);
			searchNode->neighborDist.push_back(DistanceCalculation(pathNode, targetDestination->tile) * (targetDestination->tile->terrainWeight));
		}
	}
	//down left
	targetDestination = GetTile(pathNode->column - 1, pathNode->row + 1);
	if (targetDestination != nullptr && searchNode->tile->directions[5])
	{
		if (targetDestination->tile->walkable)
		{
			searchNode->neighbors.push_back(targetDestination);
			searchNode->neighborDist.push_back(DistanceCalculation(pathNode, targetDestination->tile) * (targetDestination->tile->terrainWeight));
		}
	}
	//down
	targetDestination = GetTile(pathNode->column, pathNode->row + 1);
	if (targetDestination != nullptr && searchNode->tile->directions[6] && targetDestination->tile->walkable)
	{
		if (targetDestination->tile->walkable)
		{
			searchNode->neighbors.push_back(targetDestination);
			searchNode->neighborDist.push_back(DistanceCalculation(pathNode, targetDestination->tile) * (targetDestination->tile->terrainWeight));
		}
	}

	//down right
	targetDestination = GetTile(pathNode->column + 1, pathNode->row + 1);
	if (targetDestination != nullptr && searchNode->tile->directions[7])
	{
		if (targetDestination->tile->walkable)
		{
			searchNode->neighbors.push_back(targetDestination);
			searchNode->neighborDist.push_back(DistanceCalculation(pathNode, targetDestination->tile) * (targetDestination->tile->terrainWeight));
		}
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


void ADAI::ADPathfinding::UpdatePlayerNode(float _x, float _z, float _mapWidth, float _mapHeight)
{
	//float x = _x + ((tileMap.mapSize.x / 2) * _mapWidth);
	//float z = _z + ((tileMap.mapSize.y / 2) * _mapHeight);
	//int column = int(x/ tileMap.cellSize.x);
	//int row = int(z/ tileMap.cellSize.y);

	UINT column = (_x + (tileMap.mapSize.x / 2.f)) / tileMap.cellSize.x;
	UINT row = (_z + (tileMap.mapSize.y / 2.f)) / tileMap.cellSize.y;

	if (tileMap.nodeGrid.size() > ((row * tileMap.columns) + column))
	{
		tileMap.nodeGrid[((row * tileMap.columns) + column)]->displayState = 2;
		for (int i = 0; i < searching_map[tileMap.nodeGrid[((row * tileMap.columns) + column)]]->neighbors.size(); i++)
		{
			searching_map[tileMap.nodeGrid[((row * tileMap.columns) + column)]]->neighbors[i]->tile->displayState = 3;
		}
	}

}

std::vector<ADAI::PathingNode*>* ADAI::ADPathfinding::GetPlaneNodes()
{
	return &tileMap.nodeGrid;
}


void ADAI::ADPathfinding::EnableTile(SearchNode* _tile)
{
	//_tile->tile->walkable = true;
	SetNeighbors(_tile);
	for (auto& neighbor : _tile->neighbors)
	{
		neighbor->neighbors.push_back(_tile);
		neighbor->neighborDist.push_back(DistanceCalculation(neighbor->tile, _tile->tile) * (_tile->tile->terrainWeight));
	}
}


ADAI::ADPathfinding* ADAI::ADPathfinding::Instance()
{
	static ADPathfinding instance;
	return &instance;
}


void ADAI::ADPathfinding::Initialize(std::vector<SimpleVertex>* _planeVertices, XMFLOAT2 _mapSize, float _agentSize, float _agentToWallGap, std::vector<ADResource::ADGameplay::GameObject*>* _extraCollidables)
{

	extraCollidables = _extraCollidables;
	done = true;
	tileMap.Initializing(_planeVertices, _mapSize, _agentSize, _agentToWallGap);
	CreatePointGrid(_planeVertices);

	for (int r = 0; r < tileMap.rows; r++)
	{
		for (int c = 0; c < tileMap.columns; c++)
		{
			PathingNode* mapNode = new PathingNode();
			mapNode->position = XMFLOAT3(pointGrid[(r * (tileMap.xDivisions)) + c].position.x + (tileMap.cellSize.x / 2.f),
				((pointGrid[(r * (tileMap.xDivisions)) + c].position.y + pointGrid[((r + 1) * (tileMap.xDivisions)) + c].position.y +
					pointGrid[(r * (tileMap.xDivisions)) + (c + 1)].position.y + pointGrid[((r + 1) * (tileMap.xDivisions)) + (c + 1)].position.y) / 4),
				pointGrid[(r * (tileMap.xDivisions)) + c].position.z + (tileMap.cellSize.y / 2.f));
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

			if (r > 0 && c < tileMap.columns - 1)
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

			if (r < tileMap.rows - 1 && c > 0)
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

			if (r < tileMap.rows - 1 && c < tileMap.columns - 1)
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
					if (!tileMap.nodeGrid[((r - 1) * (tileMap.columns)) + c]->walkable)
					{
						tileMap.nodeGrid[(r * (tileMap.columns)) + c]->directions[1] = false;
					}
					if (!tileMap.nodeGrid[(r * (tileMap.columns)) + (c - 1)]->walkable)
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
					if (!tileMap.nodeGrid[((r + 1) * (tileMap.columns)) + c]->walkable)
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
	CleanUpPointGrid();

	xAdjust = (tileMap.columns / 2.f) * tileMap.cellSize.x;
	zAdjust = (tileMap.rows / 2.f) * tileMap.cellSize.y;
}


//return 0 on succcess, 1 on out of bounds, 2 on no valid neighbors
int ADAI::ADPathfinding::findAcceptablePoint(UINT& goalColumn, UINT& goalRow)
{
	//Order mattered so I didn't loop them.
	SearchNode* temp = GetTile(goalColumn, goalRow);
	if (temp)
	{
		if (temp->tile->walkable)
		{
			return 0;
		}
	}
	temp = GetTile(goalColumn -1, goalRow);
	if (temp)
	{
		if (temp->tile->walkable)
		{
			--goalColumn;
			return 0;
		}
	}
	temp = GetTile(goalColumn + 1, goalRow);
	if (temp)
	{
		if (temp->tile->walkable)
		{
			++goalColumn;
			return 0;
		}
	}
	temp = GetTile(goalColumn , goalRow-1);
	if (temp)
	{
		if (temp->tile->walkable)
		{
			--goalRow;
			return 0;
		}
	}
	temp = GetTile(goalColumn, goalRow + 1);
	if (temp)
	{
		if (temp->tile->walkable)
		{
			++goalRow;
			return 0;
		}
	}
	temp = GetTile(goalColumn-1 , goalRow-1);
	if (temp)
	{
		if (temp->tile->walkable)
		{
			--goalColumn;
			--goalRow;
			return 0;
		}
	}
	temp = GetTile(goalColumn+1, goalRow + 1);
	if (temp)
	{
		if (temp->tile->walkable)
		{
			++goalColumn;
			++goalRow;
			return 0;
		}
	}
	temp = GetTile(goalColumn-1, goalRow + 1);
	if (temp)
	{
		if (temp->tile->walkable)
		{
			--goalColumn;
			++goalRow;
			return 0;
		}
	}
	temp = GetTile(goalColumn+1, goalRow - 1);
	if (temp)
	{
		if (temp->tile->walkable)
		{
			++goalColumn;
			--goalRow;
			return 0;
		}
	}
			//int c = -1;
			//int r = -1;
			//int c2 = 0;
			//int r2 = 0;

			//do
			//{
			//	if (c != 0 && r != 0)
			//	{
			//		temp = GetTile(goalColumn + c, goalRow + r);
			//	}

			//	if (temp)
			//	{
			//		if (temp->tile->walkable)
			//		{
			//			goalColumn = goalColumn + c;
			//			goalRow = goalRow + r;
			//			return 0;
			//		}
			//	}

			//		++c;
			//		if (r == 2)
			//		{
			//			temp = GetTile(goalColumn -2, goalRow);

			//			if (temp)
			//			{
			//				if (temp->tile->walkable)
			//				{
			//					goalColumn = goalColumn -2;
			//					goalRow = goalRow ;
			//					return 0;
			//				}
			//			}

			//			temp = GetTile(goalColumn + 2, goalRow);

			//			if (temp)
			//			{
			//				if (temp->tile->walkable)
			//				{
			//					goalColumn = goalColumn + 2;
			//					goalRow = goalRow;
			//					return 0;
			//				}
			//			}

			//			temp = GetTile(goalColumn, goalRow + 2);

			//			if (temp)
			//			{
			//				if (temp->tile->walkable)
			//				{
			//					goalColumn = goalColumn;
			//					goalRow = goalRow + 2;
			//					return 0;
			//				}
			//			}
			//			temp = GetTile(goalColumn, goalRow - 2);

			//			if (temp)
			//			{
			//				if (temp->tile->walkable)
			//				{
			//					goalColumn = goalColumn;
			//					goalRow = goalRow - 2;
			//					return 0;
			//				}
			//			}
			//		}
			//		else if (c == 2)
			//		{
			//			++r;
			//			c = -1;
			//		}
			//	
			//} while (r != 2);
		//}
	//}
	else
	{
		return 1;
	}
	return 2;
}


int ADAI::ADPathfinding::enter(int startColumn, int startRow, int goalColumn, int goalRow)
{
	//Setup the search process
	SearchNode* start = GetTile(startColumn, startRow);
	if (start)
	{
		if (!start->tile->walkable)
		{
			return 1;
		}
	}
	else
	{
		return 1;
	}

	SearchNode* targetNode = GetTile(goalColumn, goalRow);
	if (targetNode)
	{
		if (!targetNode->tile->walkable)
		{
			return 2;
		}
		else 
		{
			target = targetNode->tile;
		}
	}
	else
	{
		return 2;
	}

	//}
	ClearDebug();
	done = false;
	solution.positions.clear();
	solution.totalLength.clear();
	visited_map.clear();
	pHeap.clear();
	PlannerNode* first = new PlannerNode();
	first->parent = NULL;
	first->searchNode = start;
	first->givenCost = 0;
	first->heuristicCost = DistanceCalculation(first->searchNode->tile);
	first->finalCost = first->givenCost + first->heuristicCost * hWeight;
	std::make_heap(pHeap.begin(), pHeap.end(), CompareCost());
	pHeap.push_back(first);
	visited_map[GetTile(startColumn, startRow)] = first;
	return 0;
}

void ADAI::ADPathfinding::update(float timeslice)
{
	throttle_time.Restart();
	do
	{
		throttle_time.Signal();
		timeslice -= static_cast<float>(throttle_time.SmoothDelta());

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
			solution.positions.clear();
			solution.totalLength.clear();
			retracer = current;
			while (retracer->parent != NULL)
			{
				solution.positions.push_back(XMFLOAT3(retracer->searchNode->tile->position.x - xAdjust, 0, retracer->searchNode->tile->position.z - zAdjust));
				solution.totalLength.push_back(retracer->finalCost);
				retracer->searchNode->tile->displayState = 2;
				retracer = retracer->parent;
			}
			solution.positions.push_back(XMFLOAT3(retracer->searchNode->tile->position.x - xAdjust, 0, retracer->searchNode->tile->position.z - zAdjust));
			solution.totalLength.push_back(retracer->finalCost);

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
				if (visited_map[tl] != nullptr)
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

	solution.positions.clear();
	solution.totalLength.clear();
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
		for (auto it = visited_map.begin(), itEnd = visited_map.end(); it != itEnd; ++it)
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
	solution.positions.clear();
	solution.totalLength.clear();
	previousTrace.clear();
	done = false;
}

bool ADAI::ADPathfinding::isDone() const
{
	return done;
}
//
//std::vector<const ADAI::PathingNode*> const ADAI::ADPathfinding::getSolution() const
//{
//	return solution;
//}

ADAI::Solution ADAI::ADPathfinding::getSolutionPoints() const
{
	return solution;
	//Solution points;
	//for (auto& solPoint : solution.positions)
	//{
	//	points.positions.push_back(XMFLOAT3(solPoint.x - xAdjust, 0, solPoint.z - zAdjust)); 
	//	points.totalLength.push_back(solPoint->);
	//}
	//return points;
}