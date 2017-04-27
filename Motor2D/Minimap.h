#ifndef _MINIMAP_H_
#define _MINIMAP_H_

#include "j1Module.h"

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

private:
	QuadColor enemies; // red
	QuadColor allies; // green 
	QuadColor buildings; // blue
};

#endif // !_MINIMAP_H_

