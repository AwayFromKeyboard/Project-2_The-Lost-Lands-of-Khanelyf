#include "DialogueManager.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "Log.h"
#include "j1Gui.h"

DialogueManager::DialogueManager() : j1Module()
{
	name = ("dialogue");
}

bool DialogueManager::Awake(pugi::xml_node & Dialogues)
{
	bool ret = true;

	LOG("Loading DialogManager data");

	App->LoadXML("Dialogues.xml", doc);

	if (doc == NULL)
	{
		LOG("Could not load gui xml file %s.", doc);
		ret = false;
	}

	return ret;
}

bool DialogueManager::Start()
{
	bool ret = true;
	node = doc.child("npcs");
	// Allocate memory
	int i = 0;
	for (pugi::xml_node npc = node.child("npc"); npc != NULL; npc = npc.next_sibling(), i++)
	{
		//Allocate Dialog with his ID and State
		Dialogue* tmp = new Dialogue(npc.attribute("id").as_int());
		dialog.push_back(tmp);

		//Allocate text
		for (pugi::xml_node dialogue = npc.child("dialogue"); dialogue != NULL; dialogue = dialogue.next_sibling())
		{
			for (pugi::xml_node text = dialogue.child("text"); text != NULL; text = text.next_sibling("text"))
			{
				TextLine* tmp = new TextLine(dialogue.attribute("state").as_int(), text.attribute("value").as_string());
				dialog[i]->texts.push_back(tmp);
			}
		}
	}

	win = (UI_Window*)App->gui->UI_CreateWin({ 0,0 }, 0, 0, 10);
	text_on_screen = (UI_Text*)win->CreateText(TEXT_POSITION, App->font->default_15, 0, false, 0,51,51);

	return ret;
}

bool DialogueManager::PostUpdate()
{
	/*--- CODE TO TEST RESULTS IN-GAME ---*/
	if (App->input->GetKey(SDL_SCANCODE_R) == key_down)
	{
		dialogueStep = 0;
		if (id == 0)
		{
			id = 1;
		}
		else
		{
			id = 0;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_X) == key_down)
	{
		dialogueStep = 0;
		if (NPCstate == 0)
		{
			NPCstate = 1;
		}
		else
		{
			NPCstate = 0;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == key_down)
	{
		dialogueStep = 0;
	}

	/*--- END ---*/

	//text_on_screen->Set_Active_state(true); //Active screen

	if (App->input->GetKey(SDL_SCANCODE_Q) == key_down)
	{
		dialogueStep++;
	}

	BlitDialog(id, NPCstate); //Calls Blit function
	return true;
}

bool DialogueManager::BlitDialog(uint id, uint state)
{
	//Find the correct ID
	for (int i = 0; i < dialog.size(); i++)
	{
		if (dialog[i]->id == id)
		{
			for (int j = 0; (j + dialogueStep) < dialog[i]->texts.size(); j++) //Search correct text inside Dialogue 
			{
				if (dialog[i]->texts[dialogueStep + j]->state == state)
				{
					text_on_screen->SetText(dialog[i]->texts[dialogueStep + j]->line->c_str());
					//text_on_screen->Set_String((char*)dialog[i]->texts[dialogueStep + j]->line->c_str());
					return true;
				}
			}
		}
	}

	return false;
}

DialogueManager::~DialogueManager()
{
	dialog.clear();
}

Dialogue::Dialogue(int id) : id(id)
{}

Dialogue::~Dialogue()
{
	texts.clear();
}

TextLine::TextLine(int NPCstate, std::string text) : state(NPCstate)
{
	line = new std::string(text);
}

TextLine::~TextLine()
{

}
