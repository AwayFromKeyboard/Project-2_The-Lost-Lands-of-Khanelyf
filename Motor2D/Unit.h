#ifndef _UNIT_H_
#define _UNIT_H_

#include "Entity.h"
#include "j1Pathfinding.h"

enum unit_state 
{
	unit_idle,
	unit_move,
	unit_attack,
	unit_attacking,
	unit_death,
	unit_decompose,
	unit_null
};

enum attack_state
{
	attack_unit,
	attack_building,
	attack_null
};

class GameObject;
class Building;
enum entity_name;

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
	GameObject* GetGameObject();

	// Pathfinding
	void SetPath(vector<iPoint> _path);
	vector<iPoint> GetPath() const;
	void FollowPath(float dt);
	void SetDirection();

	// Attack
	bool IsInRange(Entity* attacked_entity);
	void LookAtAttack();
	void UnitAttack();
	void BuildingAttack();
	void SetAttackingUnit(Unit* att_unit);
	void SetAttackingBuilding(Building* att_building);
public:
	GameObject* game_object = nullptr;
	unit_state state = unit_state::unit_null;
	attack_state att_state = attack_state::attack_null;
	entity_name type;
	bool flip = false;
	bool to_delete = true;
public:
	vector<iPoint> path;
	fPoint direction = NULLPOINT;
	iPoint destination = NULLPOINT;
	bool has_destination = false;
private:
	Unit* attacked_unit = nullptr;
	Building* attacked_building = nullptr;
public:
	int life = 0;
	int cost = 0;
	float speed = 0;
	int damage = 0;
	int armor = 0;
	int pierce_armor = 0;
	int range = 0;
	
	iPoint offset = NULLPOINT;
	iPoint i_offset = NULLPOINT;
	iPoint m_offset = NULLPOINT;
	iPoint a_offset = NULLPOINT;
	iPoint d_offset = NULLPOINT;
	iPoint de_offset = NULLPOINT;

	Animation* current_animation = nullptr;
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
