#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "j1Module.h"
#include "Defs.h"

class b2Fixture;
class PhysBody;
class GameObject;

enum entity_name
{
	hero,
	barbarian,
	swordsman,
	barracks,
	test
};

enum entity_type
{
	null = 0,
	player,
	ally,
	npc,
	enemy,
	building
};

class Entity;
class Player;
class Unit;
class Test;
struct SDL_Rect;

struct SelectedList {
	std::list<Unit*> group;
	int key_id = 0;
};

class j1Entity : public j1Module
{
public:

	j1Entity();

	// Destructor
	virtual ~j1Entity();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void OnCollision(Collider* col1, Collider* col2);

	Entity* CreateEntity(entity_name name, entity_type type);
	void DeleteEntity(Entity* entity);

	void SelectInQuad(const SDL_Rect& select_rect);
	void UnselectEverything();

	void AddGroup();
	void ManageGroup();

private:

public:
	// List with all entities
	std::list<Entity*> entity_list;
	std::list<GameObject*> unit_game_objects_list;
	std::list<GameObject*> building_game_objects_list;

	std::list<SelectedList> lists_selected;
	std::list<Unit*> selected;
	//std::list<Props*> props;
};

#endif // __j1ENTITY_H__