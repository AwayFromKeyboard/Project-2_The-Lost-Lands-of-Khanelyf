#include "Minimap.h"

Minimap::Minimap()
{
	name = "Minimap";
}

Minimap::~Minimap()
{
}

bool Minimap::Start()
{
	bool ret = true;
	
	QuadColor red(255, 0, 0, 255);
	enemies = red;
	QuadColor green(0, 255, 0, 255);
	allies = green;
	QuadColor blue(0, 0, 255, 255);
	buildings = blue;

	return ret;
}

bool Minimap::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool Minimap::Update(float dt)
{
	bool ret = true;

	return ret;
}

bool Minimap::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool Minimap::CleanUp()
{
	bool ret = true;

	return ret;
}
