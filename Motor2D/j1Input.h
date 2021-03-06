#ifndef __j1INPUT_H__
#define __j1INPUT_H__

#include "j1Module.h"
#include "SDL/include/SDL.h"
//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
#define CONTROLS_SIZE 8
//#define LAST_KEYS_PRESSED_BUFFER 50

struct SDL_Rect;

enum j1_event_window
{
	we_quit = 0,
	we_hide = 1,
	we_show = 2,
	we_count
};

enum j1_key_state
{
	key_idle = 0,
	key_down,
	key_repeat,
	key_up,
	j1_key_state_null
};

enum Controls {

	MOVE_MAP_UP,
	MOVE_MAP_DOWN,
	MOVE_MAP_LEFT,
	MOVE_MAP_RIGHT,
	BATTLECRY,
	WHIRLWIND,
	CHARGE,
	CREATE_GROUP,
	NULL_CONTROL
};

class j1Input : public j1Module
{

public:

	j1Input();

	// Destructor
	virtual ~j1Input();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Gather relevant win events
	bool GetWindowEvent(j1_event_window ev);

	void GetMouseWorld(int & x, int & y);

	void SetMouseWorld(int & x, int & y);

	// Check key states (includes mouse and joy buttons)
	j1_key_state GetKey(int id) const
	{
		return keyboard[id];
	}

	void DefaultControls();

	const char * GetKeyString(SDL_Scancode scan) const;

	SDL_Scancode ReturnKey();

	j1_key_state GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	// Check if a certain window event happened
	bool GetWindowEvent(int code);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

	string		input_text;

	int controls[NULL_CONTROL];

private:
	bool		windowEvents[we_count];
	j1_key_state*	keyboard = nullptr;
	j1_key_state	mouse_buttons[NUM_MOUSE_BUTTONS];
	int			mouse_motion_x = 0;
	int			mouse_motion_y = 0;
	int			mouse_x = 0;
	int			mouse_y = 0;
};

#endif // __j1INPUT_H__