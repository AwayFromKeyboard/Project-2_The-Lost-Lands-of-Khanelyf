#ifndef _MINIMAP_H_
#define _MINIMAP_H_

#include "SDL/include/SDL.h"
#include "j1Module.h"
#include "Point.h"

struct QuadColor {
	QuadColor() {}
	QuadColor(uint _r, uint _g, uint _b, uint _a) : r(_r), g(_g), b(_b), a(_a) {}
	
	uint r = 0;
	uint g = 0;
	uint b = 0;
	uint a = 255;
};

class Minimap : public j1Module
{
public:
	Minimap();
	virtual ~Minimap();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	iPoint WorldToMinimap(const iPoint & world_pos) const;

	void DrawMinimap();

private:
	QuadColor enemies; // red
	QuadColor allies; // green 
	QuadColor buildings; // blue

public:
	SDL_Rect rect = NULLRECT;
	iPoint scale = NULLPOINT;
};

#endif // !_MINIMAP_H_

