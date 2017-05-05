#include "Minimap.h"
#include "j1Scene.h"
#include "Entity.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Input.h"
#include "Player.h"

Minimap::Minimap()
{
	name = "Minimap";
}

Minimap::~Minimap()
{
	CleanUp();
}

bool Minimap::Start() 
{
	map_rect = { 150,150,273,139 };

	minimap_window = (UI_Window*)App->gui->UI_CreateWin({ 150, 150 }, 273, 139, 8); // 1388 190
	minimap_background = (UI_Image*)minimap_window->CreateImage({ 150, 150 }, { 1237, 810, 273, 139 });

	map_size = { 120, 110 };

	half_tile_size.x = ((float)(map_rect.w) / map_size.x)* 0.5f;
	half_tile_size.y = ((float)(map_rect.h + 2) / map_size.y)* 0.5f;

	int size = (map_size.x * map_size.y);

	minimap_cell new_cell;
	new_cell.cell_position = { map_rect.x + map_rect.w / 2, map_rect.y };

	iPoint pos;
	cells.reserve(size);
	for (int i = 0; i < size; i++)
	{
		pos = MiniMToScreen(i%map_size.x, i / map_size.y);
		cells.push_back(new_cell);
		cells[i].cell_position.x += pos.x;
		cells[i].cell_position.y += pos.y;
	}

	update_timer.Start();
}

bool Minimap::PreUpdate()
{
	bool ret = true;

	if (update_timer.Read() > MINIMAP_UPDATE_RATE)
	{
		units_to_print.clear();
		units_to_print.reserve(App->entity->entity_list.size());

		iPoint pos = { 0,0 };
		for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++)
		{
			pos = App->map->WorldToMap((*it)->position.x, (*it)->position.y);
			switch ((*it)->GetType())
			{
			case player:
				cells[map_rect.w * pos.y + pos.x].cell_color = { 20,20,255,255 };
				units_to_print.push_back(cells[map_rect.w * pos.y + pos.x]);
				break;
			case ally:
				cells[map_rect.w  * pos.y + pos.x].cell_color = { 20,255,20,255 };
				units_to_print.push_back(cells[map_rect.w * pos.y + pos.x]);
				break;
			case npc:
				break;
			case enemy:
				cells[map_rect.w  * pos.y + pos.x].cell_color = { 255,20,20,255 };
				units_to_print.push_back(cells[map_rect.w * pos.y + pos.x]);
				break;
			case building:
				
				break;
			case object:
				break;
			case ally_building:
				cells[map_rect.w  * pos.y + pos.x].cell_color = { 255,255,20,255 };
				units_to_print.push_back(cells[map_rect.w * pos.y + pos.x]);
				break;
			case enemy_building:
				cells[map_rect.w  * pos.y + pos.x].cell_color = { 20,255,255,255 };
				units_to_print.push_back(cells[map_rect.w * pos.y + pos.x]);
				break;
			case null:
				break;
			}
		}
		update_timer.Start();

	}

	return ret;
}

bool Minimap::PostUpdate()
{
	bool ret = true;

	Handle_Input();
	Draw();

	return ret;
}

bool Minimap::Draw()
{
	bool ret = true;

	//App->render->DrawQuad(map_rect, 0, 0, 0, 255, true, false);
	App->scene->LayerDrawQuad(map_rect, 0, 0, 0, 255, true, false, 10);

	// Draw Units in Minimap
	SDL_Color color;
	int size = units_to_print.size();
	for (int i = 0; i < size; i++)
	{
		color = units_to_print[i].cell_color;
		
		App->scene->LayerDrawQuad({ units_to_print[i].cell_position.x, units_to_print[i].cell_position.y,3,3 }, color.r, color.g, color.b, color.a, true, false, 10);
		//App->render->DrawQuad({ units_to_print[i].cell_position.x, units_to_print[i].cell_position.y,3,3 }, color.r, color.g, color.b, color.a, true, false);
	}

	return ret;
}

bool Minimap::CleanUp()
{
	bool ret = true;

	cells.clear();
	units_to_print.clear();

	return ret;
}



void Minimap::Handle_Input()
{
	if (minimap_background->MouseClickEnterLeftIntern()) {
		iPoint mouse;
		App->input->GetMousePosition(mouse.x, mouse.y);
		MoveCameraToPoint(mouse.x, mouse.y);
	}
}

void Minimap::Enable()
{
	units_to_print.clear();
	units_to_print.reserve(App->entity->entity_list.size());

	iPoint pos = { 0,0 };
	for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++)
	{
		{
			pos = App->map->WorldToMap((*it)->position.x, (*it)->position.y);
			switch ((*it)->GetType())
			{
			case player:
				cells[map_rect.w  * pos.y + pos.x].cell_color = { 20,20,255,255 };
				units_to_print.push_back(cells[map_rect.w * pos.y + pos.x]);
				break;
			case ally:
				cells[map_rect.w  * pos.y + pos.x].cell_color = { 20,255,20,255 };
				units_to_print.push_back(cells[map_rect.w * pos.y + pos.x]);
				break;
			case npc:
				break;
			case enemy:
				cells[map_rect.w  * pos.y + pos.x].cell_color = { 255,20,20,255 };
				units_to_print.push_back(cells[map_rect.w * pos.y + pos.x]);
				break;
			case building:
				break;
			case object:
				break;
			case ally_building:
				break;
			case enemy_building:
				break;
			case null:
				break;
			}
		}
		update_timer.Start();
	}
}

void Minimap::Disable()
{
	units_to_print.clear();
}

void Minimap::MoveCameraToPoint(int x, int y)
{
	if (MiniMToMap(x, y))
	{
		iPoint pos = App->map->MapToWorld(x, y);
		App->render->camera.x = -(pos.x - App->render->camera.w / 2);
		App->render->camera.y = -(pos.y - App->render->camera.h / 2);
	}
}

void Minimap::MoveUnitsToPoint(int x, int y)
{
	if (MiniMToMap(x, y))
	{
		App->player->MoveToTile({ x, y });
	}
}

bool Minimap::MiniMToMap(int& x, int& y)
{
	fPoint rec = { (float)x,(float)y };
	rec.x -= map_rect.x + map_rect.w * 0.5;
	rec.y -= map_rect.y;

	float pX = (((rec.x / half_tile_size.x) + (rec.y / half_tile_size.y)) * 0.5f);
	float pY = (((rec.y / half_tile_size.y) - (rec.x / half_tile_size.x)) * 0.5f);

	x = (int)((pX > (floor(pX) + 0.5f)) ? ceil(pX) : floor(pX));
	y = (int)((pY > (floor(pY) + 0.5f)) ? ceil(pY) : floor(pY));

	if (x <= 0 || x >= map_rect.w || y <= 0 || y >= map_rect.h) return false;
	else return true;
}

iPoint Minimap::MiniMToScreen(int x, int y)
{
	return iPoint((int)(x - y) * (half_tile_size.x), (int)(x + y) * (half_tile_size.y));
}