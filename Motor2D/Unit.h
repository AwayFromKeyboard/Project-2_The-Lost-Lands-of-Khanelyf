#ifndef _UNIT_H_
#define _UNIT_H_

#include "Entity.h"
#include "j1Pathfinding.h"

enum unit_type {
	unit_test,
	unknown
};

enum unit_state {
	unit_idle,
	unit_move,
	unit_attack,
	unit_death,
	unit_decompose
};
class GameObject;

class Unit : public Entity
{
public:
	Unit();
	Unit(unit_type  type);
	~Unit();

	bool LoadEntity();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool Draw(float dt);
	bool PostUpdate();
	bool CleanUp();

	Entity* CreateUnit(unit_type _type);
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);
	GameObject* GetGameObject();

	// Pathfinding
	void SetPath(vector<iPoint> _path);
	vector<iPoint> GetPath() const;
	void FollowPath(float dt);
	void SetDirection();
public:
	std::list<Entity*> unit_list;
	GameObject* game_object = nullptr;
	unit_state state;
	unit_type type;
	bool flip = false;
public:
	vector<iPoint> path;
	fPoint direction;
	iPoint destination;
	bool has_destination = false;
	bool end_movement = true;

};

#endif
