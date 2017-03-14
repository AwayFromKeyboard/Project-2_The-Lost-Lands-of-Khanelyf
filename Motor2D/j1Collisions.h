#ifndef _j1Collisions_H_
#define _j1Collisions_H_

#define MAX_COLLIDERS 200

#include "j1Module.h"


enum collider_type
{
	COLLIDER_NONE = -1,
	COLLIDER_UNIT,

	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	collider_type type;
	j1Module* callback = nullptr;

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
	bool preUpdate();

	bool Update();

	// Called before quitting
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, collider_type type, j1Module* callback = nullptr);
	bool EraseCollider(Collider* collider);
	void DebugDraw();

private:

	Collider* colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = true;
};

#endif
