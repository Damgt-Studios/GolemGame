#pragma once
#include <vector>
//The point variable we will be using
template <typename T>
struct ADQuadTreePoint 
{
	int x, y;
	T* data = nullptr;


	//Current Idea for the making of a point is to find the centroid of the triangle \
	and set the objects X to this point's x, and the objects Z to this point's Y \
	We will be thinking of this as a top down view so the objects Y will be irrelevant \
	unless this was an Octree.
	ADQuadTreePoint(int x, int y, T& t) : x(x), y(y), data(&t) {};
};

//Creates a Quad variable that will be the bounding box of the Quad tree
struct ADQuad
{
	int x, y;
	float width, height;

	ADQuad(int x, int y, float w, float h) :
		x(x), y(y), width(w), height(h) {};

	template <typename T>
	bool ContainsPoint(ADQuadTreePoint<T> point) 
	{
		return (x - width < point.x &&
			x + width > point.x &&
			y - height < point.y &&
			y + height > point.y);
	};

	bool Intersects(ADQuad other) 
	{
		return !(other.x - other.width > x + width ||
			other.x + other.width < x - width ||
			other.y - other.height > y + height ||
			other.y + other.height < y - height);
	};
};

template <typename T>
class QuadTree 
{
public:
	QuadTree(QuadTree&) = delete;
	QuadTree(const QuadTree&) = delete;
	QuadTree(ADQuad quad) : boundary(quad) {};

private:
	const int capacity = 3;
	bool divided = false;
	ADQuad boundary;
	std::vector<ADQuadTreePoint<T>> points;

	//Children
	QuadTree* NorthEast = nullptr;
	QuadTree* NorthWest = nullptr;
	QuadTree* SouthEast = nullptr;
	QuadTree* SouthWest = nullptr;

public:
	//Uses recursion to determine which QuadTree to insert the point into
	bool Insert(ADQuadTreePoint<T> point) 
	{
		//See if the point would be inside this box
		//If not leave the function because it doesn't belong here
		if (!boundary.ContainsPoint(point))
			return false;
		
		//If the size of the array is less than the capacity (can be any number) \
		and has already not been divided, then add it to this block and return
		if (points.size() < capacity && divided == false) {
			points.push_back(point); return true;
		}

		//Getting here means that either the capacity of this object is full \
		or the object has already been divided.
		//If it isn't divided we want to split it up and if it isn't divided \
		we need to send it to one of the other boxes
		if (divided == false) {
			Subdivide();
		}

		//Repeats the process until returns true or something is broken
		if (NorthEast->Insert(point)) return true;
		if (NorthWest->Insert(point)) return true;
		if (SouthWest->Insert(point)) return true;
		if (SouthEast->Insert(point)) return true;

		return false;

	};
	
	//Uses recusion to determine all the points within the range given
	std::vector<ADQuadTreePoint<T>> Query(ADQuad range) 
	{
		std::vector<ADQuadTreePoint<T>> pointsInRange;

		//If it does not intersect then return an empty point list;
		if (!boundary.Intersects(range))
			return pointsInRange;

		//Go through all the points in the current quad and check to see if they are within the range
		for (unsigned int i = 0; i < points.size(); i++)
		{
			//If they are within the range push back into the vector
			if (range.ContainsPoint(points[i]))
				pointsInRange.push_back(points[i]);
		}

		//If the current object has not been divided then return the points in the range
		if (divided == false)
			return pointsInRange;

		std::vector<ADQuadTreePoint<T>> temp = NorthEast->Query(range);
		pointsInRange.insert(pointsInRange.end(), temp.begin(), temp.end());
		temp.clear();
		temp = NorthWest->Query(range);
		pointsInRange.insert(pointsInRange.end(), temp.begin(), temp.end());
		temp.clear();
		temp = SouthWest->Query(range);
		pointsInRange.insert(pointsInRange.end(), temp.begin(), temp.end());
		temp.clear();
		temp = SouthEast->Query(range);
		pointsInRange.insert(pointsInRange.end(), temp.begin(), temp.end());

		return pointsInRange;
	};

	//Subdivideds the quadtree into smaller quadtrees once capacity has been surpassed
	void Subdivide() 
	{
		int x = boundary.x, y = boundary.y;
		float w = boundary.width, h = boundary.height;

		//Created new quadtrees so this can be used recursively
		ADQuad ne = ADQuad(x + w / 2.0f, y - h / 2.0f, w / 2.0f, h / 2.0f);
		NorthEast = new QuadTree(ne);
		ADQuad nw = ADQuad(x - w / 2.0f, y - h / 2.0f, w / 2.0f, h / 2.0f);
		NorthWest = new QuadTree(nw);
		ADQuad sw = ADQuad(x - w / 2.0f, y + h / 2.0f, w / 2.0f, h / 2.0f);
		SouthWest = new QuadTree(sw);
		ADQuad se = ADQuad(x + w / 2.0f, y + h / 2.0f, w / 2.0f, h / 2.0f);
		SouthEast = new QuadTree(se);
		this->divided = true;
	};

	void Shutdown()
	{
		if (divided)
		{
			NorthEast->Shutdown();
			NorthWest->Shutdown();
			SouthWest->Shutdown();
			SouthEast->Shutdown();
		}

		delete this;
	}
};