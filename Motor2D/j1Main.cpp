#include <stdlib.h>

#include "Defs.h"
#include "Log.h"
#include "j1App.h"
#include "MemLeaks.h"

#include "Brofiler\Brofiler.h"
#pragma comment( lib, "Brofiler/ProfilerCore32.lib" )

// This is needed here because SDL redefines main function
// do not add any other libraries here, instead put them in their modules
#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

enum main_state
{
	create = 1,
	awake,
	start,
	loop,
	clean,
	fail,
	exit_
};

j1App* App = NULL;

int main(int argc, char* args[])
{
	LOG("Engine starting ... %d");
	ReportMemoryLeaks();
	main_state state = main_state::create;
	int result = EXIT_FAILURE;

	while(state != exit_)
	{
		BROFILER_FRAME("Main")
		switch(state)
		{
			// Allocate the engine --------------------------------------------
			case create:
			LOG("CREATION PHASE ===============================");

			App = new j1App(argc, args);

			if(App != NULL)
				state = awake;
			else
				state = fail;

			break;

			// Awake all modules -----------------------------------------------
			case awake:
			LOG("AWAKE PHASE ===============================");
			if(App->Awake() == true)
				state = start;
			else
			{
				LOG("ERROR: Awake failed");
				state = fail;
			}

			break;

			// Call all modules before first frame  ----------------------------
			case start:
			LOG("START PHASE ===============================");
			if(App->Start() == true)
			{
				state = loop;
				LOG("UPDATE PHASE ===============================");
			}
			else
			{
				state = fail;
				LOG("ERROR: Start failed");
			}
			break;

			// Loop all modules until we are asked to leave ---------------------
			case loop:
			if(App->Update() == false)
				state = clean;
			break;

			// Cleanup allocated memory -----------------------------------------
			case clean:
			LOG("CLEANUP PHASE ===============================");
			if(App->CleanUp() == true)
			{
				RELEASE(App);
				result = EXIT_SUCCESS;
				state = exit_;
			}
			else
				state = fail;

			break;

			// Exit with errors and shame ---------------------------------------
			case fail:
			LOG("Exiting with errors :(");
			result = EXIT_FAILURE;
			state = exit_;
			break;
		}
	}

	LOG("... Bye! :)\n");

	// Dump memory leaks
	return result;
}