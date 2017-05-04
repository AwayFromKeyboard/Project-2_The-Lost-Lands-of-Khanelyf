#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Entity.h"
#include "j1Timer.h"

struct Collider;

enum entity_name;
enum entity_type;

class Object : public Entity
{
public:
	Object();
	~Object();

	bool LoadEntity();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool Draw(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void OnColl(Entity* en1, Entity* en2);
	Collider* GetCollider();
	entity_type GetType();
	entity_name GetName();

public:
	entity_name name;

public:

	iPoint offset = NULLPOINT;
	SDL_Rect tex_rect = NULLRECT;

public:
	bool is_carried = false;
	bool pickable = true;

private:
	SDL_Rect aux_collision_rect = NULLRECT;
	j1Timer death_timer;
};

#endif