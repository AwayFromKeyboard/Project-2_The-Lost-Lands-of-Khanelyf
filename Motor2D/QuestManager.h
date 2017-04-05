#ifndef __QUESTMANAGER_H__
#define __QUESTMANAGER_H__

#include "j1Module.h"
#include "Point.h"
#include "j1Gui.h"
#include "Entity.h"
#include <string>
#include "SDL/include/SDL.h"

enum titles {

	beggar, leader, titles_null

};

enum quest_type {

	kill, capture, hire, quest_null
};

struct Rewards {

	uint gold = 0;
	titles newtitle = titles::titles_null;
	uint level_point = 0;
};

struct Quest {

public:

	string quest_name;

	string description;

	quest_type type = quest_type::quest_null;

	uint quest_number = 0;

	bool active = false;

	bool completed = false;

	std::list<Entity*> requested_list;

	uint progress = 0;

	uint requested = 0;

	Rewards reward;

	Quest(string name, string description, quest_type type,uint quest_number, uint requested, uint gold, titles new_title, uint level_points, bool active = false);

	~Quest();
};

class QuestManager : public j1Module
{
public:
	QuestManager();

	virtual ~QuestManager();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update(float dt);

	bool PreUpdate();

	bool PostUpdate();

	bool CleanUp();

	//Quest* CreateQuest(list<SDL_Rect*> image, iPoint pos); 

	Quest* GetCurrentQuest();

private:

	list<Quest*> quest_list;
	Quest* quest = nullptr;

};

#endif
