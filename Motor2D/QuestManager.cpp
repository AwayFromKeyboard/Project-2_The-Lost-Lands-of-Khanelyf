#include "QuestManager.h"
#include "j1Gui.h"
#include "Log.h"
#include "j1Scene.h"
#include "SceneTest.h"
#include "Player.h"

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
	current_quest = CreateQuest("Protect the village!", "kill 5 enemies in the village", quest_type::kill, quest_id::quest_beggar, 5, 100, titles::leader, 3, true);
	CreateQuest("Create a barrack!", "Create a barrack to hire some units, we need more protection", quest_type::create, quest_id::quest_leader, 1, 20, titles::leader, 2, false);
	CreateQuest("Kill the enemies at the east!", "Go to the east and kill 4 enemies at the fortress", quest_type::kill, quest_id::quest_mayor, 4, 500, titles::mayor, 5, false);
	CreateQuest("Kill the enemies at the north!", "Go to the north and kill 5 enemies at the fortress", quest_type::kill, quest_id::quest_mayor2, 5, 500, titles::mayor, 5, false);
	
	return true;
}

bool QuestManager::PreUpdate() 
{ 
	return true; 
}

bool QuestManager::Update(float dt) {

	if (current_quest->progress == current_quest->requested) {
		current_quest->progress = 0;
		App->scene->scene_test->IncreaseGold(current_quest->reward.gold);
		// Increase level points
		// change title
		current_quest->active = false;
		switch (current_quest->id)
		{
		case quest_beggar:
			current_quest = ChangeQuest(quest_id::quest_leader);
			break;
		case quest_leader:
			current_quest = ChangeQuest(quest_id::quest_mayor);
			break;
		case quest_mayor:
			current_quest = ChangeQuest(quest_id::quest_mayor2);
			break;
		case quest_mayor2:
			current_quest = ChangeQuest(quest_id::quest_null);
			break;
		case quest_null:
			break;
		default:
			break;
		}
	
	}

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

Quest * QuestManager::ChangeQuest(quest_id new_quest)
{
	for (list<Quest*>::iterator it = quest_list.begin(); it != quest_list.end(); it++) {
		if ((*it)->id == new_quest) {
			(*it)->active = active;
			return *it;
		}
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