#ifndef _CUTSCENEMANAGER_H_
#define _CUTSCENEMANAGER_H_

#include "j1Module.h"
#include <string>
#include "j1Render.h"
#include "BezierEasing.h"
#include "j1Timer.h"

class Entity;
class UI_Text;
class Scene;

enum elements_groups
{
	e_g_map,
	e_g_image,
	e_g_entity,
	e_g_music,
	e_g_sound_effect,
	e_g_text,
	e_g_null
};

//---------------------
// Elements
//---------------------

class CutsceneElement
{
public:

	CutsceneElement(elements_groups group, const char* path, const char* name, bool active = true);
	virtual ~CutsceneElement() {};

public:
	string			path;
	bool			active = false;
	elements_groups group = e_g_null;
	int				id = -1;
	string			name;
};

class CutsceneMap : public CutsceneElement
{
public:
	CutsceneMap(elements_groups group, const char* path, const char* name, bool active = true);
};

class CutsceneImage : public CutsceneElement
{
public:
	CutsceneImage(elements_groups group, const char* path, const char* name, bool active = true, iPoint pos = { 0,0 }, SDL_Rect rect = { 0,0,0,0 }, int layer = 0);

	SDL_Texture* GetTexture()const;
	SDL_Rect GetRect()const;
	iPoint GetPos()const;
	int	GetLayer()const;

	void Move(float x, float y);
	void ChangeTex(const char* path);
	void ChangeRect(SDL_Rect r);

private:
	SDL_Texture* texture = nullptr;
	SDL_Rect	 rect = { 0,0,0,0 };
	iPoint		 pos = NULLPOINT;
	int			 layer = 0;

};

class CutsceneEntity : public CutsceneElement
{
public:
	CutsceneEntity(elements_groups group, const char* path, const char* name, bool active = true, iPoint pos = { 0,0 });
	~CutsceneEntity();

	Entity* GetEntity()const;
	void SetNull();
	void SetEntity(Entity* e);

private:
	Entity*	entity;
};

class CutsceneMusic : public CutsceneElement
{
public:
	CutsceneMusic(elements_groups group, const char* path, const char* name, bool active = true);
	~CutsceneMusic();

	bool IsPlaying()const;

	void Play();

private:
	bool playing = false;
};

class CutsceneSoundEffect : public CutsceneElement
{
public:
	CutsceneSoundEffect(elements_groups group, const char* path, const char* name, bool active = true, int loops = 0);
	~CutsceneSoundEffect();

	int GetID()const;
	int GetLoops()const;

	void Play();

private:
	int loops = 0;
	int id = 0;
};

class CutsceneText : public CutsceneElement
{
public:
	CutsceneText(elements_groups group, const char* path, const char* name, bool active = true, iPoint pos = { 0,0 });
	~CutsceneText();

	void SetText(const char* txt);
	UI_Text* GetText()const;
	void Move(float x, float y);

private:
	UI_Text* text = nullptr;
};

//---------------------
// Actions
//---------------------

enum actions
{
	a_move,
	a_action,
	a_play,
	a_stop,
	a_modify,
	a_enable,
	a_disable,
	a_change_scene,
	a_null,
};

class CutsceneAction
{
public:
	CutsceneAction(actions action, const char* name, int start_time, int duration = 0);

public:
	string	element_name;
	actions action = a_null;
	int		start = -1;
	int		duration = 0;
};

class CutSceneAction_Cmp
{
public:
	CutSceneAction_Cmp();

	bool operator()(CutsceneAction*& a1, CutsceneAction*& a2)
	{
		return a1->start > a2->start;
	}
};

enum reference_type
{
	r_t_local,
	r_t_global,
	r_t_map,
	r_t_null,
};

class CutsceneMove : public CutsceneAction
{
public:
	CutsceneMove(actions action, const char* name, int start_time, int duration = 0);
	~CutsceneMove();

public:
	BezierEasing*	bezier = nullptr;
	iPoint			dest = NULLPOINT;
	reference_type	reference = r_t_null;
	iPoint			initial_pos = NULLPOINT;
	bool			first_time = true;

};

