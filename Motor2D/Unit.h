#ifndef _UNIT_H_
#define _UNIT_H_

#include "Entity.h"
#include "j1Pathfinding.h"

enum unit_state {
	unit_idle,
	unit_move,
	unit_attack,
	unit_death,
	unit_decompose
};

enum unit_direction {
	north = 0,
	south,
	west,
	east,
	north_west,
	north_east,
	south_west,
	south_east
};

class GameObject;
enum entity_name;

class Unit : public Entity
{
private:
	unit_direction destination;

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
	GameObject* GetGameObject();

	// Pathfinding
	void SetPath(vector<iPoint> _path);
	vector<iPoint> GetPath() const;
	void CheckDirection();
	void FollowPath(float dt);
	void SetDirection();
public:
	GameObject* game_object = nullptr;
	unit_state state;
	entity_name type;
	bool flip = false;
	bool to_delete = true;
public:
	vector<iPoint> path;
	fPoint direction;
	bool has_destination = false;
public:
	int life = 0;
	int cost = 0;
	float speed = 0;
	int damage = 0;
	int armor = 0;
	int pierce_armor = 0;
	int range = 0;
	
	iPoint offset;
	iPoint i_offset;
	iPoint m_offset;
	iPoint a_offset;
	iPoint d_offset;
	iPoint de_offset;

	Animation* current_animation;
	// Idle
	Animation i_south;
	Animation i_south_west;
	Animation i_west;
	Animation i_north_west;
	Animation i_north;
	// Move
	Animation m_south;
	Animation m_south_west;
	Animation m_west;
	Animation m_north_west;
	Animation m_north;
	// Attack
	Animation a_south;
	Animation a_south_west;
	Animation a_west;
	Animation a_north_west;
	Animation a_north;
	// Death
	Animation d_south;
	Animation d_south_west;
	Animation d_west;
	Animation d_north_west;
	Animation d_north;
	// Decompose
	Animation de_south;
	Animation de_south_west;
	Animation de_west;
	Animation de_north_west;
	Animation de_north;
};

#endif
