#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "j1Timer.h"

#include "Point.h"

#define DURATION_BATTLECRY 5
#define COOLDOWN_BATTLECRY 10
#define BATTLECRY_BUFF 5
#define BATTLECRY_RANGE 10

class Unit;
class UI_Window;
class UI_Text;
class UI_Button;
class Hero;

class Player : public j1Module {
public:
	Player();
	~Player();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void Battlecry(int modifier, int range);
	void BattlecryModifier(int damage_buff);
	void CheckBattlecryRange(int range);
	void DrawBuff();
	void StopBuff(int modifier);
	void DrawCD(int ability_number);

private:
	UI_Window* attributes_window = nullptr;
	UI_Text* life_txt = nullptr;
	UI_Text* damage_txt = nullptr;
	UI_Text* armor_txt = nullptr;
	UI_Text* pierce_armor_txt = nullptr;

	UI_Text* battlecry_cd = nullptr;

	UI_Window* levelup_window = nullptr;
	UI_Button* life_button = nullptr;
	UI_Button* damage_button = nullptr;
	UI_Button* armor_button = nullptr;
	UI_Button* pierce_armor_button = nullptr;
	UI_Text* level_points_txt = nullptr;

	Hero* hero = nullptr;
private:
	void MoveToTile(iPoint tile);
	void SetAttackingEnemy(Unit* enemy);
	void UpdateAttributes();
public:
	void SetHero(Hero* hero);
	Hero* GetHero();

	std::list<Unit*> buffed_list;
public:
	UI_Window* barracks_ui_window = nullptr;
	UI_Window* player_abilities = false;
	iPoint barracks_position;

private:
	UI_Button* barbarian_img = nullptr;
	UI_Button* swordsman_img = nullptr;
	UI_Button* create_unit_button = nullptr;
	UI_Button* create_unit_button2 = nullptr;

	//buttons for abilities
	UI_Button* battlecry_ability = nullptr;

public:
	bool create_barbarian = true;
	bool create_swordsman = false;

	//player abilities
	bool battlecry_state = false;
	bool draw_battlecry_range = false;
	bool draw_buff = false;

private:
	j1Timer battlecry_timer;
};


#endif // !_PLAYER_H_
