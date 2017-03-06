#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "Point.h"
#include "j1Module.h"
#include <string>
#include <list>

#define XOFFSET 20
#define YOFFSET 20

// ----------------------------------------------------
struct Properties
{
	struct Property
	{
		std::string name;
		int value = 0;
	};

	~Properties()
	{
		Property* item = nullptr;
		item = list.front();

		for (std::list<Property*>::iterator prop = list.begin(); prop != list.end(); ++prop)
		{
			RELEASE(*prop);
		}

		list.clear();
	}

	int Get(const char* name, int default_value = 0) const;

	std::list<Property*>	list;
};

// ----------------------------------------------------
struct MapLayer
{
	std::string	name;
	int			width = 0;
	int			height = 0;
	uint*		data = 0;
	Properties	properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y*width) + x];
	}
};

// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	std::string			name;
	int					firstgid = 0;
	int					margin = 0;
	int					spacing = 0;
	int					tile_width = 0;
	int					tile_height = 0;
	SDL_Texture*		texture = nullptr;
	int					tex_width = 0;
	int					tex_height = 0;
	int					num_tiles_width = 0;
	int					num_tiles_height = 0;
	int					offset_x = 0;
	int					offset_y = 0;
};

enum map_types
{
	maptype_unknown = 0,
	maptype_orthogonal,
	maptype_isometric,
	maptype_staggered
};
// ----------------------------------------------------
struct MapData
{
	int					width = 0;
	int					height = 0;
	int					tile_width = 0;
	int					tile_height = 0;
	SDL_Color			background_color = NULLRECT;
	map_types			type = map_types::maptype_unknown;
	std::list<TileSet*>	tilesets;
	std::list<MapLayer*>layers;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;
	iPoint WorldToMapPoint(iPoint position) const;
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	void TilesToDraw_x(int& x_ini, int& x_end, MapLayer* layer) const;
	void TilesToDraw_y(int count, int x, int x_end, int& y_ini, int& y_end, MapLayer* layer) const;

	TileSet* GetTilesetFromTileId(int id) const;

public:

	MapData data;
	SDL_Rect fit_square = NULLRECT;

private:

	pugi::xml_document	map_file;
	std::string			folder;
	bool				map_loaded = false;
	int					draw_margin = 0;
	int					offset = 0;
};

#endif // __j1MAP_H__