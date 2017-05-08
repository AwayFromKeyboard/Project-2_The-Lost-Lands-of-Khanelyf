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

#define COOLDOWN_WHIRLWIND 8
#define WHIRLWIND_DAMAGE 40
#define WHIRLWIND_RANGE 2

#define COOLDOWN_CHARGE 12
#define CHARGE_DAMAGE 40
#define CHARGE_RANGE 5
#define CHARGE_SPEED 5

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
public:
	UI_Window* pause_window = nullptr;
	UI_Button* quit_game = nullptr;
private:
	
	UI_Image* pause_bg = nullptr;
	UI_Button* back = nullptr;
	UI_Button* options = nullptr;
	UI_Button* save = nullptr;
	UI_Text* pause_menu_txt = nullptr;
	UI_Text* quit_txt = nullptr;
	UI_Text* save_txt = nullptr;
	UI_Text* options_txt = nullptr;
	UI_Text* back_txt = nullptr;

	UI_Window* attributes_window = nullptr;
	UI_Text* life_txt = nullptr;
	UI_Text* damage_txt = nullptr;
	UI_Text* armor_txt = nullptr;
	UI_Text* pierce_armor_txt = nullptr;

	UI_Text* battlecry_cd = nullptr;
	UI_Text* whirlwind_cd = nullptr;
	UI_Text* charge_cd = nullptr;

	UI_Text* battlecry_key = nullptr;
	UI_Text* whirlwind_key = nullptr;
	UI_Text* charge_key = nullptr;

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
	UI_Window* player_abilities = false;
	UI_Window* inventory = nullptr;
	iPoint barracks_position;

	UI_Button* item_drop = nullptr;

private:
	UI_Button* barbarian_img = nullptr;
	UI_Button* swordsman_img = nullptr;
	UI_Button* create_unit_button = nullptr;
	UI_Button* create_unit_button2 = nullptr;

	//buttons for abilities
	UI_Button* battlecry_ability = nullptr;
	UI_Button* whirlwind_ability = nullptr;
	UI_Button* charge_ability = nullptr;

public:
	bool pause_status = false;
	bool button_on_clicked = false;

	bool create_barbarian = true;
	bool create_swordsman = false;

	//player abilities
	bool draw_battlecry_range = false;
	bool draw_whirlwind_range = false;
	bool draw_charge_range = false;
	bool draw_buff = false;
	bool charge_speed_buff = false;
	bool charge_damage_buff = false;

	std::list<iPoint> range_visited;

private:
	j1Timer battlecry_timer;
	j1Timer whirlwind_timer;
	j1Timer charge_timer;
	j1Timer button_clicked;
};


#endif // !_PLAYER_H_
