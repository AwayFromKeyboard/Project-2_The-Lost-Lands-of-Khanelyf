#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "j1Timer.h"

#include "Point.h"

#define DURATION_BATTLECRY 5
#define COOLDOWN_BATTLECRY 30
#define BATTLECRY_BUFF 5
#define BATTLECRY_RANGE 10

#define DURATION_UNDYING_WILL 4
#define COOLDOWN_UNDYING_WILL 20

#define COOLDOWN_WHIRLWIND 8
#define WHIRLWIND_DAMAGE 40
#define WHIRLWIND_RANGE 2

#define COOLDOWN_CHARGE 12
#define CHARGE_DAMAGE 40
#define CHARGE_RANGE 6
#define CHARGE_SPEED 5

enum first_ability {
	battlecry_active,
	undying_will_active,
	not_chosen
};

class Unit;
class Building;
class UI_Window;
class UI_Text;
class UI_Image;
class UI_Button;
class Hero;
class Object;

class Player : public j1Module {
public:
	Player();
	~Player();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
private:
	void Battlecry();
	void BattlecryModifier(int damage_buff);
	void CheckAbilityRange(int range);
	void CheckStraightAbilityRange(int range);
	void DrawBuff();
	void StopBuff(int modifier);

	void Whirlwind();

	void Charge();

	void DrawCD(int ability_number);
private:
	void CheckMouseEntity();

public:
	UI_Window* pause_window = nullptr;
	UI_Button* quit_game = nullptr;
	
private:
	UI_Window* victory_window = nullptr;
	UI_Text* victory = nullptr;
	UI_Text* lose = nullptr;
	UI_Image* pause_bg = nullptr;

	UI_Button* back = nullptr;
	UI_Button* options = nullptr;
	UI_Button* save = nullptr;
	UI_Button* load = nullptr;
	UI_Button* audio_button = nullptr;
	UI_Button* controls_button = nullptr;
	UI_Button* backpause = nullptr;
	UI_Button* returnmenu = nullptr;
	UI_Button* mainmenu = nullptr;

	UI_Text* pause_menu_txt = nullptr;
	UI_Text* quit_txt = nullptr;
	UI_Text* save_txt = nullptr;
	UI_Text* options_txt = nullptr;
	UI_Text* back_txt = nullptr;
	UI_Text* load_txt = nullptr;
	UI_Text* controls_txt = nullptr;
	UI_Text* audio_txt = nullptr;
	UI_Text* backpause_txt = nullptr;
	UI_Text* mainmenu_txt = nullptr;

	//Controls buttons and texts
	UI_Image* controls_bg = nullptr;

	UI_Button* backoptions = nullptr;;
	UI_Button* battlecry_button_options = nullptr;
	UI_Button* whirlwind_button_options = nullptr;
	UI_Button* charge_button_options = nullptr;
	UI_Button* move_map_up_button = nullptr;
	UI_Button* move_map_down_button = nullptr;
	UI_Button* move_map_left_button = nullptr;
	UI_Button* move_map_right_button = nullptr;
	UI_Button* create_group_button = nullptr;

	UI_Text* backoptions_txt = nullptr;
	UI_Text* battlecry_txt = nullptr;
	UI_Text* whirlwind_txt = nullptr;
	UI_Text* charge_txt = nullptr;
	UI_Text* move_map_up_txt = nullptr;
	UI_Text* move_map_down_txt = nullptr;
	UI_Text* move_map_left_txt = nullptr;
	UI_Text* move_map_right_txt = nullptr;
	UI_Text* create_group_txt = nullptr;

	UI_Text* battlecry_txt2 = nullptr;
	UI_Text* whirlwind_txt2 = nullptr;
	UI_Text* charge_txt2 = nullptr;
	UI_Text* move_map_up_txt2 = nullptr;
	UI_Text* move_map_down_txt2 = nullptr;
	UI_Text* move_map_left_txt2 = nullptr;
	UI_Text* move_map_right_txt2 = nullptr;
	UI_Text* create_group_txt2 = nullptr;

