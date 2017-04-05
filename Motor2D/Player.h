#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "j1Module.h"
#include "SDL/include/SDL.h"

#include "Point.h"

class Unit;
class UI_Window;
class UI_Button;

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
	void MoveToTile(iPoint tile);
	void SetAttackingEnemy(Unit* enemy);

public:
	UI_Window* barracks_ui_window = nullptr;
	iPoint barracks_position;

private:
	UI_Button* create_unit_button = nullptr;
	UI_Button* create_unit_button2 = nullptr;

	bool create_barbarian = false;
	bool create_swordsman = false;

};


#endif // !_PLAYER_H_
