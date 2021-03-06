#ifndef _UNIT_H_
#define _UNIT_H_

#include "Entity.h"
#include "j1Pathfinding.h"

struct Collider;

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

enum boss_phase {
	phase_1, // normal attacks
	phase_2, // move to diferent entities fast and attack (sort of a charge ability)
	phase_3, // area attack

	last_phase,

	asleep
};

enum entity_name;
enum entity_type;

class Building;
class Object;
class BossAxeKnight;
class EscortedNPC;
class BattlecryBuff;

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

	void CheckPhase();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void OnColl(Collider* col1, Collider* col2);
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

	//Object iteration
	void SetPickObject(Object* object);
	void PickObject();
	void DropObject();
  
public:
	bool flip = false;
  
	bool can_hold_object = false;
	bool is_holding_object = false;

public:
	vector<iPoint> path;
	fPoint direction = NULLPOINT;
	bool has_destination = false;
	uint path_id;
public:
	Unit* attacked_unit = nullptr;
	Building* attacked_building = nullptr;
	Object* to_pick_object = nullptr;
	attack_state att_state = attack_state::attack_null;
	bool has_moved = false;
	bool stay_still = false;
public:
	int human_cost = 0; // only for allies
	int gold_drop = 0; // only for enemies
	float speed = 0;
	int damage = 0;
	int armor = 0;
	int pierce_armor = 0;
	int range = 0;

	j1Timer life_up_timer;
	iPoint position_map = NULLPOINT;
	iPoint aux_pos = NULLPOINT;

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
	bool buffed = false;
private:
	j1Timer death_timer;
	j1Timer AI_timer;
	j1Timer whirlwind_damage;
	bool timer_whirlwind_start = true;
	bool choose_another_build = false;
public:
	bool IsInsideCircle(int x, int y);
	bool damaged_by_whirlwind = false;
public:
	// attacked audio
	bool shout_fx = true;

public:
	bool is_boss = false;
	boss_phase phase = asleep;
	BossAxeKnight* boss = nullptr;

	bool LooksDiagonal();

public:
	bool is_escortednpc = false;
	EscortedNPC* npc_quest = nullptr;
	BattlecryBuff* buff_particle = nullptr;
};

#endif

