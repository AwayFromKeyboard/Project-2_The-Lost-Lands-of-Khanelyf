#include "QuestManager.h"
#include "j1Gui.h"
#include "Log.h"

QuestManager::QuestManager() {

	name = "Questmanager";
}

QuestManager:: ~QuestManager() {}

bool QuestManager::Awake(pugi::xml_node&) {
	bool ret = true;

	LOG("Loading QuestManager");

	return ret;
}

bool QuestManager::Start() {
	CreateQuest("Protect the village!", "kill 5 enemies in the village", quest_type::kill, quest_id::quest_beggar, 5, 20, titles::leader, 3, true);
	CreateQuest("Create a barrack!", "Create a barrack to hire some units, we need more protection", quest_type::create, quest_id::quest_leader, 1, 20, titles::leader, 2, false);
	CreateQuest("Kill the enemies at the east!", "Go to the east and kill 4 enemies at the fortress", quest_type::kill, quest_id::quest_leader, 4, 500, titles::mayor, 5, false);
	CreateQuest("Kill the enemies at the north!", "Go to the north and kill 5 enemies at the fortress", quest_type::kill, quest_id::quest_leader, 5, 500, titles::mayor, 5, false);
	
	return true;
}

bool QuestManager::Update(float dt) {

	return true;
}

bool QuestManager::PreUpdate() 
{ 
	return true; 
}

bool QuestManager::PostUpdate() {
	return true; 
}

bool QuestManager::CleanUp() {

	for (list<Quest*>::iterator it = quest_list.begin(); it != quest_list.end(); it++)
	{
		RELEASE(*it);
	}
	quest_list.clear();

	return true;
}

Quest* QuestManager::CreateQuest(string name, string description, quest_type type, quest_id id, uint requested, uint gold, titles new_title, uint level_points, bool active)
{
	Quest* quest = new Quest(name, description, type, id, requested, gold, new_title, level_points, active);
	quest_list.push_back(quest);

	return quest;
}

Quest * QuestManager::GetCurrentQuest() {


	for (list<Quest*>::iterator it = quest_list.begin(); it != quest_list.end(); it++) {
		if ((*it)->active)
			return *it;
	}

	return nullptr;
}

Quest::Quest(string name, string description, quest_type type, quest_id id, uint requested, uint gold, titles new_title, uint level_points, bool active)
{
	this->name = name;
	this->description = description;
	this->type = type;
	this->id = id;
	this->requested = requested;
	reward.gold = gold;
	reward.newtitle = new_title;
	reward.level_point = level_points;
	this->active = active;
}

Quest::~Quest()
{
}