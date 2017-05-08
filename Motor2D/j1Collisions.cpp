#include "Defs.h"
#include "Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "QuadTree.h"
#include "j1Window.h"
#include "j1Map.h"


j1Collisions::j1Collisions()
{
	matrix[COLLIDER_UNIT][COLLIDER_UNIT] = true;
}

j1Collisions::~j1Collisions()
{

}

bool j1Collisions::Start() 
{
	quadTree = nullptr;
	return true;
}

bool j1Collisions::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (std::list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
	{
		if ((*it) != nullptr && (*it)->to_delete == true)
		{
			RELEASE (*it);
			it = colliders.erase(it);
		}	
	}

	return true;
}

bool j1Collisions::Update(float dt)
{
	Collider* col1;
	Collider* col2;

	//for (std::list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it) 
	//{
	//	// skip empty colliders
	//	if ((*it) == nullptr)
	//		continue;

	//	col1 = (*it);

	//	// avoid checking collisions already checked
	//	for (std::list<Collider*>::iterator it2 = colliders.begin(); it2 != colliders.end(); ++it2)
	//	{
	//		// skip empty colliders
	//		if ((*it2) == nullptr)
	//			continue;

	//		col2 = (*it2);

	//		if (col1->CheckCollision(col2->rect) == true)
	//		{
	//			if (matrix[col1->type][col2->type] && col1->callback)
	//				col1->callback->OnCollision(col1, col2);
	//			if (matrix[col2->type][col1->type] && col2->callback)
	//				col2->callback->OnCollision(col2, col1);

	//		}
	//	}
	//}


	quadTree->ClearTree();

	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); it++) {
		quadTree->Insert(*it);
	}

	quadTreeChecks = 0;


	nodeList.clear();
	quadTree->GetNodes(nodeList);

	return true;
}

void j1Collisions::DebugDraw()
{
	if (debug == true)
	{
		Uint8 alpha = 80;
		for (std::list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		{
			if ((*it) == nullptr)
				continue;

			if ((*it)->print_collider == true)
			{
				switch ((*it)->type)
				{
				case COLLIDER_NONE: // white
					App->render->DrawQuad((*it)->rect, 255, 255, 255, alpha, false);
					break;
				case COLLIDER_UNIT: // yellow
					App->render->DrawQuad((*it)->rect, 255, 255, 0, alpha, false);
					break;
				}
			}
		}

		for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); it++) {
			potentialCollisionList.clear();
			quadTree->Retrieve(potentialCollisionList, *it);
			if (potentialCollisionList.size() > 0) {
				for (list<Collider*>::iterator it2 = potentialCollisionList.begin(); it2 != potentialCollisionList.end(); it2++) {
					if ((*it)->CheckCollision((*it2)->rect)) {
						if (matrix[(*it)->type][(*it2)->type] && (*it)->callback)
							(*it)->callback->OnCollision((*it), (*it2));

						if (matrix[(*it2)->type][(*it)->type] && (*it2)->callback)
							(*it2)->callback->OnCollision((*it2), (*it));
					}
					quadTreeChecks++;
					App->render->DrawQuad((*it2)->rect, 0, 255, 0, 255, false);
					App->render->DrawLine((*it)->rect.x + (*it)->rect.w / 2, (*it)->rect.y + (*it)->rect.h / 2, (*it2)->rect.x + (*it2)->rect.w / 2, (*it2)->rect.y + (*it2)->rect.h / 2, 255, 255, 255, 255);
				}
			}
		}

		for (int i = 0; i < nodeList.size(); i++) {
			if (nodeList[i] != nullptr) {
				App->render->DrawQuad(nodeList[i]->nodeRect, 255, 255, 255, 255, false);
			}
		}
	}
}

bool j1Collisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (std::list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)

	{
		if ((*it) != nullptr)
		{
			RELEASE (*it);
			it = colliders.erase(it);
		}
	}

	return true;
}

void j1Collisions::UpdateQuadtree() {
	if (quadTree != nullptr) {
		delete quadTree;
	}
	quadTree = new QuadTree({ -App->map->data.height * App->map->data.tile_height + App->map->data.tile_height, 17,
		App->map->data.width * App->map->data.tile_width, App->map->data.height * App->map->data.tile_height });
}

Collider* j1Collisions::AddCollider(SDL_Rect rect, collider_type type, j1Module* callback)
{
	Collider* ret = new Collider(rect, type, callback);
	colliders.push_back(ret);
	
	return ret;
}

bool j1Collisions::EraseCollider(Collider* collider)
{
	for (std::list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
	{
		if ((*it) == collider)
		{
			RELEASE (*it);
			it = colliders.erase(it);
			return true;
		}
	}

	return false;
}

///

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}