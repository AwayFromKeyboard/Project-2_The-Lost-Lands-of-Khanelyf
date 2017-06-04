#ifndef _SceneTest_H_
#define _SceneTest_H_

#include "Scene.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "Animation.h"

class b2Fixture;
class Parallax;
class Hero;
class Barbarian;

class UI_Window;
class UI_Image;
class UI_Button;
class UI_Text;

#define TROOP_PRICE 20
#define TROOP_OFFSET 1

enum enemy_waves
{
	first,
	second,
	third,
	fourth,
	fifth,
	sixth,
	seventh,
	eighth,
	ninth,
	tenth,
	none
};

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

	UI_Image* GetCursor();
	void SetCurrentCursor(SDL_Rect new_cursor);

private:
	UI_Window* cursor_window = nullptr;
	UI_Image* cursor = nullptr;
	SDL_Rect current_cursor_r = NULLRECT;

	UI_Window* general_ui_window = nullptr;
	UI_Image* general_ui_image = nullptr;

	SDL_Rect ui_r = NULLRECT;
	SDL_Rect buy_unit = NULLRECT;

	enemy_waves enemy_wave_number = none;
	
	UI_Window* main_menu_window = nullptr;
	UI_Image* main_menu_background = nullptr;
	
	UI_Button* new_game_button = nullptr;
	UI_Button* load_game_button = nullptr;
	UI_Button* credits_button = nullptr;
	UI_Button* exit_game_button = nullptr;
	UI_Button* trailer_button = nullptr;

	UI_Text* new_game_txt = nullptr;
	UI_Text* load_game_txt = nullptr;
	UI_Text* credits_txt = nullptr;
	UI_Text* exit_game_txt = nullptr;
	UI_Text* trailer_txt = nullptr;

public:
	SDL_Rect cursor_r = NULLRECT;
	SDL_Rect cursor_attack_r = NULLRECT;
	SDL_Rect cursor_build_r = NULLRECT;
	SDL_Rect cursor_object_r = NULLRECT;
	SDL_Rect cursor_ui_r = NULLRECT;
	bool move_init = false;
	iPoint clicked_point = NULLPOINT;

	int gold = 0;
	UI_Text* gold_txt = nullptr;
	SDL_Texture* debug_tex = nullptr;

	int human_resources_max = 1;
	int current_human_resources = 0;
	UI_Text* human_resources_txt = nullptr;

	bool create_barrack = false;
	bool create_blacksmith = false;

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

public:
	bool mission_provision_created = false;

	j1Timer enemy_waves_timer;
	bool start_waves_timer = true;
	bool enemy_waves_active = false;

};

#endif // _SceneTest_H_
