#ifndef _SCENE_
#define _SCENE_

#include "j1App.h"
#include "j1Render.h"
#include "PugiXml\src\pugixml.hpp"
#include <string>
#include <list>

class UI_Window;
class UI_ColoredRect;

//class PhysBody;
class Scene
{
public:
	Scene() {};

	virtual ~Scene() {};

	virtual bool Init() { return true; };
	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	virtual void OnColl(Collider* c1, Collider* c2) {};

	virtual void OnCommand(std::list<std::string>& tokens) {}

	virtual void OnCVar(std::list<std::string>& tokens) {}

	virtual void SaveCVar(std::string& cvar_name, pugi::xml_node& node) const {}

public:
	void InitCameraMovement(); // Init windows to move the camera
	void UpdateCameraMovement(); // Look if enters any condition to move the camera 
private:
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
};

#endif