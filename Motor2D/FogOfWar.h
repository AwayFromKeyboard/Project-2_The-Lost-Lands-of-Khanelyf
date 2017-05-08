#ifndef _FOGOFWAR_H_
#define	_FOGOFWAR_H_

#include <list>
#include <vector>
#include "Unit.h"

#define FOW_RADIUM 4

// This is not necessary, but will make a more readable code and will simplify stuff
enum fow_id
{
	// Dim 

	fow_null,
	dim_middle,
	dim_left,
	dim_right,
	dim_up,
	dim_down,
	dim_bottom_right,
	dim_bottom_left,
	dim_top_right,
	dim_top_left,
	dim_inner_top_left,
	dim_inner_top_right,
	dim_inner_bottom_left,
	dim_inner_bottom_right,

	// Dark->dim 

	darkd_middle,
	darkd_left,
	darkd_right,
	darkd_up,
	darkd_down,
	darkd_bottom_right,
	darkd_bottom_left,
	darkd_top_right,
	darkd_top_left,
	darkd_inner_top_left,
	darkd_inner_top_right,
	darkd_inner_bottom_left,
	darkd_inner_bottom_right,

	// Dark->clear

	darkc_middle,
	darkc_left,
	darkc_right,
	darkc_up,
	darkc_down,
	darkc_bottom_right,
	darkc_bottom_left,
	darkc_top_right,
	darkc_top_left,
	darkc_inner_top_left,
	darkc_inner_top_right,
	darkc_inner_bottom_left,
	darkc_inner_bottom_right,



	// No mask 

	dim_clear,

};

struct ally_unit
{
	list<iPoint>	frontier;
	list<iPoint>	current_points;
	iPoint			player_pos;
	int				id = -1;
};

struct simple_unit
{
	iPoint			player_pos = NULLPOINT;
	bool			visible = false;
	int				id = -1;
	entity_type		type = null;
};

class FogOfWar
{
public:

	FogOfWar();
	~FogOfWar();

	void Start();

	void Update(iPoint prev_pos, iPoint next_pos, uint entityID);

	bool AddPlayer(Unit* new_entity);

	uint RemoveDimJaggies();
	void RemoveDarkJaggies();

	// Update Methods

	void MoveFrontier(iPoint prev_pos, const char* direction, uint entityID);

	void MoveArea(ally_unit& player, string direction, uint entityID);

	SDL_Rect GetRect(int fow_id);

	// Create Areas ---

	void FillFrontier();

	void GetEntitiesCircleArea(ally_unit& new_player);
	void GetEntitiesRectangleFrontier(ally_unit& new_player, int with, int height);

	void DeletePicks(ally_unit& frontier);
	void GetCurrentPointsFromFrontier(ally_unit& player);

	// Characters --

	void ManageCharacters();

	// Utilility --

	bool IsVisible(iPoint char_pos, entity_type type);

	bool IsFrontier(iPoint point, ally_unit& player);

	uint Get(int x, int y);

	// Atlas ---

	SDL_Texture*				fog_of_war_texture;
	list<simple_unit>			simple_char_on_fog_pos;

private:

	vector<ally_unit>			players_on_fog;
	uint*						data = nullptr;




};

#endif
