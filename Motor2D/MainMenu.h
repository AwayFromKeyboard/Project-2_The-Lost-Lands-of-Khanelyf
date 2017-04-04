#ifndef _MainMenu_H_
#define _MainMenu_H_

#include "Scene.h"
#include "j1Render.h"
#include "j1Gui.h"

class GameObject;
class b2Fixture;
class Parallax;
class Hero;

#define TROOP_PRICE 20
#define TROOP_OFFSET 1

class MainMenu : public Scene
{
public:
	MainMenu();
	~MainMenu();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void OnColl(Collider* c1, Collider* c2);

	void CheckUnitCreation(iPoint p);

private:
	UI_Window* cursor_window = nullptr;
	UI_Image* cursor = nullptr;

	UI_Window* main_menu_window = nullptr;
	UI_Image* main_menu_image = nullptr;

	SDL_Rect cursor_r;
	SDL_Rect ui_r;

private:
	SDL_Texture* debug_tex = nullptr;
};

#endif // _MainMenu_H_
