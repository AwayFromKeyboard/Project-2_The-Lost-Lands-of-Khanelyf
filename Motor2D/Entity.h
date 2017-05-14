#ifndef _ENTITY_
#define _ENTITY_

#include "j1App.h"
#include "j1Render.h"
#include "PugiXml\src\pugixml.hpp"
#include "Animation.h"
#include "j1Entity.h"
#include <list>

enum entity_state {
	entity_idle,
	entity_move,
	entity_move_to_enemy,
	entity_move_to_building,
	entity_attack,
	entity_death,
	entity_decompose,
	entity_pick_object,

	object_picked,
	object_dropped,
	object_destroyed,

	entity_null
};

enum entity_quest_id {
	quest_0,
	quest_1,
	quest_2,
	quest_3,
	quest_4,
	quest_5,
	quest_6,
	quest_7,

	quest_id_null
};


class Entity
{
public:
	Entity() {};

	virtual ~Entity() {};

	virtual bool LoadEntity(iPoint pos, entity_name name) { return true; };
	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool Draw(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	virtual void OnColl(Entity* en1, Entity* en2) {};

	virtual Collider* GetCollider() {
		return nullptr;
	}

	virtual entity_type GetType() {
		return type;
	}

	virtual void SetSelected(bool _selected) {
		selected = _selected;
	}
	bool GetSelected() {
		return selected;
	}

	void KillEntity(){
		life = 0;
		state = entity_death;
	};

	void LifeBar(iPoint size, iPoint offset);

public:
	entity_type type = entity_type::null;
	entity_state state = entity_state::entity_null;
	entity_name name = entity_name::test;
	entity_quest_id id = entity_quest_id::quest_id_null;
	bool to_delete = false;

	int life = 0;
	int max_life = 0;
	int cost = 0; // only for allies

	iPoint position = NULLPOINT;
	Collider* collision = nullptr;
	SDL_Texture* entity_texture;
	Animator* animator;

	bool show_life_bar = true;

	int layer = 0;
protected:
	bool selected = false;
};

#endif