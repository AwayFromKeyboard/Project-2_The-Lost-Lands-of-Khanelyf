#ifndef _SceneTest_H_
#define _SceneTest_H_

#include "Scene.h"
#include "j1Render.h"
#include "j1Gui.h"

class b2Fixture;
class Parallax;
class Hero;
class Barbarian;

#define TROOP_PRICE 20
#define TROOP_OFFSET 1

class SceneTest : public Scene
{
public:
	SceneTest();
	~SceneTest();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void OnColl(Collider* c1, Collider* c2);

	void CheckUnitCreation(iPoint p);
	void IncreaseGold(int gold);

private:
	UI_Window* cursor_window = nullptr;
	UI_Image* cursor = nullptr;

	UI_Window* general_ui_window = nullptr;
	UI_Image* general_ui_image = nullptr;

	SDL_Rect cursor_r;
	SDL_Rect ui_r;
	SDL_Rect buy_unit;

public:
	int gold = 0;
	UI_Text* gold_txt = nullptr;
	SDL_Texture* debug_tex = nullptr;

	int human_resources_max = 5;
	int current_human_resources = 0;
	UI_Text* human_resources_txt = nullptr;

	bool create_barrack = false;

public:
	//FXs
	unsigned int death_id;
	unsigned int death2_id;
	unsigned int get_hit_id;
	unsigned int get_hit2_id;
	unsigned int get_hit3_id;
	unsigned int get_hit4_id;
	unsigned int swords_clash_id;
	unsigned int swords_clash2_id;
	unsigned int swords_clash3_id;
	unsigned int swords_clash4_id;
	unsigned int swords_clash5_id;

public:
	bool escortedNPC_created = false;

public:
	SDL_Rect quest_0 = NULLRECT;
	SDL_Rect quest_2 = NULLRECT;
	SDL_Rect quest_3 = NULLRECT;
	SDL_Rect quest_4 = NULLRECT;

	int progress_quest_0 = 0;
	int progress_quest_2 = 0;
	int progress_quest_3 = 0;
	int progress_quest_4 = 0;
};

#endif // _SceneTest_H_
