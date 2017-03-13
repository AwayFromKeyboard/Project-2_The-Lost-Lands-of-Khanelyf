#include "Defs.h"
#include "Log.h"
#include "j1Scene.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "MainScene.h"
#include "MainMenu.h"
#include "MainScene.h"
#include "SceneTest.h"
#include "j1Console.h"


j1Scene::j1Scene() : j1Module()
{
	name = "scene";
}

// Destructor
j1Scene::~j1Scene()
{

}

// Called before render is available
bool j1Scene::Awake()
{
	bool ret = true;

	LOG("Loading SceneManager");

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = false;

	LOG("Start module scene");

	// Create scenes
	main_menu = new MainMenu();
	main_scene = new MainScene();
	scene_test = new SceneTest();

	scenes.push_back(main_menu);
	scenes.push_back(main_scene);
	scenes.push_back(scene_test);
	// -------------

	// Starting scene
	current_scene = scene_test;

	if(current_scene != nullptr)
		ret = current_scene->Start();

	return ret;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	bool ret = false;

	if (current_scene != nullptr)
		ret = current_scene->PreUpdate();

	return ret;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	bool ret = false;

	if (current_scene != nullptr)
		ret = current_scene->Update(dt);

	// Blit different layers
	DoLayerBlit();
	// ---------------------
	
	return ret;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = false;

	if (current_scene != nullptr)
		ret = current_scene->PostUpdate();

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == key_down)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	bool ret = false;

	if (current_scene != nullptr)
		ret = current_scene->CleanUp();

	return ret;
}

void j1Scene::ChangeScene(Scene * new_scene)
{
	LOG("Changing current scene");

	Scene* prev_scene = current_scene;
	current_scene = new_scene;
	prev_scene->CleanUp();
	current_scene->Start();
}

Scene * j1Scene::GetCurrentScene()
{
	return current_scene;
}

void j1Scene::LayerBlit(int layer, SDL_Texture * texture, iPoint pos, const SDL_Rect section, float scale, SDL_RendererFlip flip, double angle, int pivot_x, int pivot_y)
{
	layer_blit lblit(texture, pos, section, scale, flip, angle, pivot_x, pivot_y, layer);
	layer_list.push(lblit);
}

void j1Scene::OnCollision(Collider* c1, Collider* c2)
{
	// CONVERT
	/*for(list<Scene*>::iterator it = scenes.begin(); it != scenes.end(); it++)
		(*it)->OnColl(bodyA, bodyB, fixtureA, fixtureB);*/
}

void j1Scene::DoLayerBlit()
{
	while(layer_list.size() > 0)
	{
		layer_blit current = layer_list.top();
		layer_list.pop();
		App->render->Blit(current.texture, current.pos.x, current.pos.y, &current.section, current.scale, current.flip, current.angle, current.pivot_x, current.pivot_y);
	}
}

