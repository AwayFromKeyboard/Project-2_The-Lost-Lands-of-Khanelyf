#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Gui.h"
#include "Scene.h"
#include <queue>

struct SDL_Texture;
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

	int layer;
};
struct layer_quad
{
	layer_quad() {};
	layer_quad(const SDL_Rect _rect, Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a, bool _filled, bool _use_camera)
	{
		rect = { _rect }; r = _r; g = _g; b = _b; a = _a; filled = _filled; use_camera = _use_camera;
	}
	SDL_Rect rect = NULLRECT;
	Uint8    r = 0;
	Uint8    g = 0;
	Uint8    b = 0;
	Uint8    a = 0;
	bool     filled = false;
	bool     use_camera = true;

	int layer;
};

struct layer_line
{
	layer_line() {};
	layer_line(int _x1, int _y1, int _x2, int _y2, Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a, bool _use_camera)
	{
		x1 = _x1; y1 = _y1; x2 = _x2; y2 = _y2; r = _r; g = _g; b = _b; a = _a; use_camera = _use_camera;
	}
	int   x1 = 0;
	int   y1 = 0;
	int   x2 = 0;
	int   y2 = 0;
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint8 a = 255;
	bool  use_camera = true;

	int layer;
};

struct layer_circle
{
	layer_circle() {};
	layer_circle(int _x1, int _y1, int _redius, Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a, bool _filled, bool _use_camera)
	{
		x1 = _x1; y1 = _y1; redius = _redius; r = _r; g = _g; b = _b; a = _a; filled = _filled, use_camera = _use_camera;
	}
	int   x1 = 0;
	int   y1 = 0;
	int   redius = 0;
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint8 a = 0;
	bool filled = false;
	bool  use_camera = true;

	int layer;
};

struct compare_layer {
	bool operator()(layer_blit const& l1, layer_blit const& l2) {
		return l1.layer < l2.layer;
	}
};

struct compare_layer_quad {
	bool operator()(layer_quad const& l1, layer_quad const& l2) {
		return l1.layer < l2.layer;
	}
};

struct compare_layer_line {
	bool operator()(layer_line const& l1, layer_line const& l2) {
		return l1.layer < l2.layer;
	}
};

struct compare_layer_circle {
	bool operator()(layer_circle const& l1, layer_circle const& l2) {
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
	Scene* GetCurrentScene();

	// Blit choosing the layer
	void LayerBlit(int layer, SDL_Texture* texture, iPoint pos, const SDL_Rect section = NULLRECT, float scale = -1.0f, SDL_RendererFlip _flip = SDL_FLIP_NONE,  double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX);
	void LayerDrawQuad(const SDL_Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, int layer = 0, bool use_camera = true);
	void LayerDrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, int layer = 0, bool use_camera = true);
	void LayerDrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, int layer = 0, bool filled = false, bool use_camera = true);

	void OnCollision(Collider* col1, Collider* col2);

private:
	// Blit in the layer order
	void DoLayerBlit();

public:
	// Scenes
	SceneTest*		     scene_test = nullptr;
	Scene*				 play_scene = nullptr;
private:
	// Layer Blit list
	std::priority_queue<layer_blit, std::vector<layer_blit>, compare_layer> layer_list;
	std::priority_queue<layer_blit, std::vector<layer_blit>, compare_layer_quad> quad_list;
	std::priority_queue<layer_blit, std::vector<layer_blit>, compare_layer_line> line_list;
	std::priority_queue<layer_blit, std::vector<layer_blit>, compare_layer_circle> circle_list;

	// Scenes list
	list<Scene*>         scenes;

	// Current scene
	Scene*               current_scene = nullptr;

};

#endif // __j1SCENE_H__