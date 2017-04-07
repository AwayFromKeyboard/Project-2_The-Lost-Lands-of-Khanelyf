#ifndef _UNIT_H_
#define _UNIT_H_

#include "Entity.h"
#include "j1Pathfinding.h"

struct Collider;

enum unit_state {
	unit_idle,
	unit_move,
	unit_move_to_enemy,
	unit_attack,
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
enum entity_type;

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
	entity_type GetType();
	void SetSelected(bool _selected);

	// Pathfinding
	void SetPath(vector<iPoint> _path);
	vector<iPoint> GetPath() const;
	void CheckDirection();
	void FollowPath(float dt);
	void SetDirection();
	void LookAtMovement();

	// Attack
	bool CheckSurroundings();
	bool IsInRange(Entity* attacked_entity);
	void LookAtAttack();
	void UnitAttack();
	void BuildingAttack();
	void SetAttackingUnit(Unit* att_unit);
	void SetAttackingBuilding(Building* att_building);
  
	// Death
	void CheckDeathDirection();

	//Decompose
	void CheckDecomposeDirection();
  
public:
	GameObject* game_object = nullptr;
	unit_state state = unit_state::unit_null;
	entity_name name;
	bool flip = false;
  
public:
	vector<iPoint> path;
	fPoint direction = NULLPOINT;
	bool has_destination = false;
	uint path_id;
public:
	Unit* attacked_unit = nullptr;
	Building* attacked_building = nullptr;
	attack_state att_state = attack_state::attack_null;
	bool has_moved = false;
public:
	int human_cost = 0; // only for allies
	int gold_drop = 0; // only for enemies
	float speed = 0;
	int damage = 0;
	int armor = 0;
	int pierce_armor = 0;
	int range = 0;

	iPoint position = NULLPOINT;
	iPoint position_map = NULLPOINT;

	Collider* collision = nullptr;

	iPoint offset = NULLPOINT;
	iPoint i_offset = NULLPOINT;
	iPoint m_offset = NULLPOINT;
	iPoint a_offset = NULLPOINT;
	iPoint d_offset = NULLPOINT;
	iPoint de_offset = NULLPOINT;

	//flip
	int flip_i_offset = 0;
	int flip_m_offset = 0;
	int flip_a_offset = 0;
	int flip_d_offset = 0;
	int flip_de_offset = 0;

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
	uint radius_of_action = 0;

private:
		j1Timer death_timer;
		j1Timer AI_timer;
public:
	bool IsInsideCircle(int x, int y);

public:
	bool shout_fx = true;
};

#endif

