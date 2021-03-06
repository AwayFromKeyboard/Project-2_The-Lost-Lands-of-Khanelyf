#include "Defs.h"
#include "Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Window.h"
#include "SDL/include/SDL.h"
#include "j1Render.h"

#define MAX_KEYS 300

j1Input::j1Input() : j1Module()
{
	name = "input";

	keyboard = new j1_key_state[MAX_KEYS];
	memset(keyboard, key_idle, sizeof(j1_key_state) * MAX_KEYS);
	memset(mouse_buttons, key_idle, sizeof(j1_key_state) * NUM_MOUSE_BUTTONS);
}

// Destructor
j1Input::~j1Input()
{
	delete[] keyboard;
}

// Called before render is available
bool j1Input::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	// GUI -------------------------
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		LOG("Error on SDL_Init");
	// -----------------------------

	return ret;
}

// Called before the first frame
bool j1Input::Start()
{
	LOG("Start module input");
	SDL_StopTextInput();
	DefaultControls();
	return true;
}

// Called each loop iteration
bool j1Input::PreUpdate()
{
	static SDL_Event event;
	
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == key_idle)
				keyboard[i] = key_down;
			else
				keyboard[i] = key_repeat;
		}
		else
		{
			if(keyboard[i] == key_repeat || keyboard[i] == key_down)
				keyboard[i] = key_up;
			else
				keyboard[i] = key_idle;
		}
	}

	for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if(mouse_buttons[i] == key_down)
			mouse_buttons[i] = key_repeat;

		if(mouse_buttons[i] == key_up)
			mouse_buttons[i] = key_idle;
	}

	// GUI -------------------------
	SDL_StartTextInput();
	input_text.clear();
	// -----------------------------

	while(SDL_PollEvent(&event) != 0)
	{
		switch(event.type)
		{
			case SDL_QUIT:
				windowEvents[we_quit] = true;
			break;

			case SDL_WINDOWEVENT:
				switch(event.window.event)
				{
					//case SDL_WINDOWEVENT_LEAVE:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_FOCUS_LOST:
					windowEvents[we_hide] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
					windowEvents[we_show] = true;
					break;
				}
			break;

			case SDL_MOUSEBUTTONDOWN:
				mouse_buttons[event.button.button - 1] = key_down;
				//LOG("Mouse button %d down", event.button.button-1);
			break;

			case SDL_MOUSEBUTTONUP:
				mouse_buttons[event.button.button - 1] = key_up;
				//LOG("Mouse button %d up", event.button.button-1);
			break;

			case SDL_MOUSEMOTION:
			{
				int scale = App->win->GetScale();
				mouse_motion_x = event.motion.xrel / scale;
				mouse_motion_y = event.motion.yrel / scale;
				mouse_x = event.motion.x / scale;
				mouse_y = event.motion.y / scale;
				//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			}
			break;

			// GUI -------------------
			case SDL_TEXTINPUT:
			{
				input_text.insert(input_text.size(), event.text.text);
			}
			break;

			// ------------------------
		}
	}

	// GUI -------------------------
	SDL_StopTextInput();
	// -----------------------------

	return true;
}

// Called before quitting
bool j1Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

// ---------
bool j1Input::GetWindowEvent(j1_event_window ev)
{
	return windowEvents[ev];
}

void j1Input::GetMouseWorld(int& x, int& y)
{
	x = mouse_x - App->render->camera.x;
	y = mouse_y - App->render->camera.y;
}

void j1Input::SetMouseWorld(int& x, int& y)
{
	mouse_x = x;
	mouse_y = y;
}

const char * j1Input::GetKeyString(SDL_Scancode scan) const
{
	return SDL_GetKeyName(SDL_GetKeyFromScancode(scan));
}

void j1Input::DefaultControls()
{
	for (int i = 0; i < NULL_CONTROL; i++)
	{
		if (i == MOVE_MAP_UP)
		{
			controls[i] = SDL_SCANCODE_UP;
		}
		else if (i == MOVE_MAP_DOWN)
		{
			controls[i] = SDL_SCANCODE_DOWN;
		}
		else if (i == MOVE_MAP_LEFT)
		{
			controls[i] = SDL_SCANCODE_LEFT;
		}
		else if (i == MOVE_MAP_RIGHT)
		{
			controls[i] = SDL_SCANCODE_RIGHT;
		}
		else if (i == BATTLECRY)
		{
			controls[i] = SDL_SCANCODE_X;
		}
		else if (i == WHIRLWIND)
		{
			controls[i] = SDL_SCANCODE_C;
		}
		else if (i == CHARGE)
		{
			controls[i] = SDL_SCANCODE_V;
		}
		else if (i == CREATE_GROUP)
		{
			controls[i] = SDL_SCANCODE_LSHIFT;
		}
	}
}

