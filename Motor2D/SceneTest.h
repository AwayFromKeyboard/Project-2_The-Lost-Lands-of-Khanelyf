#ifndef _SceneTest_H_
#define _SceneTest_H_

#include "Scene.h"
#include "j1Render.h"
#include "j1Gui.h"

class GameObject;
class b2Fixture;
class Parallax;
class Test;

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

private:
	UI_Window* cursor_window = nullptr;
	UI_Image* cursor = nullptr;

	UI_Window* movement_window_u = nullptr;
	UI_Window* movement_window_d = nullptr;
	UI_Window* movement_window_l = nullptr;
	UI_Window* movement_window_r = nullptr;
	UI_Window* movement_window_u_l = nullptr;
	UI_Window* movement_window_u_r = nullptr;
	UI_Window* movement_window_d_l = nullptr;
	UI_Window* movement_window_d_r = nullptr;

	UI_ColoredRect* rect_u = nullptr;
	UI_ColoredRect* rect_d = nullptr;
	UI_ColoredRect* rect_l = nullptr;
	UI_ColoredRect* rect_r = nullptr;
	UI_ColoredRect* rect_u_l = nullptr;
	UI_ColoredRect* rect_u_r = nullptr;
	UI_ColoredRect* rect_d_l = nullptr;
	UI_ColoredRect* rect_d_r = nullptr;

	Test* troop = nullptr;

	SDL_Rect cursor_r;
};

#endif // _SceneTest_H_