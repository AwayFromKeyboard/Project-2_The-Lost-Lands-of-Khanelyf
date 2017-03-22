#ifndef _SceneTest_H_
#define _SceneTest_H_

#include "Scene.h"
#include "j1Render.h"
#include "j1Gui.h"

class GameObject;
class b2Fixture;
class Parallax;
class Hero;

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

	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

	void CheckUnitCreation(iPoint p);

private:
	UI_Window* cursor_window = nullptr;
	UI_Image* cursor = nullptr;

	UI_Window* general_ui_window = nullptr;
	UI_Image* general_ui_image = nullptr;

	

	Hero* troop = nullptr;

	SDL_Rect cursor_r;
	SDL_Rect ui_r;

private:
	int gold = 0;
	UI_Text* gold_txt = nullptr;
	SDL_Texture* debug_tex = nullptr;
};

#endif // _SceneTest_H_
