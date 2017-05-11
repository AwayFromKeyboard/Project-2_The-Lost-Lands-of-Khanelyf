#include "Entity.h"
#include "j1Scene.h"

void Entity::LifeBar(iPoint size, iPoint offset)
{
	if (selected || life != max_life || type == entity_type::enemy)
	{
		SDL_Rect rect = { position.x + offset.x, position.y + offset.y, size.x, size.y };
		SDL_Rect life_rect = { rect.x, rect.y, rect.w, rect.h};

		life_rect.w = (rect.w*life) / max_life;
		if (life_rect.w < 0)
			life_rect.w = 0;

		// Back bar
		App->scene->LayerDrawQuad(rect, 30, 30, 30, 255, true, true, 10);

		if (type == entity_type::enemy || type == entity_type::enemy_building)
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
