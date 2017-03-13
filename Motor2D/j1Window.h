#ifndef __j1WINDOW_H__
#define __j1WINDOW_H__

#include "j1Module.h"
#include "Point.h"

struct SDL_Window;
struct SDL_Surface;

class j1Window : public j1Module
{
public:

	j1Window();

	// Destructor
	virtual ~j1Window();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Changae title
	void SetTitle(const char* new_title);

	void SetWindowIcon(SDL_Surface * sur);

	// Retrive window size
	void GetWindowSize(uint& width, uint& height) const;
	iPoint _GetWindowSize();
	// Retrieve window scale
	uint GetScale() const;

	void OnCommand(std::list<std::string>& tokens);

	void OnCVar(std::list<std::string>& tokens);

	void SaveCVar(std::string& cvar_name, pugi::xml_node& node) const;

public:
	//The window we'll be rendering to
	SDL_Window*  window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

private:
	string	     title;
	uint		 width = 0;
	uint		 height = 0;
	uint		 scale = 0;
};

#endif // __j1WINDOW_H__