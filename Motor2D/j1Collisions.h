#ifndef _j1Collisions_H_
#define _j1Collisions_H_

#define MAX_COLLIDERS 200

#include "j1Module.h"
#include <list>
#include <vector>
#include "SDL/include/SDL.h"

class QuadTree;

enum collider_type
{
	COLLIDER_NONE = -1,
	COLLIDER_UNIT,
	COLLIDER_BUILDING,

	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	collider_type type;
	bool print_collider = false;
	j1Module* callback = nullptr;
	int offset_x = 0;
	int offset_y = 0;

	Collider(SDL_Rect rectangle, collider_type type, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{
	}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class j1Collisions : public j1Module
{
public:

	j1Collisions();

	// Destructor
	virtual ~j1Collisions();

	// Called before render is available
	bool PreUpdate();

	bool Start();
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	void UpdateQuadtree();

	Collider* AddCollider(SDL_Rect rect, collider_type type, j1Module* callback = nullptr);
	bool EraseCollider(Collider* collider);
	void DebugDraw();

private:

	std::list<Collider*> colliders;
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];

	int quadTreeChecks;
	QuadTree* quadTree;
	std::vector<QuadTree*> nodeList;
	std::list<Collider*> potentialCollisionList;
public:
	bool debug = false;
};

#endif
