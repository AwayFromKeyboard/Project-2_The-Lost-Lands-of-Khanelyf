#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Gui.h"
#include "Scene.h"
#include <queue>

struct SDL_Texture;
class MainMenu;
class MainScene;
class SceneTest;

// LayerBlit struct
struct layer_blit
{
	layer_blit() {};

	layer_blit(SDL_Texture* _texture, iPoint _pos, const SDL_Rect _section, float _scale, SDL_RendererFlip _flip, double _angle, int _pivot_x, int _pivot_y, int _layer = 0)
	{
		texture = _texture; 
		pos = _pos; 
		section.x = _section.x;  section.y = _section.y; section.w = _section.w; section.h = _section.h;
		scale = _scale;
		flip = _flip;
		angle = _angle;
		pivot_x = _pivot_x;
		pivot_y = _pivot_y;
		layer = _layer;
	};

	SDL_Texture*	 texture = nullptr;
	iPoint			 pos = NULLPOINT; 
	SDL_Rect		 section = NULLRECT;
	float			 scale = 0.0f;
	SDL_RendererFlip flip;
	double           angle = 0; 
	int              pivot_x = 0;
	int              pivot_y = 0;

	int layer = 0;
};

struct compare_layer {
	bool operator()(layer_blit const& l1, layer_blit const& l2) {
		return l1.layer < l2.layer;
	}
};

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Change scene
	void ChangeScene(Scene* new_scene);

	// Get current scene
	Scene* GetCurrentScene();

	// Blit choosing the layer
	void LayerBlit(int layer, SDL_Texture* texture, iPoint pos, const SDL_Rect section = NULLRECT, float scale = -1.0f, SDL_RendererFlip _flip = SDL_FLIP_NONE,  double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX);
	
	void OnCollision(Collider* c1, Collider* c2);

private:
	// Blit in the layer order
	void DoLayerBlit();

public:
	// Scenes
	MainMenu*			 main_menu = nullptr;
	MainScene*           main_scene = nullptr;
	SceneTest*		     scene_test = nullptr;
private:
	// Layer Blit list
	std::priority_queue<layer_blit, std::vector<layer_blit>, compare_layer> layer_list;

	// Scenes list
	list<Scene*>         scenes;

	// Current scene
	Scene*               current_scene = nullptr;

};

#endif // __j1SCENE_H__