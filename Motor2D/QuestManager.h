#ifndef __QUESTMANAGER_H__
#define __QUESTMANAGER_H__

#include "j1Module.h"
#include "Point.h"
#include "j1Gui.h"
#include "Entity.h"
#include <string>
#include "SDL/include/SDL.h"

enum titles {
	beggar, leader, mayor, titles_null

};

enum quest_type {
	kill, capture, hire, create, move_object, type_null
};

enum quest_id {
	quest_beggar,
	quest_leader,
	quest_mayor,
	quest_mayor2,
	quest_provisions,
	quest_null
};

struct Rewards {
	uint gold = 0;
	titles newtitle = titles::titles_null;
	uint level_point = 0;
};

class Quest {
public:
	Quest(string name, string description, quest_type type, quest_id id, uint requested, uint gold, titles new_title, uint level_points, bool active = false);
	~Quest();

public:
	string name;
	string description;
	quest_type type = quest_type::type_null;
	quest_id id = quest_id::quest_null;
	Rewards reward;

	bool active = false;
	bool completed = false;

	uint progress = 0;
	uint requested = 0;
};

class QuestManager : public j1Module
{
public:
	QuestManager();

	virtual ~QuestManager();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();
	
	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	Quest* CreateQuest(string name, string description, quest_type type, quest_id id, uint requested, uint gold, titles new_title, uint level_points, bool active = false);
	Quest* GetCurrentQuest();
	Quest* ChangeQuest(quest_id new_quest);

private:
	list<Quest*> quest_list;
	Quest* current_quest = nullptr;

public:
	bool create_provision = true;
	iPoint provision_quest1 = NULLPOINT;
	iPoint provision_quest2 = NULLPOINT;
	SDL_Rect provision_quest_rect = NULLRECT;
};

#endif
