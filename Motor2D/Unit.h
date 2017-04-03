#ifndef _UNIT_H_
#define _UNIT_H_

#include "Entity.h"
#include "j1Pathfinding.h"

struct Collider;

enum unit_state {
	unit_idle,
	unit_move,
	unit_attack,
	unit_attacking,
	unit_death,
	unit_decompose,
	unit_null
};

enum attack_state {
	attack_unit,
	attack_building,
	attack_null
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
class Building;

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

	void OnColl(Collider* col1, Collider* col2);
	GameObject* GetGameObject();
	Collider* GetCollider();

	// Pathfinding
	void SetPath(vector<iPoint> _path);
	vector<iPoint> GetPath() const;
	void CheckDirection();
	void FollowPath(float dt);
	void SetDirection();
	void LookAtMovement();

	// Attack
	bool IsInRange(Entity* attacked_entity);
	void LookAtAttack();
	void UnitAttack();
	void BuildingAttack();
	void SetAttackingUnit(Unit* att_unit);
	void SetAttackingBuilding(Building* att_building);
  
	// Death
	void CheckDeathDirection();
	j1Timer death_timer;

	//Decompose
	void CheckDecomposeDirection();
  
public:
	GameObject* game_object = nullptr;
	unit_state state = unit_state::unit_null;
	entity_name type;
	bool flip = false;
	bool to_delete = false;
  

public:
	vector<iPoint> path;
	fPoint direction = NULLPOINT;
	bool has_destination = false;
	uint path_id;
public:
	Unit* attacked_unit = nullptr;
	Building* attacked_building = nullptr;
	attack_state att_state = attack_state::attack_null;
public:
	int life = 0;
	int cost = 0;
	float speed = 0;
	int damage = 0;
	int armor = 0;
	int pierce_armor = 0;
	int range = 0;

	iPoint position = NULLPOINT;

	Collider* idle_collision = nullptr;
	Collider* walk_collision = nullptr;
	Collider* attack_collision = nullptr;

	iPoint offset = NULLPOINT;
	iPoint i_offset = NULLPOINT;
	iPoint m_offset = NULLPOINT;
	iPoint a_offset = NULLPOINT;
	iPoint d_offset = NULLPOINT;
	iPoint de_offset = NULLPOINT;

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

public:
	bool is_selected = false;
};

#endif

