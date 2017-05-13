#ifndef _MainMenu_H_
#define _MainMenu_H_

#include "Scene.h"
#include "j1Render.h"
#include "j1Gui.h"

class GameObject;
class b2Fixture;
class Parallax;

class MainMenu : public Scene
{
public:
	MainMenu();
	~MainMenu();

	bool Init();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void OnColl(Collider* c1, Collider* c2);

private:
	UI_Window* cursor_window = nullptr;
	UI_Image* cursor = nullptr;

	UI_Window* main_menu_window = nullptr;
	UI_Image* main_menu_image = nullptr;

	UI_Button* start_button = nullptr;
	UI_Image* start_button_image = nullptr;
	UI_Image* start_button_text = nullptr;

	SDL_Rect cursor_r;
	SDL_Rect ui_r;
	SDL_Rect start_r;

private:
	SDL_Texture* debug_tex = nullptr;
};

#endif // _MainMenu_H_
