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


	Quest* quest = new Quest("first quest", "kill enemies in town", kill, 5, 20, titles::leader, 3);
	quest_list.push_back(quest);
	return true;
}

bool QuestManager::Update(float dt) {

	/*Quest* current = GetCurrentQuest();
	if (current->type==kill)
	for (list<Entity*>::iterator it = requested_list.begin(); it != requested_list.end(); it++)*/

	return true;
}

bool QuestManager::PreUpdate() { return true; }

bool QuestManager::PostUpdate() { return true; }

bool QuestManager::CleanUp() {

	for (list<Quest*>::iterator it = quest_list.begin(); it != quest_list.end(); it++)
	{
		RELEASE(*it);

	}
	quest_list.clear();

	return true;
}

Quest * QuestManager::GetCurrentQuest() {


	for (list<Quest*>::iterator it = quest_list.begin(); it != quest_list.end(); it++) {
		if ((*it)->active)
			return *it;
	}

	return nullptr;
}

/*Quest* QuestManager::CreateQuest(list<SDL_Rect*> image, iPoint pos) {

Quest* new_quest = new Quest;
}*/

Quest::Quest(string name, string description, quest_type type, uint requested, uint gold, titles new_title, uint level_points, bool active)
{

	quest_name = name;
	this->description = description;
	this->type = type;
	this->requested = requested;
	reward.gold = gold;
	reward.newtitle = new_title;
	reward.level_point = level_points;
	this->active = active;

}

Quest::~Quest()
{
}