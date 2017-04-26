#include "Entity.h"
#include "j1Scene.h"

void Entity::LifeBar(iPoint size, iPoint offset)
{
	if (show_life_bar)
	{
		SDL_Rect rect = { pos2.x + offset.x, pos2.y + offset.y, size.x, size.y };
		SDL_Rect life_rect = rect;

		life_rect.w = (rect.w*life) / max_life;
		if (life_rect.w < 0)
			life_rect.w = 0;

		// Back bar
		App->scene->LayerDrawQuad(rect, 30, 30, 30, 255, true, 100);



	}
}
