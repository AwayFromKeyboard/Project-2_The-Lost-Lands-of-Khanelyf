#include "Player.h"
#include "j1App.h"
#include "j1Input.h"
#include "Unit.h"
#include "j1Gui.h"
#include "j1Entity.h"
#include "j1Collisions.h"
#include "j1Entity.h"
#include "GameObject.h"
#include "Defs.h"
#include "Log.h"

Player::Player()
{
}

Player::~Player()
{
}

bool Player::Start()
{
	bool ret = true;
	return ret;
}

bool Player::PreUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_F1) == key_down)
		App->debug_mode = !App->debug_mode;

	if (App->input->GetKey(SDL_SCANCODE_F2) == key_down && App->debug_mode)
		App->gui->debug = !App->gui->debug;
	else if (!App->debug_mode)
		App->gui->debug = false;

	if (App->input->GetKey(SDL_SCANCODE_F3) == key_down && App->debug_mode)
		App->collisions->debug = !App->collisions->debug;
	else if (!App->debug_mode)
		App->gui->debug = false;

	if (App->input->GetKey(SDL_SCANCODE_F10) == key_down)
		ShellExecute(NULL, "open", "https://github.com/AwayFromKeyboard/Project-2_The-Lost-Lands-of-Khanelyf/issues", NULL, NULL, SW_SHOWMAXIMIZED);

	return ret;
}

bool Player::Update(float dt)
{
	bool ret = true;

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_down) {
		iPoint mouse;
		App->input->GetMouseWorld(mouse.x, mouse.y);
		App->entity->UnselectEverything();
		for (std::list<Entity*>::iterator it = App->entity->entity_list.begin(); it != App->entity->entity_list.end(); it++) {
			Collider* unit = (*it)->GetCollider();
			
			if (mouse.x > unit->rect.x && mouse.x < unit->rect.w && mouse.y > unit->rect.y && mouse.y < unit->rect.y + unit->rect.h) {
				(*it)->SetSelected(true);
			}
			if ((*it)->GetSelected())
				App->entity->selected.push_back((Unit*)*it);
		}

	}

	return ret;
}

bool Player::PostUpdate()
{
	bool ret = true;
	return ret;
}

bool Player::CleanUp()
{
	bool ret = true;
	return ret;
}
