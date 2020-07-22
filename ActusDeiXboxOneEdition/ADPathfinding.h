#pragma once
#include "pch.h"
#include "Types.h"
#include "ResourceManager.h"
#include "ADPhysics.h"

#include <unordered_map>
#include <set>


namespace ADAI
{

	struct PathingNode
	{
		XMFLOAT3 position;
		int column;
		int row;

		bool walkable;
		long displayState;
		unsigned char terrainWeight;
		bool threeSided;
		bool directions[8];
		float finalCost;
	};

	struct PathingGrid
	{
		XMFLOAT2 mapSize;
		XMFLOAT2 cellSize;
		UINT columns;
		UINT rows;
		UINT xDivisions;
		UINT yDivisions;
		float agentToWallGap;
		std::vector<PathingNode*> nodeGrid;

		void Initializing(std::vector<SimpleVertex>* _planeVertices, XMFLOAT2 _mapSize, float _agentSize, float _agentToWallGap)
		{
			//float xhigh = 0;
			//float zhigh = 0;
			//for (int i = 0; i < _planeVertices->size(); i++)
			//{
			//	float x = (*_planeVertices)[i].Position.x;
			//	float z = (*_planeVertices)[i].Position.z;
			//	if (x < planeLows.x)
			//	{
			//		planeLows.x = x;
			//	}
			//	if (x > xhigh)
			//	{
			//		xhigh = x;
			//	}
			//	if (z < planeLows.y)
			//	{
			//		planeLows.y = z;
			//	}
			//	if (z > zhigh)
			//	{
			//		zhigh = z;
			//	}
			//}
			//float xdelta = xhigh - planeLows.x;
			//float zdelta = zhigh - planeLows.y;
			//mapSize.x = xdelta * _mapSize.x;
			//mapSize.y = zdelta * _mapSize.y;
			//cellSize.x = (_agentSize + _agentToWallGap);
			//cellSize.y = (_agentSize + _agentToWallGap);
			agentToWallGap = _agentToWallGap;
			mapSize = _mapSize;
			cellSize = { _agentSize, _agentSize };
			columns = (mapSize.x / cellSize.x);
			rows = (mapSize.y / cellSize.y);
			xDivisions = columns + 1;
			yDivisions = rows + 1;
		}

		void GetColumnRowFromPosition(XMFLOAT2 _position, UINT& _out_column, UINT& _out_row)
		{
			_out_column = (_position.x + (mapSize.x / 2.f)) / cellSize.x;
			_out_row = (_position.y + (mapSize.y / 2.f)) / cellSize.y;
			//float x = _position.x - (planeLows.x * mapSize.x);
			//float z = _position.y - (planeLows.y * mapSize.y);
			//_out_column = int(x / cellSize.x);
			//_out_row = int(z / cellSize.y);
		};
	};

	struct SearchNode
	{
		SearchNode(PathingNode* _tile) : tile(_tile) {};
		PathingNode* tile;
		std::vector<SearchNode*> neighbors;
		std::vector<float> neighborDist;
	};

	struct Solution
	{
		std::vector<XMFLOAT3> positions;
		std::vector<float> totalLength;
	};


	class ADPathfinding
	{
	private:

		ADPathfinding() {};
		~ADPathfinding() { shutdown(); };
		ADPathfinding(const ADPathfinding& _rhs) {};
		ADPathfinding& operator =(const ADPathfinding& _rhs) {};

		// Timing
		XTime throttle_time;

		struct PathingPoint
		{
			XMFLOAT3 position;
			bool walkable;
		};

		struct PlannerNode
		{
			SearchNode* searchNode;
			PlannerNode* parent;

			float heuristicCost;
			float givenCost;
			float finalCost;
		};

		class CompareCost
		{
		public:
			bool operator() (PlannerNode* _a, PlannerNode* _b)
			{
				return (_a->finalCost > _b->finalCost);
			};
		};
		std::vector<ADResource::ADGameplay::GameObject*>* extraCollidables;

		std::vector<PathingPoint> pointGrid;

		std::unordered_map<PathingNode*, SearchNode*> searching_map;
		std::unordered_map<SearchNode*, PlannerNode*> visited_map;
		//std::vector<PathingNode const*> solution;
		Solution solution;
		std::vector<PathingNode*> previousTrace;
		PlannerNode* current;
		PlannerNode* retracer;

		std::vector<PlannerNode*> pHeap;

		PathingNode* target;
		UINT hWeight = 1.0f;

		void CreatePointGrid(std::vector<SimpleVertex>* _planeVertices);
		void CleanUpPointGrid();
		void SetNeighbors(SearchNode* searchNode);
		float DistanceCalculation(PathingNode* _a);
		float DistanceCalculation(PathingNode* _a, PathingNode* _b);
		bool done = false;
		float xAdjust; 
		float zAdjust; 

	public:
		static ADPathfinding* Instance();
		PathingGrid tileMap;
		void Initialize(std::vector<SimpleVertex>* _planeVertices, XMFLOAT2 _mapSize, float _agentSize, float _agentToWallGap, std::vector<ADResource::ADGameplay::GameObject*>* _extraCollidables);
		int findAcceptablePoint(UINT& goalColumn, UINT& goalRow);
		int enter(int startColumn, int startRow, int goalColumn, int goalRow);
		bool isDone() const;
		void update(float timeslice);
		//std::vector<PathingNode const*> const getSolution() const;
		Solution getSolutionPoints() const;
		void exit();
		void shutdown();
		void UpdatePlayerNode(float x, float z, float mapWidth, float mapHeight);
		std::vector<PathingNode*>* GetPlaneNodes();
		void EnableTile(SearchNode* tile);

		void ClearDebug()
		{
			for (auto it = searching_map.begin(), itEnd = searching_map.end(); it != itEnd; ++it)
			{
				if (it->first->walkable)
					it->first->displayState = 0;
				else
					it->first->displayState = 4;
			}
		}

		SearchNode* GetTile(int _column = 0, int _row = 0)
		{
			if (_column < 0 || _column >= tileMap.columns || _row < 0 || _row >= tileMap.rows)
			{
				return nullptr;
			}
			return searching_map[tileMap.nodeGrid[(_row * tileMap.columns) + _column]];
		};

		SearchNode* GetTileFromPosition(XMFLOAT2 _position)
		{
			UINT tcolumn = 0;
			UINT trow = 0;
			tileMap.GetColumnRowFromPosition(_position, tcolumn, trow);
			if (tcolumn < 0 || tcolumn >= tileMap.columns || trow < 0 || trow >= tileMap.rows)
			{
				return nullptr;
			}
			return searching_map[tileMap.nodeGrid[(trow * tileMap.columns) + tcolumn]];
		};

		XMFLOAT3 AdjustPosition(XMFLOAT3 _position)
		{
			return { _position.x - xAdjust, 0, _position.z - zAdjust };

		}
	};
}

