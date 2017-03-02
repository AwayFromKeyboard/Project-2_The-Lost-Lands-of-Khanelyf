#ifndef _SceneMenu_H_
#define _SceneMenu_H_

#include "Scene.h"
#include "j1Render.h"
#include "j1Gui.h"

class GameObject;
class b2Fixture;

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

private:
	UI_Window* cursor_window = nullptr;
	UI_Image* cursor = nullptr;

	UI_Window* general_ui_window = nullptr;
	UI_Image* general_ui_image = nullptr;

	UI_Button* single_player_button = nullptr;

	SDL_Rect cursor_r;
	SDL_Rect ui_r;

private:
	void CheckSinglePlayerButton();
};

#endif // _SceneMenu_H_