#include "Entity.h"
#include "j1Scene.h"

void Entity::LifeBar(iPoint size, iPoint offset)
{
	if (selected )
	{
		SDL_Rect rect = { pos2.x + offset.x, pos2.y + offset.y, size.x, size.y };
		SDL_Rect life_rect = { rect.x, rect.y, rect.w, rect.h};

		life_rect.w = (rect.w*life) / max_life;
		if (life_rect.w < 0)
			life_rect.w = 0;

		// Back bar
		App->scene->LayerDrawQuad(rect, 30, 30, 30, 255, true, true, 10);

		if (life >= max_life * 50 / 100) // Life bar green
			App->scene->LayerDrawQuad(life_rect, 0, 255, 0, 255, true, true, 11);
		else if (life_rect.w >= rect.w * 25 / 100) // Life bar orange
			App->scene->LayerDrawQuad(life_rect, 255, 164, 32, 255, true, true, 11);
		else // Life bar red
			App->scene->LayerDrawQuad(life_rect, 255, 0, 0, 255, true, true, 11);

	}
}