	UI_Text* audio_on_off = nullptr;

	UI_Window* attributes_window = nullptr;
	UI_Text* life_txt = nullptr;
	UI_Text* damage_txt = nullptr;
	UI_Text* armor_txt = nullptr;
	UI_Text* pierce_armor_txt = nullptr;

	UI_Text* battlecry_cd = nullptr;
	UI_Text* undying_will_cd = nullptr;
	UI_Text* whirlwind_cd = nullptr;
	UI_Text* charge_cd = nullptr;

	UI_Text* battlecry_key = nullptr;
	UI_Text* whirlwind_key = nullptr;
	UI_Text* charge_key = nullptr;

	UI_Text* show_ability_name = nullptr;

	UI_Window* levelup_window = nullptr;
	UI_Button* life_button = nullptr;
	UI_Button* damage_button = nullptr;
	UI_Button* armor_button = nullptr;
	UI_Button* pierce_armor_button = nullptr;
	UI_Text* level_points_txt = nullptr;

	Hero* hero = nullptr;
public:
	void MoveToTile(iPoint tile);
private:
	void SetAttackingEnemy(Unit* enemy);
	void SetAttackingBuilding(Building* building);
	void UpdateAttributes();
	void SetPickingObject(Object* object);
public:
	void SetHero(Hero* hero);
	Hero* GetHero();

	std::list<Unit*> buffed_list;
public:
	UI_Window* barracks_ui_window = nullptr;
	UI_Window* brokenbuilding_ui_window = nullptr;

	UI_Window* player_abilities = false;
	UI_Window* inventory = nullptr;
	iPoint barracks_position;

	UI_Button* item_drop = nullptr;

	//buttons for brokenbuilding
	UI_Button* create_building_button = nullptr;
	UI_Button* create_building_button2 = nullptr;
	UI_Button* create_building_button3 = nullptr;

	bool blacksmith_alive = false;

	UI_Button* choose_ability_b = nullptr;
	UI_Button* choose_ability_uw = nullptr;

	UI_Text* choose_ability_b_txt = nullptr;
	UI_Text* choose_ability_uw_txt = nullptr;

private:
	//buttons for barracks
	UI_Button* barbarian_img = nullptr;
	UI_Button* swordsman_img = nullptr;
	UI_Button* create_unit_button = nullptr;
	UI_Button* create_unit_button2 = nullptr;

	//buttons for brokenbuilding
	UI_Button* barrack_img = nullptr;
	UI_Button* house_img = nullptr;
	UI_Button* blacksmith_img = nullptr;

	//buttons for abilities
	UI_Button* battlecry_ability = nullptr;
	UI_Button* whirlwind_ability = nullptr;
	UI_Button* charge_ability = nullptr;

	bool clicked_charge_1 = false;
	bool clicked_charge_2 = false;

	bool changing_ability = false;

public:
	bool audio_status = false;
	bool change_controls_status = false;
	bool options_status = false;
	bool pause_status = false;
	bool button_on_clicked = false;
	bool victory_status = false;
	bool lose_status = false;
	bool create_barbarian = true;
	bool create_swordsman = false;

	bool change_button_config;

	//player abilities
	bool draw_battlecry_range = false;
	bool draw_whirlwind_range = false;
	bool draw_charge_range = false;
	bool draw_buff = false;
	bool charge_speed_buff = false;
	bool charge_damage_buff = false;

	std::list<iPoint> range_visited;

	first_ability active_ability = not_chosen;
	bool undying_state_active = false;

	bool audio_muted = false;

private:
	j1Timer battlecry_timer;
	j1Timer undying_will_timer;
	j1Timer whirlwind_timer;
	j1Timer charge_timer;
	j1Timer button_clicked;
};


#endif // !_PLAYER_H_
