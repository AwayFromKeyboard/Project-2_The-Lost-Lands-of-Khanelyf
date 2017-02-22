#ifndef _UNIT_H_
#define _UNIT_H_

#include "Entity.h"

class GameObject;

class Unit : public Entity
{
public:
	Unit();
	~Unit();

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

public:
	GameObject* game_object = nullptr;

	bool flip = false;
};

#endif