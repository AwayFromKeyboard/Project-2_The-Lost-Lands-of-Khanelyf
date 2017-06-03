#include "UndWillBuf.h"
#include "Point.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1App.h"
#include "Functions.h"
#include "Log.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "Player.h"

UndWillBuff::UndWillBuff(int set)
{
	this->set = set;
	type = particle_type::undwillbuff;
}

UndWillBuff::~UndWillBuff()
{
}

bool UndWillBuff::Draw(float dt)
{
	App->scene->LayerBlit(4, particle_texture, { position.x + offset.x, position.y + offset.y }, current_animation->GetAnimationFrame(dt), -1.0);

	return true;
}

bool UndWillBuff::Update(float dt)
{
	if (!App->player->undying_state_active) {
		to_delete = true;
	}

	return true;
}

bool UndWillBuff::LoadParticle(iPoint pos)
{
	bool ret = true;

	pugi::xml_document doc;
	pugi::xml_node node;
	App->LoadXML("Particles.xml", doc);

	for (pugi::xml_node particle = doc.child("particles").child("particle"); particle; particle = particle.next_sibling("particle"))
	{
		if (TextCmp(particle.attribute("type").as_string(), "UndWill_Buff") && type == particle_type::undwillbuff)
		{
			node = particle;
			break;
		}
	}
	if (node)
	{
		position = { pos.x, pos.y };
		offset = { node.child("offset").attribute("x").as_int(), node.child("offset").attribute("y").as_int() };
		particle_texture = App->tex->LoadTexture(node.child("texture").attribute("path").as_string());
		node = node.child("animations");
		animator->LoadUndWillBuffFromParticlesXML(node, this);

		switch (set)
		{
		case 0:
			current_animation = &movement;
			break;
		}

	}

	return ret;
}