enum entity_actions
{
	e_a_kill,
	e_a_spawn,
	e_a_change_pos,
	e_a_null
};

class CutsceneModifyEntity : public CutsceneAction
{
public:
	CutsceneModifyEntity(actions action, const char* name, int start_time, int duration = 0);

public:
	entity_actions entity_action = e_a_null;
	iPoint pos = NULLPOINT;

};

class CutsceneModifyImage : public CutsceneAction
{
public:
	CutsceneModifyImage(actions action, const char* name, int start_time, int duration = 0);

public:
	string	 var;
	string	 path;
	SDL_Rect rect = NULLRECT;

};

class CutsceneModifyText : public CutsceneAction
{
public:
	CutsceneModifyText(actions action, const char* name, int start_time, int duration = 0);

public:
	string txt;

};

class CutsceneModifyCamera : public CutsceneAction
{
public:
	CutsceneModifyCamera(actions action, const char* name, int start_time, int duration = 0);

public:
	iPoint position;

};

enum change_scene_effects
{
	c_s_e_fade,
	c_s_e_circle,
	c_s_e_star,
	c_s_e_null,
};

class CutsceneChangeScene : public CutsceneAction
{
public:
	CutsceneChangeScene(actions action, const char* name, int start_time, int duration = 0);

public:
	string					path;
	change_scene_effects	effect = c_s_e_null;

};

//---------------------
// Manager
//---------------------

class j1CutSceneManager : public j1Module
{
public:
	j1CutSceneManager();
	~j1CutSceneManager();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	int GetNextID() const;

	void Play(const char* path, Scene* next_scene = nullptr);
	bool HasFinished()const;

private:

	void Load(const char* path);

	elements_groups GetElementGroup(const char* ele)const;
	CutsceneElement* GetElement(const char* ele)const;

	void PerformActions(float dt);

	void ChangeScene();

	void ClearScene();

	// Load elements
	void LoadMap(pugi::xml_node& node);
	void LoadCSImage(pugi::xml_node& node);
	void LoadEntity(pugi::xml_node& node);
	void LoadMusic(pugi::xml_node& node);
	void LoadSoundEffect(pugi::xml_node& node);
	void LoadText(pugi::xml_node& node);

	// Update Elements
	void UpdateElements(float dt);

	//Load Actions
	void LoadAction(pugi::xml_node& node);
	void LoadMove(pugi::xml_node& node);
	void LoadModify(pugi::xml_node& node);
	void LoadChangeScene(pugi::xml_node& node);

	//Perform Actions
	void PerformMove(CutsceneElement* ele, CutsceneMove* move);
	void PerformAction(CutsceneElement* ele);
	void PerformPlay(CutsceneElement* ele);
	void PerformStop(CutsceneElement* ele);
	void PerformModify(CutsceneElement* ele, CutsceneAction* act);
	void PerformEnable(CutsceneElement* ele);
	void PerformDisable(CutsceneElement* ele);
	void PerformChangeScene(CutsceneAction* act);
	void MoveCamera(CutsceneMove* move);

public:
	UI_Window*					gui_win = nullptr;

private:

	std::list<CutsceneElement*>																elements;

	std::priority_queue<CutsceneAction*, std::vector<CutsceneAction*>, CutSceneAction_Cmp>	remaining_actions;
	std::list<CutsceneAction*>																active_actions;

	j1Timer																					scene_timer;

	//Change scene
	j1Timer																					change_scene_timer;
	bool																					change_scene = false;
	int																						change_scene_duration = 0;
	change_scene_effects																	change_scene_effect = c_s_e_null;
	string																					new_scene;
	bool																					changed = false;

	SDL_Texture*																			circle_mask_tex = nullptr;
	SDL_Texture*																			star_mask_tex = nullptr;

	bool																					finished = false;
	Scene*																					next_scene = nullptr;
};


#endif