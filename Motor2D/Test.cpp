#include "Test.h"
#include "GameObject.h"
#include "Scene.h"
#include "j1App.h"
#include "j1Input.h"
#include "CollisionFilters.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "Functions.h"
#include "j1Textures.h"
#include "j1Entity.h"

Test::Test()
{
}

Test::~Test()
{
}

bool Test::LoadEntity()
{
	bool ret = true;

	game_object = new GameObject(iPoint(150, 150), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_player, 0);

	game_object->CreateCollision(COLLISION_ADJUSTMENT, 20, 54, fixture_type::f_t_null);
	game_object->SetListener((j1Module*)App->entity);
	game_object->SetFixedRotation(true);
	
	game_object->SetTexture(App->tex->LoadTexture("Villager.png"));

	pugi::xml_document doc;
	App->LoadXML("test.xml", doc);
	game_object->LoadAnimationsFromXML(doc);
	
	game_object->SetAnimation("idle_south");

	App->entity->unit_game_objects_list.push_back(game_object);

	return ret;
}

bool Test::Start()
{
	bool ret = true;

	return ret;
}

bool Test::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool Test::Update(float dt)
{
	bool ret = true;

	float speed = (200 * dt);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == key_down) {
		iPoint mouse_position; 
		App->input->GetMousePosition(mouse_position.x, mouse_position.y);
		App->pathfinding->CreatePath(game_object->GetPos(), mouse_position);
	}

	return ret;
}

bool Test::Draw(float dt)
{
	bool ret = true;

	if (flip) {
		App->scene->LayerBlit(5, game_object->GetTexture(), { game_object->GetPos().x, game_object->GetPos().y }, game_object->GetCurrentAnimationRect(dt), -1.0, SDL_FLIP_HORIZONTAL);
	}
	else
		App->scene->LayerBlit(5, game_object->GetTexture(), { game_object->GetPos().x, game_object->GetPos().y }, game_object->GetCurrentAnimationRect(dt));

	return ret;
}

bool Test::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool Test::CleanUp()
{
	bool ret = true;

	for (std::list<GameObject*>::iterator it = App->entity->unit_game_objects_list.begin(); it != App->entity->unit_game_objects_list.end(); it++) {
		if ((*it) == game_object) {
			App->entity->unit_game_objects_list.erase(it++);
		}
	}
	

	return ret;
}

void Test::OnColl(PhysBody* bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{

}