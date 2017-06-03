#include "Defs.h"
#include "Log.h"
#include "j1App.h"
#include "j1PathFinding.h"
#include "Unit.h"
#include "j1Entity.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), width(0), height(0)
{
	name = "pathfinding";
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	for (std::map<uint, Path*>::iterator it = paths.begin(); it != paths.end();++it)
		RELEASE(it->second);
	paths.clear();

	RELEASE_ARRAY(map);
	return true;
}

bool j1PathFinding::PreUpdate()
{
	int iterations = 0;
	bool can_calculate = true;

	for (std::map<uint, Path*>::iterator it = paths.begin(); it != paths.end();) 
	{
		if (!it->second->completed) {
			if (can_calculate) {
				iterations = CalculatePath(it->second, 8 - iterations);

				if (iterations >= 8)
					can_calculate = false;
			}
			++it;
		}
		else{
			for (std::list<Entity*>::iterator it2 = App->entity->entity_list.begin(); it2 != App->entity->entity_list.end(); it2++) {
				if ((*it2)->GetType() != null && (*it2)->GetType() != enemy_building && (*it2)->GetType() != building && (*it2)->GetType() != ally_building) {
					if ((*it).first == ((Unit*)*it2)->path_id && it->second->completed && (*it).second->finished_path.size() < 200) {
						((Unit*)*it2)->SetPath((*it).second->finished_path);
					}
				}
			}
			RELEASE((*it).second);
			it = paths.erase(it);
		}
	}

	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
			pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if(CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
list<PathNode>::iterator PathList::Find(const iPoint& point)
{
	for (std::list<PathNode>::iterator it = node_list.begin(); it != node_list.end(); ++it)
	{
		if (it->pos == point) 
		{
			return it;
		}
	}
	return node_list.end();
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
list<PathNode>::iterator PathList::GetNodeLowestScore()
{
	list<PathNode>::iterator ret = node_list.end();
	int min = 65535;

	for (std::list<PathNode>::iterator it = node_list.begin(); it != node_list.end(); ++it)
	{
		if (it->Score() < min)
		{
			min = it->Score();
			ret = it;
		}
	}

	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill.node_list.size();

	// north
	cell.create(pos.x, pos.y + 1);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.node_list.push_back(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.node_list.push_back(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.node_list.push_back(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.node_list.push_back(PathNode(-1, -1, cell, this));

	// north-east
	cell.create(pos.x + 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.node_list.push_back(PathNode(-1, -1, cell, this));

	// north-west
	cell.create(pos.x - 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.node_list.push_back(PathNode(-1, -1, cell, this));

	// south-east
	cell.create(pos.x + 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.node_list.push_back(PathNode(-1, -1, cell, this));

	// south-west
	cell.create(pos.x - 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.node_list.push_back(PathNode(-1, -1, cell, this));

	return list_to_fill.node_list.size();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + this->pos.DistanceManhattan(parent->pos);
	h = pos.DistanceManhattan(destination);

	return g + h;

}

void PathNode::IdentifySuccessors(PathList & list_to_fill, iPoint startNode, iPoint endNode, j1PathFinding* pathfinder) const
{
	PathList neighbours;
	this->FindWalkableAdjacents(neighbours);

	list<PathNode>::iterator neighbour = neighbours.node_list.begin();

	while (neighbour != neighbours.node_list.end())
	{
		int dx = clamp(neighbour->pos.x - this->pos.x, -1, 1);
		int dy = clamp(neighbour->pos.y - this->pos.y, -1, 1);

		PathNode jump_point(-1, -1, iPoint(-1, -1), this);
		bool succed = pathfinder->Jump(this->pos.x, this->pos.y, dx, dy, startNode, endNode, jump_point);

		if (succed == true)
			list_to_fill.node_list.push_back(jump_point);

		++neighbour;
	}
}

bool j1PathFinding::Jump(int current_x, int current_y, int dx, int dy, iPoint start, iPoint end, PathNode& new_node)
{
	iPoint next(current_x + dx, current_y + dy);

	if (IsWalkable(next) == false)
		return false;		
	else if (next.x == end.x && next.y == end.y) 
	{
		new_node.pos = next;
		return true;
	}

	if (dx != 0 && dy != 0) // Diagonal Case   
	{
		if (!IsWalkable(iPoint(current_x + dx, current_y)))
		{
			new_node.pos = next;
			return true;
		}
		else if (!IsWalkable(iPoint(current_x, current_y + dy)))
		{
			new_node.pos = next;
			return true;
		}

		// Check in horizontal and vertical directions for forced neighbors
		// This is a special case for diagonal direction
		if (Jump(next.x, next.y, dx, 0, start, end, new_node) != NULL || Jump(next.x, next.y, 0, dy, start, end, new_node) != NULL)
		{
			new_node.pos = next;
			return true;
		}
	}
	else
	{
		if (dx != 0) // Horizontal case
		{
			if (!IsWalkable(iPoint(current_x, current_y + 1)))
			{
				if (IsWalkable(iPoint(current_x + dx, current_y + 1)))
				{
					new_node.pos = next;
					return true;
				}
			}
			else if (!IsWalkable(iPoint(current_x, current_y - 1)))
			{
				if (IsWalkable(iPoint(current_x + dx, current_y - 1)))
				{
					new_node.pos = next;
					return true;
				}
			}
		}
		else // Vertical case
		{
			if (!IsWalkable(iPoint(current_x + 1, current_y)))
			{
				if (IsWalkable(iPoint(current_x + 1, current_y + dy)))
				{
					new_node.pos = next;
					return true;
				}
			}
			else if (!IsWalkable(iPoint(current_x - 1, current_y)))
			{
				if (IsWalkable(iPoint(current_x - 1, current_y + dy)))
				{
					new_node.pos = next;
					return true;
				}
			}
		}

	}

	// If forced neighbor was not found try next jump point
	return Jump(next.x, next.y, dx, dy, start, end, new_node);
}

void j1PathFinding::DeletePath(uint path_id)
{
	for (std::map<uint, Path*>::iterator it = paths.begin(); it != paths.end();++it)
	{
		if (it->first == path_id) {
			it->second->completed = true;
		}
	}
}

iPoint j1PathFinding::FindNearestWalkable(const iPoint & origin)
{
	iPoint ret(origin);

	// dx -> direction x  | dy -> direction y  
	// search_in_radius -> finds the nearest walkable tile in a radius (max radius in FIND_RADIUS) 
	int search_in_radius = 1;
	while (search_in_radius != FIND_RADIUS)
	{
		for (int dx = -search_in_radius; dx < search_in_radius; dx++)
		{
			for (int dy = -search_in_radius; dy < search_in_radius; dy++)
			{
				ret.x = origin.x + dx;
				ret.y = origin.y + dy;
				if (IsWalkable(ret))
					return ret; // Found the nearest walkable tile
			}
		}

		++search_in_radius;
	}


	return ret.create(-1, -1);
}
iPoint j1PathFinding::FindNearestWalkableToOrigin(const iPoint& origin, const iPoint& destination){
	iPoint ret(destination);

	for (;;) {
		std::list<iPoint> walkable;
		std::list<iPoint> tiles;

		iPoint tile;

		tile = ret + iPoint(1, 0);
		tiles.push_back(tile);
		if (IsWalkable(tile))
			walkable.push_back(tile);

		tile = ret + iPoint(0, 1);
		tiles.push_back(tile);
		if (IsWalkable(tile))
			walkable.push_back(tile);

		tile = ret + iPoint(-1, 0);
		tiles.push_back(tile);
		if (IsWalkable(tile))
			walkable.push_back(tile);

		tile = ret + iPoint(0, -1);
		tiles.push_back(tile);
		if (IsWalkable(tile))
			walkable.push_back(tile);

		tile = ret + iPoint(1, 1);
		tiles.push_back(tile);
		if (IsWalkable(tile))
			walkable.push_back(tile);

		tile = ret + iPoint(-1, -1);
		tiles.push_back(tile);
		if (IsWalkable(tile))
			walkable.push_back(tile);

		if (!walkable.empty())
		{
			std::list<iPoint>::iterator it = walkable.begin();
			ret = *it;
			it++;
			for (; it != walkable.end(); ++it)
			{
				if ((*it).DistanceManhattan(origin) < ret.DistanceManhattan(origin))
					ret = *it;
			}

			return ret;
		}
		else {
			std::list<iPoint>::iterator it = tiles.begin();

			for (; it != tiles.end(); ++it)
			{
				if ((*it).DistanceManhattan(origin) < ret.DistanceManhattan(origin))
					ret = *it;
			}
		}
	}
}

	 //----------------||----------------||----------------\\
	//----------------||A* + JPS algorithm||----------------\\
   //----------------||____________________||----------------\\

int j1PathFinding::CalculatePath(Path * path, int max_iterations)
{
	int it_time = 0;

	timer.Start();

	while (path->open.node_list.size() > 0)
	{
		list<PathNode>::iterator lowest_score_node = path->open.GetNodeLowestScore(); // Get the lowest score node from the open list
		path->closed.node_list.push_back(*lowest_score_node);						  // Adds it to the closed list
		path->open.node_list.erase(lowest_score_node);								  // Delete the lowest_score_node from the open list
		list<PathNode>::iterator next_tile = --path->closed.node_list.end();		  // Next tile will be the newly added node

		if (next_tile->pos == path->destination)
		{
			path->finished_path.clear();
			const PathNode* path_node = &(*next_tile);

			while (path_node) // We backtrack to create the resulting path
			{
				path->finished_path.push_back(path_node->pos); // That path will be stored in finished_path, inside path
				path_node = path_node->parent;
			}

			iPoint* start = &path->finished_path[0];
			iPoint* end = &path->finished_path[path->finished_path.size() - 1];

			while (start < end)
				SWAP(*start++, *end--);

			path->completed = true;

			break;	// As the path is completed we exit the loop
		}
		path->adjacent.node_list.clear();											// Fill a list with all adjacent nodes
		next_tile->IdentifySuccessors(path->adjacent, path->origin, path->destination, this);

		for (std::list<PathNode>::iterator it = path->adjacent.node_list.begin(); it != path->adjacent.node_list.end();) 
		{																			// Iterate for every adjacent node
			if (path->closed.Find(it->pos) != path->closed.node_list.end())
			{
				++it;
				continue;
			}

			list<PathNode>::iterator adjacent_in_open = path->open.Find(it->pos);	// save from the open list to adjacent_in_open
																					// the node that has the position of the current
			if (adjacent_in_open == path->open.node_list.end())						// iterator
			{
				it->CalculateF(path->destination);
				path->open.node_list.push_back(*it);
			}
			else if (adjacent_in_open->g > it->g + 1) 
			{
				adjacent_in_open->parent = it->parent;
				adjacent_in_open->CalculateF(path->destination);
			}
			++it;
		}

		it_time += timer.Read();

		if (it_time >= max_iterations)
			break;
	}

	return it_time;
}

int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	int ret = -1;

	iPoint current_origin = origin;

	if (IsWalkable(destination))
	{
		if (!IsWalkable(origin))
		{
			current_origin = FindNearestWalkable(origin);

			if (IsWalkable(current_origin))
			{
				LOG("Found new origin walkable");
			}

			if (current_origin.x == -1 && current_origin.y == -1)
			{
				LOG("Path no created: Origin no walkable && no near walkable tiles available");
				return ret;
			}

		}
		Path* path = new Path();

		paths.insert(pair<uint, Path*>(++current_id, path));

		// Start pushing the origin in the open list
		path->open.node_list.push_back(PathNode(0, 0, current_origin, nullptr));

		path->origin = current_origin;
		path->destination = destination;

		ret = current_id; //Id of the path created
	}
	else {
		iPoint new_dest = FindNearestWalkableToOrigin(origin, destination);
		if (new_dest != iPoint(-1, -1)) {
			CreatePath(origin, new_dest);
		}
	}

	return current_id;
}

vector<iPoint> j1PathFinding::GetPath(uint id) const
{
	vector<iPoint> ret;
	std::map<uint, Path*>::const_iterator it = paths.find(id);

	if (it == paths.end())
	{
		LOG("PathFinding ERROR: wrong id to get Path");
	}
	else
	{
		ret = it->second->finished_path;
	}

	return ret;
}

std::map<uint, Path*> j1PathFinding::GetPaths() const
{
	return paths;
}

