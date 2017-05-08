#include "Scene.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1Map.h"

void Scene::InitCameraMovement()
{
	movement_window_u = (UI_Window*)App->gui->UI_CreateWin(iPoint(MOVEMENT_AREA, -1), App->win->_GetWindowSize().x - 2 * MOVEMENT_AREA, MOVEMENT_AREA, 5);
	movement_window_d = (UI_Window*)App->gui->UI_CreateWin(iPoint(MOVEMENT_AREA, App->win->_GetWindowSize().y - MOVEMENT_AREA), App->win->_GetWindowSize().x - 2 * MOVEMENT_AREA, MOVEMENT_AREA, 5);
	movement_window_l = (UI_Window*)App->gui->UI_CreateWin(iPoint(-1, MOVEMENT_AREA), MOVEMENT_AREA, App->win->_GetWindowSize().y - 2 * MOVEMENT_AREA, 5);
	movement_window_r = (UI_Window*)App->gui->UI_CreateWin(iPoint(App->win->_GetWindowSize().x - MOVEMENT_AREA, MOVEMENT_AREA), MOVEMENT_AREA, App->win->_GetWindowSize().y - 2 * MOVEMENT_AREA, 5);
	movement_window_u_l = (UI_Window*)App->gui->UI_CreateWin(iPoint(-1, -1), MOVEMENT_AREA, MOVEMENT_AREA, 5);
	movement_window_u_r = (UI_Window*)App->gui->UI_CreateWin(iPoint(App->win->_GetWindowSize().x - MOVEMENT_AREA, -1), MOVEMENT_AREA, MOVEMENT_AREA, 5);
	movement_window_d_l = (UI_Window*)App->gui->UI_CreateWin(iPoint(-1, App->win->_GetWindowSize().y - MOVEMENT_AREA), MOVEMENT_AREA, MOVEMENT_AREA, 5);
	movement_window_d_r = (UI_Window*)App->gui->UI_CreateWin(iPoint(App->win->_GetWindowSize().x - MOVEMENT_AREA, App->win->_GetWindowSize().y - MOVEMENT_AREA), MOVEMENT_AREA, MOVEMENT_AREA, 5);

	movement_window_u->SetEnabled(true);
	movement_window_d->SetEnabled(true);
	movement_window_l->SetEnabled(true);
	movement_window_r->SetEnabled(true);
	movement_window_u_l->SetEnabled(true);
	movement_window_u_r->SetEnabled(true);
	movement_window_d_l->SetEnabled(true);
	movement_window_d_r->SetEnabled(true);
}

void Scene::UpdateCameraMovement()
{
	int map_width = ((App->map->data.width / 2) * sqrt((App->map->data.tile_width * App->map->data.tile_width) + (App->map->data.tile_height * App->map->data.tile_height))) - MAP_LIMIT;
	int map_height = ((App->map->data.height / 2) * sqrt((App->map->data.tile_width * App->map->data.tile_width) + (App->map->data.tile_height * App->map->data.tile_height))) - MAP_LIMIT;

	if (movement_window_u_l->MouseEnter() || (App->input->GetKey(SDL_SCANCODE_UP) == key_repeat && App->input->GetKey(SDL_SCANCODE_LEFT) == key_repeat)) {
		if (App->render->camera.x <= map_width) // Blocks camera in the left limit but allows movement in the other direction
			App->render->camera.x += CAMERA_SPEED;
		if (App->render->camera.y <= MAP_TOP)
			App->render->camera.y += CAMERA_SPEED;
	}
	else if (movement_window_u_r->MouseEnter() || (App->input->GetKey(SDL_SCANCODE_UP) == key_repeat && App->input->GetKey(SDL_SCANCODE_RIGHT) == key_repeat)) {
		if (App->render->camera.x >= (-map_width + 300))
			App->render->camera.x -= CAMERA_SPEED;
		if (App->render->camera.y <= MAP_TOP)
			App->render->camera.y += CAMERA_SPEED;
	}
	else if (movement_window_d_l->MouseEnter() || (App->input->GetKey(SDL_SCANCODE_DOWN) == key_repeat && App->input->GetKey(SDL_SCANCODE_LEFT) == key_repeat)) {
		if (App->render->camera.x <= map_width)
			App->render->camera.x += CAMERA_SPEED;
		if (App->render->camera.y >= -map_height)
			App->render->camera.y -= CAMERA_SPEED;
	}
	else if (movement_window_d_r->MouseEnter() || (App->input->GetKey(SDL_SCANCODE_DOWN) == key_repeat && App->input->GetKey(SDL_SCANCODE_RIGHT) == key_repeat)) {
		if (App->render->camera.x >= (-map_width + 300))
			App->render->camera.x -= CAMERA_SPEED;
		if (App->render->camera.y >= -map_height)
			App->render->camera.y -= CAMERA_SPEED;
	}
	else if ((movement_window_u->MouseEnter() || App->input->GetKey(SDL_SCANCODE_UP) == key_repeat) && App->render->camera.y <= MAP_TOP) {
			App->render->camera.y += CAMERA_SPEED;
	}
	else if ((movement_window_d->MouseEnter() || App->input->GetKey(SDL_SCANCODE_DOWN) == key_repeat) && App->render->camera.y >= -map_height) {
		App->render->camera.y -= CAMERA_SPEED;
	}
	else if ((movement_window_l->MouseEnter() || App->input->GetKey(SDL_SCANCODE_LEFT) == key_repeat) && App->render->camera.x <= map_width) {
		App->render->camera.x += CAMERA_SPEED;
	}
	else if ((movement_window_r->MouseEnter() || App->input->GetKey(SDL_SCANCODE_RIGHT) == key_repeat) && App->render->camera.x >= (-map_width + 300)) {
		App->render->camera.x -= CAMERA_SPEED;
	}
	
}
