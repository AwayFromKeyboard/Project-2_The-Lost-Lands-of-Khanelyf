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
}

bool Minimap::Start() 
{
	map_rect = { 1389,912,273,139 };

	minimap_window = (UI_Window*)App->gui->UI_CreateWin({ 1389, 912 }, 273, 139, 8);
	minimap_background = (UI_Image*)minimap_window->CreateImage({ 1389, 912 }, { 1237, 810, 273, 139 });

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
		pos = MiniMToScreen(i % map_size.x, i / map_size.y);
		cells.push_back(new_cell);
		cells[i].cell_position.x += pos.x;
		cells[i].cell_position.y += pos.y;
	}

	update_timer.Start();

	return true;
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
				cells[map_size.x * pos.y + pos.x].cell_color = { 0,255,0,255 };
				units_to_print.push_back(cells[map_size.x * pos.y + pos.x]);
				break;
			case ally:
				cells[map_size.x * pos.y + pos.x].cell_color = { 50,255,50,255 };
				units_to_print.push_back(cells[map_size.x * pos.y + pos.x]);
				break;
			case npc:
				cells[map_size.x * pos.y + pos.x].cell_color = { 120,255,20,255 };
				units_to_print.push_back(cells[map_size.x * pos.y + pos.x]);
				break;
			case enemy:
				cells[map_size.x * pos.y + pos.x].cell_color = { 255,20,20,255 };
				units_to_print.push_back(cells[map_size.x * pos.y + pos.x]);
				break;
			case building:
				
				break;
			case object:
				break;
			case ally_building:
				cells[map_size.x * pos.y + pos.x].cell_color = { 255,255,20,255 };
				units_to_print.push_back(cells[map_size.x * pos.y + pos.x]);
				break;
			case enemy_building:
				cells[map_size.x * pos.y + pos.x].cell_color = { 20,255,255,255 };
				units_to_print.push_back(cells[map_size.x * pos.y + pos.x]);
				break;
			case enemy_boss:
				cells[map_size.x * pos.y + pos.x].cell_color = { 255,0,0,255 };
				units_to_print.push_back(cells[map_size.x * pos.y + pos.x]);
				break;
			case null:
				break;
			}
		}
		update_timer.Start();

		minimap_quad = App->map->WorldToMap(App->render->camera.x, App->render->camera.y);
		if (minimap_quad.x >= 0 && minimap_quad.y < 0) // When Minimap quad gets out of limits (needs revision)
		{
			cells[map_size.x * -minimap_quad.y + minimap_quad.x].cell_color = { 255,255,255,255 };
			units_to_print.push_back(cells[map_size.x * -minimap_quad.y + minimap_quad.x]);
		}
		else if (minimap_quad.x >= 0 && minimap_quad.y >= 0) // When Minimap quad gets out of limits (needs revision)
		{
			cells[map_size.x * minimap_quad.y + minimap_quad.x].cell_color = { 255,255,255,255 };
			units_to_print.push_back(cells[map_size.x * minimap_quad.y + minimap_quad.x]);
		}
		else if (minimap_quad.x < 0 && minimap_quad.y >= 0) // When Minimap quad gets out of limits (needs revision)
		{
			cells[map_size.x * minimap_quad.y + -minimap_quad.x].cell_color = { 255,255,255,255 };
			units_to_print.push_back(cells[map_size.x * minimap_quad.y + -minimap_quad.x]);
		}
		else if (minimap_quad.x < 0 && minimap_quad.y < 0)
		{
			cells[map_size.x * -minimap_quad.y + -minimap_quad.x].cell_color = { 255,255,255,255 };
			units_to_print.push_back(cells[map_size.x * -minimap_quad.y + -minimap_quad.x]);
		}
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

	App->scene->LayerDrawQuad(map_rect, 0, 0, 0, 255, true, false, 10);

	// Draw Units in Minimap
	SDL_Color color;
	int size = units_to_print.size();
	for (int i = 0; i < size; i++)
	{
		color = units_to_print[i].cell_color;
		if (color.r == 255 && color.g == 255 && color.b == 255) {
			iPoint map_size = { (App->render->camera.w / 26), (App->render->camera.h / 26) };
			App->scene->LayerDrawQuad({ units_to_print[i].cell_position.x - 5, units_to_print[i].cell_position.y, map_size.x, map_size.y }, color.r, color.g, color.b, color.a, false, false, 11);
		}
		else if (color.r == 255 && color.g == 0 && color.b == 0) {
			App->scene->LayerDrawQuad({ units_to_print[i].cell_position.x, units_to_print[i].cell_position.y,5,5 }, color.r, color.g, color.b, color.a, true, false, 10);

		}
		else
			App->scene->LayerDrawQuad({ units_to_print[i].cell_position.x, units_to_print[i].cell_position.y,3,3 }, color.r, color.g, color.b, color.a, true, false, 10);
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
	iPoint mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);
	if (mouse.PointInRect(map_rect.x, map_rect.y, map_rect.w, map_rect.h))
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_down) {
			MoveCameraToPoint(mouse.x, mouse.y);
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == key_down) {
			MoveUnitsToPoint(mouse.x, mouse.y);
		}
	}
}

bool Minimap::IsMouseOver()
{
	iPoint mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);
	return mouse.PointInRect(map_rect.x, map_rect.y, map_rect.w, map_rect.h);
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
				cells[map_size.x  * pos.y + pos.x].cell_color = { 20,20,255,255 };
				units_to_print.push_back(cells[map_size.x * pos.y + pos.x]);
				break;
			case ally:
				cells[map_size.x  * pos.y + pos.x].cell_color = { 20,255,20,255 };
				units_to_print.push_back(cells[map_size.x * pos.y + pos.x]);
				break;
			case npc:
				break;
			case enemy:
				cells[map_size.x  * pos.y + pos.x].cell_color = { 255,20,20,255 };
				units_to_print.push_back(cells[map_size.x * pos.y + pos.x]);
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