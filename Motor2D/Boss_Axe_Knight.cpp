#include "Boss_Axe_Knight.h"
#include "CollisionFilters.h"
#include "j1Scene.h"
#include "Functions.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "Log.h"
#include "j1Collisions.h"
#include "j1Scene.h"
#include "SceneTest.h"
#include "ParticleManager.h"
#include "Particle.h"

BossAxeKnight::BossAxeKnight(entity_type _type)
{
	type = _type;
}

BossAxeKnight::~BossAxeKnight()
{
}

bool BossAxeKnight::LoadEntity(iPoint pos, entity_name name)
{
	bool ret = true;

	pugi::xml_document doc;
	pugi::xml_node node;
	App->LoadXML("Units.xml", doc);
	for (pugi::xml_node unit = doc.child("units").child("unit"); unit; unit = unit.next_sibling("unit"))
	{
		if (TextCmp(unit.attribute("type").as_string(), "Axe_Knight_Boss"))
		{
			node = unit;
			break;
		}
	}
	if (node)
	{
		this->name = name;

		position = { pos.x, pos.y };
		collision = App->collisions->AddCollider({ position.x, position.y, node.child("collision_box").attribute("w").as_int(), node.child("collision_box").attribute("h").as_int() }, COLLIDER_UNIT, App->entity);
		collision->offset_x = node.child("collision_box").attribute("offset_x").as_int();
		collision->offset_y = node.child("collision_box").attribute("offset_y").as_int();
		collision->parent = this;

		cost = node.child("cost").attribute("value").as_int(0);
		human_cost = node.child("human_cost").attribute("value").as_int(0);
		gold_drop = node.child("gold_drop").attribute("value").as_int(0);
		speed = node.child("speed").attribute("value").as_float();
		damage = node.child("damage").attribute("value").as_int();
		armor = node.child("armor").attribute("value").as_int();
		pierce_armor = node.child("pierce_armor").attribute("value").as_int();
		range = node.child("range").attribute("value").as_int();
		life = node.child("life").attribute("value").as_int();
		radius_of_action = node.child("radius_of_action").attribute("value").as_int(0);

		entity_texture = App->tex->LoadTexture(node.child("texture").attribute("value").as_string());
		node = node.child("animations");
		animator->LoadAnimationsFromUnitsXML(node, this);

		i_offset.create(node.child("idle").attribute("offset_x").as_int(), node.child("idle").attribute("offset_y").as_int());
		m_offset.create(node.child("move").attribute("offset_x").as_int(), node.child("move").attribute("offset_y").as_int());
		a_offset.create(node.child("attack").attribute("offset_x").as_int(), node.child("attack").attribute("offset_y").as_int());
		d_offset.create(node.child("death").attribute("offset_x").as_int(), node.child("death").attribute("offset_y").as_int());
		de_offset.create(node.child("decompose").attribute("offset_x").as_int(), node.child("decompose").attribute("offset_y").as_int());

		flip_i_offset = node.child("idle").attribute("offset_flip").as_int();
		flip_m_offset = node.child("move").attribute("offset_flip").as_int();
		flip_a_offset = node.child("attack").attribute("offset_flip").as_int();
		flip_d_offset = node.child("death").attribute("offset_flip").as_int();
		flip_de_offset = node.child("decompose").attribute("offset_flip").as_int();

		current_animation = &i_south;
		direction = { 0, 1 };

		state = entity_state::entity_idle;
		phase = boss_phase::asleep;
		this->boss = this;
		is_boss = true;

	}
	else LOG("\nERROR, no node found\n");

	return ret;
}

void BossAxeKnight::Draw_Phase2()
{
	std::list<iPoint> frontier;

		if (position_map != range_visited.front())
		{
			range_visited.clear();
			range_visited.push_back(position_map);
			frontier.push_back(position_map);
			for (int i = 0; i < PHASE2_RANGE; ++i) {
				for (int j = frontier.size(); j > 0; j--) {
					iPoint neighbors[4];
					neighbors[0] = frontier.front() + iPoint(1, 0);
					neighbors[1] = frontier.front() + iPoint(-1, 0);
					neighbors[2] = frontier.front() + iPoint(0, 1);
					neighbors[3] = frontier.front() + iPoint(0, -1);
					frontier.pop_front();

					for (int k = 0; k < 4; k++) {
						bool is_visited = false;
						for (std::list<iPoint>::iterator it = range_visited.begin(); it != range_visited.end(); ++it) {
							if (neighbors[k] == *it) {
								is_visited = true;
								break;
							}
						}
						if (!is_visited) {
							frontier.push_back(neighbors[k]);
							range_visited.push_back(neighbors[k]);
						}
					}
				}
			}
		}
		for (std::list<iPoint>::iterator it = range_visited.begin(); it != range_visited.end(); it++) {
			App->scene->LayerBlit(200, App->scene->scene_test->debug_tex, App->map->MapToWorldPoint(*it), { 0, 0, 64, 64 });
		}
}

