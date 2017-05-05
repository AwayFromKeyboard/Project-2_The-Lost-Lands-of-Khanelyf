#include "Scene.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Input.h"
#include "Player.h"

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

	/*rect_u = (UI_ColoredRect*)movement_window_u->CreateColoredRect(movement_window_u->GetPosition(), movement_window_u->GetRect().w, movement_window_u->GetRect().h, { 255, 0, 0, 100 });
	rect_d = (UI_ColoredRect*)movement_window_d->CreateColoredRect(movement_window_d->GetPosition(), movement_window_d->GetRect().w, movement_window_d->GetRect().h, { 255, 0, 0, 100 });
	rect_l = (UI_ColoredRect*)movement_window_l->CreateColoredRect(movement_window_l->GetPosition(), movement_window_l->GetRect().w, movement_window_l->GetRect().h, { 255, 0, 0, 100 });
	rect_r = (UI_ColoredRect*)movement_window_r->CreateColoredRect(movement_window_r->GetPosition(), movement_window_r->GetRect().w, movement_window_r->GetRect().h, { 255, 0, 0, 100 });
	rect_u_l = (UI_ColoredRect*)movement_window_u_l->CreateColoredRect(movement_window_u_l->GetPosition(), movement_window_u_l->GetRect().w, movement_window_u_l->GetRect().h, { 0, 255, 0, 100 });
	rect_u_r = (UI_ColoredRect*)movement_window_u_r->CreateColoredRect(movement_window_u_r->GetPosition(), movement_window_u_r->GetRect().w, movement_window_u_r->GetRect().h, { 0, 255, 0, 100 });
	rect_d_l = (UI_ColoredRect*)movement_window_d_l->CreateColoredRect(movement_window_d_l->GetPosition(), movement_window_d_l->GetRect().w, movement_window_d_l->GetRect().h, { 0, 255, 0, 100 });
	rect_d_r = (UI_ColoredRect*)movement_window_d_r->CreateColoredRect(movement_window_d_r->GetPosition(), movement_window_d_r->GetRect().w, movement_window_d_r->GetRect().h, { 0, 255, 0, 100 });*/

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
	if (App->player->pause_status == false) {
		if (movement_window_u_l->MouseEnter() || (App->input->GetKey(SDL_SCANCODE_UP) == key_repeat && App->input->GetKey(SDL_SCANCODE_LEFT) == key_repeat)) {
			App->render->camera.x += CAMERA_SPEED;
			App->render->camera.y += CAMERA_SPEED;
		}
		else if (movement_window_u_r->MouseEnter() || (App->input->GetKey(SDL_SCANCODE_UP) == key_repeat && App->input->GetKey(SDL_SCANCODE_RIGHT) == key_repeat)) {
			App->render->camera.x -= CAMERA_SPEED;
			App->render->camera.y += CAMERA_SPEED;
		}
		else if (movement_window_d_l->MouseEnter() || (App->input->GetKey(SDL_SCANCODE_DOWN) == key_repeat && App->input->GetKey(SDL_SCANCODE_LEFT) == key_repeat)) {
			App->render->camera.x += CAMERA_SPEED;
			App->render->camera.y -= CAMERA_SPEED;
		}
		else if (movement_window_d_r->MouseEnter() || (App->input->GetKey(SDL_SCANCODE_DOWN) == key_repeat && App->input->GetKey(SDL_SCANCODE_RIGHT) == key_repeat)) {
			App->render->camera.x -= CAMERA_SPEED;
			App->render->camera.y -= CAMERA_SPEED;
		}
		else if (movement_window_u->MouseEnter() || App->input->GetKey(SDL_SCANCODE_UP) == key_repeat) {
			App->render->camera.y += CAMERA_SPEED;
		}
		else if (movement_window_d->MouseEnter() || App->input->GetKey(SDL_SCANCODE_DOWN) == key_repeat) {
			App->render->camera.y -= CAMERA_SPEED;
		}
		else if (movement_window_l->MouseEnter() || App->input->GetKey(SDL_SCANCODE_LEFT) == key_repeat) {
			App->render->camera.x += CAMERA_SPEED;
		}
		else if (movement_window_r->MouseEnter() || App->input->GetKey(SDL_SCANCODE_RIGHT) == key_repeat) {
			App->render->camera.x -= CAMERA_SPEED;
		}
	}
}
