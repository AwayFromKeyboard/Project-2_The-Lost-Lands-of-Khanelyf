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

Test::Test()
{
}

Test::~Test()
{
}

bool Test::LoadEntity()
{
	bool ret = true;

	test_go = new GameObject(iPoint(150, 150), App->cf->CATEGORY_PLAYER, App->cf->MASK_PLAYER, pbody_type::p_t_player, 0);

	test_go->CreateCollision(COLLISION_ADJUSTMENT, 17, 35, fixture_type::f_t_null);
	test_go->SetListener((j1Module*)App->entity);
	test_go->SetFixedRotation(true);

	test_go->SetTexture(App->tex->LoadTexture("Vilager.png"));

	pugi::xml_document doc;
	App->LoadXML("test.xml", doc);
	test_go->LoadAnimationsFromXML(doc);
	
	test_go->SetAnimation("south");

	last_height = test_go->fGetPos().y;

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

	if ((App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT))
	{
		test_go->SetPos({ test_go->fGetPos().x - speed, test_go->fGetPos().y - speed });
	}
	else if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT))
	{
		test_go->SetPos({ test_go->fGetPos().x + speed, test_go->fGetPos().y - speed });
	}
	else if ((App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT))
	{
		test_go->SetPos({ test_go->fGetPos().x - speed, test_go->fGetPos().y + speed });
	}
	else if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT))
	{
		test_go->SetPos({ test_go->fGetPos().x + speed, test_go->fGetPos().y + speed });
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		test_go->SetPos({ test_go->fGetPos().x - speed, test_go->fGetPos().y });
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		test_go->SetPos({ test_go->fGetPos().x + speed, test_go->fGetPos().y });
	}
	else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		test_go->SetPos({ test_go->fGetPos().x, test_go->fGetPos().y - speed });
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		test_go->SetPos({ test_go->fGetPos().x, test_go->fGetPos().y + speed });
	}

	return ret;
}

bool Test::Draw(float dt)
{
	bool ret = true;

	if ((App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT))
	{
		test_go->SetAnimation("north_west");
	}
	else if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT))
	{
		test_go->SetAnimation("north_east");
	}
	else if ((App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT))
	{
		test_go->SetAnimation("south_west");
	}
	else if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT))
	{
		test_go->SetAnimation("south_east");
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		test_go->SetAnimation("west");
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		test_go->SetAnimation("east");
	}
	else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		test_go->SetAnimation("nord");
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		test_go->SetAnimation("south");
	}

	App->scene->LayerBlit(5, test_go->GetTexture(), { test_go->GetPos().x, test_go->GetPos().y}, test_go->GetCurrentAnimationRect(dt));

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

	return ret;
}

void Test::OnColl(PhysBody* bodyA, PhysBody * bodyB, b2Fixture * fixtureA, b2Fixture * fixtureB)
{

}