void BossAxeKnight::Phase2_Attack()
{
	std::list<iPoint> frontier;
	std::list<iPoint> visited;

	visited.push_back(position_map);
	frontier.push_back(position_map);

	for (int i = 0; i < PHASE2_RANGE; ++i) {
		for (int j = frontier.size(); j > 0; j--) {
			iPoint neighbors[4];
			neighbors[0] = frontier.front() + iPoint(1, 0);
			neighbors[1] = frontier.front() + iPoint(-1, 0);
			neighbors[2] = frontier.front() + iPoint(0, 1);
			neighbors[3] = frontier.front() + iPoint(0, -1);

			frontier.pop_front();

			for (int k = 0; k < 4; k++) {
				if (neighbors[k].x >= 0 && neighbors[k].y >= 0) {
					Unit* found = (Unit*)App->map->entity_matrix[neighbors[k].x][neighbors[k].y];
					if (found != nullptr && found->life > 0 && (found->type == ally || found->type == player)) {
						found->life -= PHASE2_DAMAGE;
						if (found->life <= 0)
							found->state = entity_death;
					}
					else {
						bool is_visited = false;
						for (std::list<iPoint>::iterator it = visited.begin(); it != visited.end(); ++it) {
							if (neighbors[k] == *it) {
								is_visited = true;
								break;
							}
						}
						if (!is_visited) {
							frontier.push_back(neighbors[k]);
							visited.push_back(neighbors[k]);
						}
					}
				}
			}
		}
	}
}

void BossAxeKnight::Draw_Phase3()
{
	std::list<iPoint> visited;

	visited.push_back(position);

	for (int i = 0; i < PHASE3_RANGE; ++i) {
		iPoint neighbors[12];
		neighbors[0] = position_map + iPoint(1 + i, 0);
		neighbors[1] = position_map + iPoint(-1 - i, 0);
		neighbors[2] = position_map + iPoint(0, 1 + i);
		neighbors[3] = position_map + iPoint(0, -1 - i);

		neighbors[4] = position_map + iPoint(1 + i, i);
		neighbors[5] = position_map + iPoint(-1 - i, i);
		neighbors[6] = position_map + iPoint(i, 1 + i);
		neighbors[7] = position_map + iPoint(i, -1 - i);

		neighbors[8] = position_map + iPoint(1 + i, -i);
		neighbors[9] = position_map + iPoint(-1 - i, -i);
		neighbors[10] = position_map + iPoint(-i, 1 + i);
		neighbors[11] = position_map + iPoint(-i, -1 - i);

		for (int k = 0; k < 12; k++) {
			bool is_visited = false;
			for (std::list<iPoint>::iterator it = visited.begin(); it != visited.end(); ++it) {
				if (neighbors[k] == *it) {
					is_visited = true;
					break;
				}
			}
			if (!is_visited) {
				visited.push_back(neighbors[k]);
			}
		}
	}
	for (std::list<iPoint>::iterator it = visited.begin(); it != visited.end(); it++) {
		App->scene->LayerBlit(200, App->scene->scene_test->debug_tex, App->map->MapToWorldPoint(*it), { 0, 0, 64, 64 });
	}
}

void BossAxeKnight::Phase3_Attack()
{
	std::list<iPoint> visited;

	visited.push_back(position);

	for (int i = 0; i < PHASE3_RANGE; ++i) {
		iPoint neighbors[12];
		neighbors[0] = position_map + iPoint(1 + i, 0);
		neighbors[1] = position_map + iPoint(-1 - i, 0);
		neighbors[2] = position_map + iPoint(0, 1 + i);
		neighbors[3] = position_map + iPoint(0, -1 - i);

		neighbors[4] = position_map + iPoint(1 + i, i);
		neighbors[5] = position_map + iPoint(-1 - i, i);
		neighbors[6] = position_map + iPoint(i, 1 + i);
		neighbors[7] = position_map + iPoint(i, -1 - i);

		neighbors[8] = position_map + iPoint(1 + i, -i);
		neighbors[9] = position_map + iPoint(-1 - i, -i);
		neighbors[10] = position_map + iPoint(-i, 1 + i);
		neighbors[11] = position_map + iPoint(-i, -1 - i);

		for (int k = 0; k < 12; k++) {
			bool is_visited = false;
			for (std::list<iPoint>::iterator it = visited.begin(); it != visited.end(); ++it) {
				if (neighbors[k] == *it) {
					is_visited = true;
					break;
				}
			}
			if (!is_visited) {
				visited.push_back(neighbors[k]);
			}
		}
	}
	for (std::list<iPoint>::iterator it = visited.begin(); it != visited.end(); it++) {
		Particle* part = App->particle->CreateParticle(particle_type::fire, RandomGenerate(0, 4), App->map->MapToWorldPoint(*it));
		fireballs.push_back(part);
		fireball_points.push_back(*it);
	}

}

void BossAxeKnight::Phase3_Damage()
{
	for (std::list<iPoint>::iterator it = fireball_points.begin(); it != fireball_points.end(); it++) {
		for (std::list<Entity*>::iterator it2 = App->entity->entity_list.begin(); it2 != App->entity->entity_list.end(); it2++) {
			if ((*it2)->type == entity_type::ally || (*it2)->type == entity_type::player) {
				if (App->map->WorldToMapPoint((*it2)->position) == *it) {
					(*it2)->life -= PHASE3_DAMAGE;
					if ((*it2)->life <= 0)
						(*it2)->state = entity_state::entity_death;
				}
			}
		}
	}

}