SDL_Scancode j1Input::ReturnKey()
{
	if (GetKey(SDL_SCANCODE_Q) == key_down)
	{
		return SDL_SCANCODE_Q;
	}
	else if (GetKey(SDL_SCANCODE_W) == key_down)
	{
		return SDL_SCANCODE_W;
	}
	else if (GetKey(SDL_SCANCODE_E) == key_down)
	{
		return SDL_SCANCODE_E;
	}
	else if (GetKey(SDL_SCANCODE_R) == key_down)
	{
		return SDL_SCANCODE_R;
	}
	else if (GetKey(SDL_SCANCODE_T) == key_down)
	{
		return SDL_SCANCODE_T;
	}
	else if (GetKey(SDL_SCANCODE_Y) == key_down)
	{
		return SDL_SCANCODE_Y;
	}
	else if (GetKey(SDL_SCANCODE_U) == key_down)
	{
		return SDL_SCANCODE_U;
	}
	else if (GetKey(SDL_SCANCODE_I) == key_down)
	{
		return SDL_SCANCODE_I;
	}
	else if (GetKey(SDL_SCANCODE_O) == key_down)
	{
		return SDL_SCANCODE_O;
	}
	else if (GetKey(SDL_SCANCODE_P) == key_down)
	{
		return SDL_SCANCODE_P;
	}
	else if (GetKey(SDL_SCANCODE_A) == key_down)
	{
		return SDL_SCANCODE_A;
	}
	else if (GetKey(SDL_SCANCODE_S) == key_down)
	{
		return SDL_SCANCODE_S;
	}
	else if (GetKey(SDL_SCANCODE_D) == key_down)
	{
		return SDL_SCANCODE_D;
	}
	else if (GetKey(SDL_SCANCODE_F) == key_down)
	{
		return SDL_SCANCODE_F;
	}
	else if (GetKey(SDL_SCANCODE_G) == key_down)
	{
		return SDL_SCANCODE_G;
	}
	else if (GetKey(SDL_SCANCODE_H) == key_down)
	{
		return SDL_SCANCODE_H;
	}
	else if (GetKey(SDL_SCANCODE_J) == key_down)
	{
		return SDL_SCANCODE_J;
	}
	else if (GetKey(SDL_SCANCODE_K) == key_down)
	{
		return SDL_SCANCODE_K;
	}
	else if (GetKey(SDL_SCANCODE_L) == key_down)
	{
		return SDL_SCANCODE_L;
	}
	else if (GetKey(SDL_SCANCODE_Z) == key_down)
	{
		return SDL_SCANCODE_Z;
	}
	else if (GetKey(SDL_SCANCODE_X) == key_down)
	{
		return SDL_SCANCODE_X;
	}
	else if (GetKey(SDL_SCANCODE_C) == key_down)
	{
		return SDL_SCANCODE_C;
	}
	else if (GetKey(SDL_SCANCODE_V) == key_down)
	{
		return SDL_SCANCODE_V;
	}
	else if (GetKey(SDL_SCANCODE_B) == key_down)
	{
		return SDL_SCANCODE_B;
	}
	else if (GetKey(SDL_SCANCODE_N) == key_down)
	{
		return SDL_SCANCODE_N;
	}
	else if (GetKey(SDL_SCANCODE_M) == key_down)
	{
		return SDL_SCANCODE_M;
	}
	else if (GetKey(SDL_SCANCODE_TAB) == key_down)
	{
		return SDL_SCANCODE_TAB;
	}
	else if (GetKey(SDL_SCANCODE_LSHIFT) == key_down)
	{
		return SDL_SCANCODE_LSHIFT;
	}
	else if (GetKey(SDL_SCANCODE_UP) == key_down)
	{
		return SDL_SCANCODE_UP;
	}
	else if (GetKey(SDL_SCANCODE_DOWN) == key_down)
	{
		return SDL_SCANCODE_DOWN;
	}
	else if (GetKey(SDL_SCANCODE_RIGHT) == key_down)
	{
		return SDL_SCANCODE_RIGHT;
	}
	else if (GetKey(SDL_SCANCODE_LEFT) == key_down)
	{
		return SDL_SCANCODE_LEFT;
	}
	else if (GetKey(SDL_SCANCODE_1) == key_down)
	{
		return SDL_SCANCODE_1;
	}
	else if (GetKey(SDL_SCANCODE_2) == key_down)
	{
		return SDL_SCANCODE_2;
	}
	else if (GetKey(SDL_SCANCODE_3) == key_down)
	{
		return SDL_SCANCODE_3;
	}
	else if (GetKey(SDL_SCANCODE_4) == key_down)
	{
		return SDL_SCANCODE_4;
	}
	else if (GetKey(SDL_SCANCODE_5) == key_down)
	{
		return SDL_SCANCODE_5;
	}
	else if (GetKey(SDL_SCANCODE_6) == key_down)
	{
		return SDL_SCANCODE_6;
	}
	else if (GetKey(SDL_SCANCODE_7) == key_down)
	{
		return SDL_SCANCODE_7;
	}
	else if (GetKey(SDL_SCANCODE_8) == key_down) 
	{
		return SDL_SCANCODE_8;
	}
	else if (GetKey(SDL_SCANCODE_9) == key_down)
	{
		return SDL_SCANCODE_9;
	}
	else
	{
		return SDL_SCANCODE_0;
	}
}


void j1Input::GetMousePosition(int& x, int& y)
{
	x = mouse_x;
	y = mouse_y;
}

void j1Input::GetMouseMotion(int& x, int& y)
{
	x = mouse_motion_x;
	y = mouse_motion_y;
}

