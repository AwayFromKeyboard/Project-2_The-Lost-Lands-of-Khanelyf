#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "j1Module.h"
#include "Defs.h"

class b2Fixture;
class PhysBody;
class GameObject;

enum entity_name
{
	player,
	test,
	entity_null
};

class Entity;
class Player;
class Test;
class j1Entity : public j1Module
{
public:

	j1Entity();

	// Destructor
	virtual ~j1Entity();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

	Entity* CreateEntity(entity_name entity);
	void DeleteEntity(Entity* entity);

private:

public:
	// List with all entities
	std::list<Entity*> entity_list;
	std::list<GameObject*> unit_game_objects_list;
	std::list<GameObject*> building_game_objects_list;
};

#endif // __j1ENTITY_H__