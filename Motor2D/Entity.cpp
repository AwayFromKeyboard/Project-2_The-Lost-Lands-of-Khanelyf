#include "Entity.h"
#include "j1Scene.h"
#include "SceneTest.h"

void Entity::LifeBar(iPoint size, iPoint offset)
{
	if (selected || type == entity_type::enemy || type == entity_type::enemy_building || type == entity_type::enemy_boss || life != max_life)
	{
		SDL_Rect rect = { position.x + offset.x, position.y + offset.y, size.x, size.y };
		SDL_Rect life_rect = { rect.x, rect.y, rect.w, rect.h};

		life_rect.w = (rect.w*life) / max_life;
		if (life_rect.w < 0)
			life_rect.w = 0;

		// Back bar
		App->scene->LayerDrawQuad(rect, 30, 30, 30, 255, true, true, 10);

		if (type == entity_type::enemy || type == entity_type::enemy_building || type == entity_type::enemy_boss)
		{
			if (life >= max_life * 50 / 100) // Life bar >=50%
				App->scene->LayerDrawQuad(life_rect, 255, 0, 0, 255, true, true, 11);
			else if (life_rect.w >= rect.w * 25 / 100) // Life bar >=25%
				App->scene->LayerDrawQuad(life_rect, 200, 51, 51, 255, true, true, 11);
			else // Life bar >0%
				App->scene->LayerDrawQuad(life_rect, 150, 51, 51, 255, true, true, 11);
		}
		else
		{
			if (life >= max_life * 50 / 100) // Life bar >=50%
				App->scene->LayerDrawQuad(life_rect, 0, 255, 0, 255, true, true, 11);
			else if (life_rect.w >= rect.w * 25 / 100) // Life bar >=25%
				App->scene->LayerDrawQuad(life_rect, 255, 164, 32, 255, true, true, 11);
			else // Life bar >0%
				App->scene->LayerDrawQuad(life_rect, 255, 0, 0, 255, true, true, 11);
		}
		
	}
}

void Entity::CheckIDInRect()
{
	if (type == entity_type::enemy) {
		if (position.PointInRect(App->scene->scene_test->quest_0.x, App->scene->scene_test->quest_0.y, App->scene->scene_test->quest_0.w, App->scene->scene_test->quest_0.h)) {
			id = entity_quest_id::quest_0;
		}
		else if (position.PointInRect(App->scene->scene_test->quest_2.x, App->scene->scene_test->quest_2.y, App->scene->scene_test->quest_2.w, App->scene->scene_test->quest_2.h)) {
			id = entity_quest_id::quest_2;
		}
		else if (position.PointInRect(App->scene->scene_test->quest_3.x, App->scene->scene_test->quest_3.y, App->scene->scene_test->quest_3.w, App->scene->scene_test->quest_3.h)) {
			id = entity_quest_id::quest_3;
		}
	}
	else if (type == entity_type::enemy_building) {
		if (position.PointInRect(App->scene->scene_test->quest_4.x, App->scene->scene_test->quest_4.y, App->scene->scene_test->quest_4.w, App->scene->scene_test->quest_4.h)) {
			id = entity_quest_id::quest_4;
		}
	}
}
