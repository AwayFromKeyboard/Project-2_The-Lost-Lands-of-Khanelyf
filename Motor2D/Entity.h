#ifndef _ENTITY_
#define _ENTITY_

#include "j1App.h"
#include "j1Render.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Physics.h"
#include "Animation.h"
#include <list>

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

public:
};

#endif