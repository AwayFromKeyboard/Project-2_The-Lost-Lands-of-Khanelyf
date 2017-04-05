#ifndef _BUILDING_H_
#define _BUILDING_H_

#include "Entity.h"

struct Collider;

enum entity_name;
enum entity_type;
class GameObject;

class Building : public Entity
{
public:
	Building();
	~Building();

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
	GameObject* GetGameObject();
	Collider* GetCollider();
	entity_type GetType();

public:

	GameObject* game_object = nullptr;
	entity_name name;
	bool to_delete = false;

public:

	int cost = 0;

	iPoint position = NULLPOINT;
	iPoint offset = NULLPOINT;

	Collider* collision = nullptr;
	SDL_Rect tex_rect = NULLRECT;
	entity_type type = entity_type::null;

public:
	bool is_selected = false;
};

#endif