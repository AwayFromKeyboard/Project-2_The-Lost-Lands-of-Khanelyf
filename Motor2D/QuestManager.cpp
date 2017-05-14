#include "QuestManager.h"
#include "j1Gui.h"
#include "Log.h"
#include "j1Scene.h"
#include "SceneTest.h"
#include "Player.h"
#include "Hero.h"
#include "DialogueManager.h"
#include "j1Map.h"

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
	// Quests
	current_quest = CreateQuest("Protect the village!", "kill 5 enemies in the village", quest_type::kill, quest_id::quest_beggar, 5, 100, titles::leader, 3, true);
	CreateQuest("Create a barrack!", "Create a barrack to hire some units, we need more protection", quest_type::create, quest_id::quest_leader, 1, 20, titles::leader, 2, false);
	CreateQuest("Kill the enemies at the east!", "Go to the east and kill 4 enemies at the towers", quest_type::kill, quest_id::quest_mayor, 4, 50, titles::mayor, 5, false);
	CreateQuest("Kill the enemies at the north!", "Go to the north and kill 5 enemies at the fortress", quest_type::kill, quest_id::quest_mayor2, 5, 75, titles::mayor, 5, false);
	CreateQuest("Conquer the north fortress!", "Attack to the enemy houses near the fortress to conquer them", quest_type::conquer, quest_id::quest_conquer, 2, 20, titles::mayor, 3, false);
	CreateQuest("Escort the NPC", "Escort the old man to the main village", quest_type::escort, quest_id::quest_escort, 1, 200, titles::mayor, 5, false );
	CreateQuest("Give provisions to the other village!", "Go to the captured village and leave the provisions there", quest_type::move_object, quest_id::quest_provisions, 1, 100, titles::mayor, 3, false);
	CreateQuest("kill the boss", "Go to the north city and kill the boss", quest_type::kill, quest_id::quest_boss, 1, 500, titles::king, 5, false);

	CreateQuest("", "", quest_type::type_null, quest_id::quest_null, 999, 999, titles::titles_null, 999, false);

	// Variables
	provision_quest1 = App->map->MapToWorldPoint({ 20, 21 });
	provision_quest2 = App->map->MapToWorldPoint({ 6, 19 });
	provision_quest_rect = { provision_quest2.x, provision_quest2.y, provision_quest1.x - provision_quest2.x, provision_quest1.y - provision_quest2.y };

	// Dialogs
	App->dialogs->id = current_quest->id;
	App->dialogs->NPCstate = 0;
	App->dialogs->dialogueStep = 0;

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
		App->player->GetHero()->levelup_points += current_quest->reward.level_point;
		current_quest->active = false;
		switch (current_quest->id)
		{
		case quest_beggar:
			current_quest = ChangeQuest(quest_id::quest_leader);
			App->scene->scene_test->create_barrack = true;
			App->scene->scene_test->enemy_waves_active = true;
			break;
		case quest_leader:
			current_quest = ChangeQuest(quest_id::quest_mayor);
			current_quest->progress += App->scene->scene_test->progress_quest_2;
			break;
		case quest_mayor:
			current_quest = ChangeQuest(quest_id::quest_mayor2);
			current_quest->progress += App->scene->scene_test->progress_quest_3;
			App->player->create_swordsman = true;
			break;
		case quest_mayor2:
			current_quest = ChangeQuest(quest_id::quest_conquer);
			current_quest->progress += App->scene->scene_test->progress_quest_4;
			App->player->create_swordsman = true;
			break;
		case quest_conquer:
			current_quest = ChangeQuest(quest_id::quest_escort);
			break;
		case quest_escort:
			current_quest = ChangeQuest(quest_id::quest_provisions);
			break;
		case quest_provisions:
			current_quest = ChangeQuest(quest_id::quest_boss);
			break;
		case quest_boss:
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

bool QuestManager::Load(pugi::xml_node& data)
{
	pugi::xml_node quest = data.child("Current_Quest");
	pugi::xml_node dialogues = data.child("Dialogues");

	int _id = quest.attribute("ID").as_int();
	quest_id id;
	switch (_id)
	{
	case quest_beggar:
		id = quest_beggar;
		break;
	case quest_leader:
		id = quest_leader;
		break;
	case quest_mayor:
		id = quest_mayor;
		break;
	case quest_mayor2:
		id = quest_mayor2;
		break;
	case quest_conquer:
		id = quest_conquer;
		break;
	case quest_provisions:
		id = quest_provisions;
		break;
	case quest_escort:
		id = quest_escort;
		break;
	case quest_boss:
		id = quest_boss;
		break;
	case quest_null:
		id = quest_null;
		break;
	}

	ChangeQuest(id);
	current_quest->progress = quest.attribute("Progress").as_int();

	App->dialogs->LoadGame(dialogues);

	return true;
}

bool QuestManager::Save(pugi::xml_node& data) const
{
	pugi::xml_node quest = data.append_child("Current_Quest");
	pugi::xml_node dialogues = data.append_child("Dialogues");

	quest.append_attribute("ID") = current_quest->id;
	quest.append_attribute("Progress") = current_quest->progress;

	App->dialogs->SaveGame(dialogues);

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
			App->dialogs->id = (*it)->id;
			App->dialogs->NPCstate = 0;
			App->dialogs->dialogueStep = 0;
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