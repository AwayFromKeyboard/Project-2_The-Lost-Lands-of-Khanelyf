#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Entity.h"

struct Collider;

enum entity_name;
enum entity_type;

class Object : public Entity
{
public:
	Object();
	~Object();

	bool LoadEntity();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool Draw(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);
	Collider* GetCollider();
	entity_type GetType();
	entity_name GetName();

public:

	entity_name name;
	bool to_delete = false;

public:

	iPoint offset = NULLPOINT;
	SDL_Rect tex_rect = NULLRECT;

public:
	bool is_carried = false;
	bool pickable = false;

private:
	SDL_Rect aux_collision_rect = NULLRECT;
};

#endif