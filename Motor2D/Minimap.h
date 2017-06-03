#ifndef HUD_MINIMAP_PANEL
#define HUD_MINIMAP_PANEL

#include "Defs.h"
#include "Point.h"
#include "j1Module.h"
#include <vector>
#include <list>
#include "PugiXml\src\pugixml.hpp"

#include "j1Timer.h"
#include "SDL\include\SDL_rect.h"
#include "SDL_image\include\SDL_image.h"

class UI_Window;
class UI_Image;

#define MINIMAP_UPDATE_RATE		1000
#define MINIMAP_TIME_TO_UPDATE	1

struct minimap_cell
{
	SDL_Color cell_color = { 255,255,255,255 };
	iPoint    cell_position = { 0,0 };

	bool operator==(const minimap_cell& target)
	{
		if (cell_position == target.cell_position) return true;
		else return false;
	}
};

class Minimap : public j1Module
{
public:
	Minimap();
	~Minimap();

	bool CleanUp();
	bool Start();
	bool PreUpdate();
	bool PostUpdate();
	bool Draw();

	// Handle_input_GUI
	void Handle_Input();

	// Mouse Over
	bool IsMouseOver();

	// Disable/Enable Module
	void Enable();
	void Disable();

	// MiniMap Input Funtionality ------------------
	void MoveCameraToPoint(int x = 0, int y = 0);
	void MoveUnitsToPoint(int x = 0, int y = 0);

	// Minimap to Map 
	bool MiniMToMap(int& x, int& y);
	iPoint MiniMToScreen(int x, int y);

public:

	//HUD Panels
	UI_Window*	minimap_window = nullptr;

public:
	SDL_Rect    map_rect = { 0,0,0,0 };
	iPoint		minimap_quad = NULLPOINT;

private:
	// Map Data
	bool		in_minimap = false;
	SDL_Rect	minimap_size = { 0,0,0,0 };
	fPoint		half_tile_size = { 0.0f,0.0f };
	iPoint		map_size = NULLPOINT;

	// Map Cells
	std::vector<minimap_cell>	cells;
	std::vector<minimap_cell>	units_to_print;

	j1Timer			update_timer;
};

#endif 
