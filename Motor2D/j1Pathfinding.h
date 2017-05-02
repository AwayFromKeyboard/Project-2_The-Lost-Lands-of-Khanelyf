#ifndef __j1PATHFINDING_H__
#define __j1PATHFINDING_H__

#include "j1Module.h"
#include "Point.h"
#include <list>
#include <vector>
#include <map>
#include "j1Timer.h"

#define DEFAULT_PATH_LENGTH 50
#define INVALID_WALK_CODE 255
#define FIND_RADIUS 5

// forward declarations
struct Path; 
struct PathList;
struct PathNode;

class j1PathFinding : public j1Module
{
public:

	j1PathFinding();

	// Destructor
	~j1PathFinding();

	// Called before quitting
	bool CleanUp();

	// We will call here the CalculatePath(path)
	bool PreUpdate();
	
	// Sets up the walkability map
	void SetMap(uint width, uint height, uchar* data);

	// Main function to request a path from A to B
	int CreatePath(const iPoint& origin, const iPoint& destination);

	vector<iPoint> GetPath(uint id) const;
	std::map<uint, Path*> GetPaths() const;

	// Utility: return true if pos is inside the map boundaries
	bool CheckBoundaries(const iPoint& pos) const;

	// Utility: returns true is the tile is walkable
	bool IsWalkable(const iPoint& pos) const;

	// Utility: return the walkability value of a tile
	uchar GetTileAt(const iPoint& pos) const;

	bool Jump(int current_x, int current_y, int dx, int dy, iPoint start, iPoint end, PathNode& new_node);

	void DeletePath(uint path_id);
private:
	iPoint FindNearestWalkable(const iPoint& origin);
	iPoint FindNearestWalkableToOrigin(const iPoint & origin, const iPoint& destination);
	int CalculatePath(Path* path, int max_iterations);
	void FillGaps(Path* path);
private:
	// Timer
	j1Timer timer;
	
	// Paths ID
	uint current_id = 0;

	// size of the map
	uint width;
	uint height;
	// all map walkability values [0..255]
	uchar* map;
	// we store the created paths here
	std::map<uint, Path*> paths;
};

// ---------------------------------------------------------------------
// Pathnode: Helper struct to represent a node in the path creation
// ---------------------------------------------------------------------
struct PathNode
{
	// Convenient constructors
	PathNode();
	PathNode(int g, int h, const iPoint& pos, const PathNode* parent);
	PathNode(const PathNode& node);

	// Fills a list (PathList) of all valid adjacent pathnodes
	uint FindWalkableAdjacents(PathList& list_to_fill) const;
	// Calculates this tile score
	int Score() const;
	// Calculate the F for a specific destination tile
	int CalculateF(const iPoint& destination);

	void IdentifySuccessors(PathList& list_to_fill, iPoint startNode, iPoint endNode, j1PathFinding* path_finder) const;

	iPoint FindNearestWalkableToOrigin(const iPoint & origin, const iPoint destination);

	// -----------
	int g;
	int h;
	iPoint pos;
	const PathNode* parent; // needed to reconstruct the path in the end
};

// ---------------------------------------------------------------------
// Helper struct to include a list of path nodes
// ---------------------------------------------------------------------
struct PathList
{
	// Looks for a node in this list and returns it's list node or NULL
	list<PathNode>::iterator Find(const iPoint& point);

	// Returns the Pathnode with lowest score in this list or NULL if empty
	list<PathNode>::iterator GetNodeLowestScore();

	// -----------
	// The list itself
	std::list<PathNode> node_list;
};

struct Path {
	Path() { completed = false; }
	PathList open;
	PathList closed;
	PathList adjacent;

	iPoint origin;
	iPoint destination;

	vector<iPoint> finished_path;

	bool completed;
};

#endif // __j1PATHFINDING_H__