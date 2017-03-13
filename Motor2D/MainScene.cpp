#include "MainScene.h"
#include "j1Scene.h"
#include "Log.h"
#include "j1Input.h"
#include "Functions.h"
#include "j1Physics.h"
#include "GameObject.h"
#include "j1App.h"
#include "j1Gui.h"
#include "Parallax.h"
#include "j1Entity.h"
#include "CollisionFilters.h"


MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}

bool MainScene::Start()
{
	bool ret = true;

	LOG("Start MainScene");


	return ret;
}

bool MainScene::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool MainScene::Update(float dt)
{
	bool ret = true;

	return ret;
}

bool MainScene::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool MainScene::CleanUp()
{
	bool ret = true;


	return ret;
}

void MainScene::OnColl(Collider* c1, Collider* c2)
{

}

