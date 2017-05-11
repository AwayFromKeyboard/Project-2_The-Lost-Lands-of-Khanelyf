#include "Fire.h"
#include "Point.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1App.h"
#include "Functions.h"
#include "Log.h"
#include "j1Textures.h"

Fire::Fire(int set)
{
	this->set = set;
	type = particle_type::fire;
}

Fire::~Fire()
{
}

bool Fire::LoadEntity(iPoint pos)
{
	bool ret = true;

	pugi::xml_document doc;
	pugi::xml_node node;
	App->LoadXML("Particles.xml", doc);

	for (pugi::xml_node particle = doc.child("particles").child("particle"); particle; particle = particle.next_sibling("particle"))
	{
		if (TextCmp(particle.attribute("type").as_string(), "Little_Fire") && type == particle_type::fire)
		{
			node = particle;
			break;
		}
	}
	if (node)
	{
		position = { pos.x, pos.y };
		entity_texture = App->tex->LoadTexture(node.child("texture").attribute("value").as_string());
		node = node.child("animations");
		animator->LoadFireAnimationsFromParticlesXML(node, this);

		switch (set)
		{
		case 0:
			current_animation = &set_0;
			break;
		case 1:
			current_animation = &set_1;
			break;
		case 2:
			current_animation = &set_2;
			break;
		case 3:
			current_animation = &set_3;
			break;
		case 4:
			current_animation = &set_4;
			break;
		}

	}
	
	return ret;
}
