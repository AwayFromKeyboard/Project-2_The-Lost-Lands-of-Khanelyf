#ifndef _ENTITY_
#define _ENTITY_

#include "j1App.h"
#include "j1Render.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Physics.h"
#include "Animation.h"
#include "j1Entity.h"
#include <list>

enum entity_state {
	entity_idle,
	entity_move,
	entity_move_to_enemy,
	entity_attack,
	entity_death,
	entity_decompose,
	entity_null
};

class b2Fixture;
class PhysBody;
class GameObject;

class Entity
{
public:
	Entity() {};

	virtual ~Entity() {};

	virtual bool LoadEntity() { return true; };
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

	virtual void OnColl(Collider* col1, Collider* col2) {};

	virtual GameObject* GetGameObject() {
		return nullptr;
	}

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

public:
	entity_type type = entity_type::null;
	entity_state state = entity_state::entity_null;
	bool to_delete = false;

	int life = 0;
	int cost = 0; // only for allies

	iPoint position = NULLPOINT;
	Collider* collision = nullptr;
	//std::string texture = NULL;

protected:
	bool selected = false;
};

#endif