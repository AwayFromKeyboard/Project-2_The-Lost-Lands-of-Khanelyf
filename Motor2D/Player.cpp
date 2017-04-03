#include "Player.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Collisions.h"
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

	if (App->input->GetKey(SDL_SCANCODE_F10) == key_down) {
		ShellExecute(NULL, "open", "https://github.com/AwayFromKeyboard/Project-2_The-Lost-Lands-of-Khanelyf/issues", NULL, NULL, SW_SHOWMAXIMIZED);
	}

	return ret;
}

bool Player::Update(float dt)
{
	bool ret = true;
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
