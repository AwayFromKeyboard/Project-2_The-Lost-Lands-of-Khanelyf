#include "CursorAnimations.h"
#include "Point.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1App.h"
#include "Functions.h"
#include "Log.h"
#include "j1Textures.h"
#include "j1Scene.h"

CursorAnimations::CursorAnimations(int set)
{
	this->set = set;
	type = particle_type::cursor;
}

CursorAnimations::~CursorAnimations()
{
}

bool CursorAnimations::Draw(float dt)
{
	App->scene->LayerBlit(10, particle_texture, { position.x + offset.x, position.y + offset.y }, current_animation->GetAnimationFrame(dt), -1.0);

	return true;
}

bool CursorAnimations::Update(float dt)
{
	if (current_animation->Finished()) {
		to_delete = true;
	}


	return true;
}

bool CursorAnimations::LoadParticle(iPoint pos)
{
	bool ret = true;

	pugi::xml_document doc;
	pugi::xml_node node;
	App->LoadXML("Particles.xml", doc);

	for (pugi::xml_node particle = doc.child("particles").child("particle"); particle; particle = particle.next_sibling("particle"))
	{
		if (TextCmp(particle.attribute("type").as_string(), "Cursor_Animations") && type == particle_type::cursor)
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
		animator->LoadCursorAnimationsFromParticlesXML(node, this);

		switch (set)
		{
		case 0:
			current_animation = &movement;
			break;
		}

	}

	return ret;
}
