#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "j1Module.h"
#include "SDL/include/SDL.h"

#include "Point.h"

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

private:
	UI_Window* attributes_window = nullptr;
	UI_Text* life_txt = nullptr;
	UI_Text* damage_txt = nullptr;
	UI_Text* armor_txt = nullptr;
	UI_Text* pierce_armor_txt = nullptr;

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
public:
	UI_Window* barracks_ui_window = nullptr;
	iPoint barracks_position;

private:
	UI_Button* barbarian_img = nullptr;
	UI_Button* swordsman_img = nullptr;
	UI_Button* create_unit_button = nullptr;
	UI_Button* create_unit_button2 = nullptr;

	bool create_barbarian = false;
	bool create_swordsman = false;

};


#endif // !_PLAYER_H_
