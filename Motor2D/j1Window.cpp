#include "Defs.h"
#include "Log.h"
#include "Point.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Console.h"

#include "SDL/include/SDL.h"


j1Window::j1Window() : j1Module()
{
	window = NULL;
	screen_surface = NULL;
	name = "window";
}

// Destructor
j1Window::~j1Window()
{
}

// Called before render is available
bool j1Window::Awake(pugi::xml_node& config)
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		Uint32 flags = SDL_WINDOW_SHOWN;
		fullscreen = config.child("fullscreen").attribute("value").as_bool(false);
		bool borderless = config.child("borderless").attribute("value").as_bool(false);
		bool resizable = config.child("resizable").attribute("value").as_bool(false);
		bool fullscreen_window = config.child("fullscreen_window").attribute("value").as_bool(false);

		width = config.child("resolution").attribute("width").as_int(640);
		height = config.child("resolution").attribute("height").as_int(480);
		scale = config.child("resolution").attribute("scale").as_int(1);

		if(fullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(borderless == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(resizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(fullscreen_window == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(App->GetTitle(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
	
		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	title = App->GetTitle();

	App->console->AddCVar("window.set_title", this, "Changes window title");
	App->console->ChangeCVarMaxArgs("window.set_title", 50);

	return ret;
}

// Called before quitting
bool j1Window::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

// Set new window title
void j1Window::SetTitle(const char* new_title)
{
	SDL_SetWindowTitle(window, new_title);
}

// Set a 32x32 window icon
void j1Window::SetWindowIcon(SDL_Surface* sur)
{
	SDL_SetWindowIcon(window, sur);
}

void j1Window::GetWindowSize(uint& width, uint& height) const
{
	width = this->width;
	height = this->height;
}

iPoint j1Window::_GetWindowSize() {
	return iPoint(this->width, this->height);
}

uint j1Window::GetScale() const
{
	return scale;
}

void j1Window::OnCommand(std::list<std::string>& tokens)
{
}

void j1Window::OnCVar(std::list<std::string>& tokens)
{
	if (tokens.front() == "window.set_title")
	{
		if (tokens.size() > 1)
		{
			title.clear();
			list<string>::iterator it = tokens.begin();
			it++;
			for (; it != tokens.end(); it++)
			{
				if (it != --tokens.end())
				{
					(*it)[(*it).size()] = ' ';
					(*it)[(*it).size() + 1] = '\0';
				}
				title.insert(title.size(), (*it).c_str());
			}
			SetTitle(title.c_str());
			string new_title("Title set to: ");
			new_title.insert(new_title.size(), title);
			App->console->AddText(new_title.c_str(), output);
		}
	}
}

void j1Window::SaveCVar(std::string& cvar_name, pugi::xml_node & node) const
{
	if (cvar_name == "set_title") {
		pugi::xml_node app = node.parent().parent().child("app");
		app.child("title").attribute("name").set_value(title.c_str());
	}
}

//bool j1Window::ToggleFullscreen(bool toggle)
//{
//	if (toggle) {
//		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
//	}
//	else {
//		SDL_DisplayMode new_win;
//		new_win.w = width;
//		new_win.h = height;
//		new_win.refresh_rate = 0;
//		SDL_SetWindowDisplayMode(window, &new_win);
//	}
//	return true;
//}

bool j1Window::IsInFullScreen() const
{
	return fullscreen;
}

bool j1Window::ChangeToFullScreen()
{
	bool ret = true;

	//Create window
	Uint32 flags = SDL_WINDOW_SHOWN;

	if (fullscreen == false)
	{
		flags |= SDL_WINDOW_FULLSCREEN;
		fullscreen = true;
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}

	if (window == NULL)
	{
		LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Get window surface
		screen_surface = SDL_GetWindowSurface(window);
	}
	return ret;
}

bool j1Window::ChangeToWindow()
{
	bool ret = true;

	//Create window

	if (fullscreen == true)
	{
		fullscreen = false;
		SDL_SetWindowFullscreen(window, SDL_FALSE);
	}

	if (window == NULL)
	{
		LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Get window surface
		screen_surface = SDL_GetWindowSurface(window);
	}


	return ret;
}