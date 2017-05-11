#include "Animation.h"
#include "Functions.h"
#include "Log.h"
#include "Unit.h"
#include "Player.h"
#include "Fire.h"

Animation::Animation() : frames(5), speed(1.0f), curr_frame(0), loop(true), loops(0)
{
}

Animation::Animation(const char* _name, list<SDL_Rect>& rects, float speed, bool loop) : speed(speed), loop(loop)
{

	for (list<SDL_Rect>::iterator it = rects.begin(); it != rects.end(); it++)
		frames.push_back(*it);

	name = _name;
}

Animation::~Animation()
{
}

SDL_Rect& Animation::GetAnimationFrame(float dt)
{
	if (App->player->pause_status == false) {
		curr_frame += (speed * dt);

		if (curr_frame >= frames.size())
		{
			if (!loop)
				curr_frame = frames.size() - 1;
			else
				curr_frame = 0.0f;

			loops++;
		}
	}
		int counter = 0;
		for (list<SDL_Rect>::iterator it = frames.begin(); it != frames.end(); it++, counter++)
		{
			if (counter == (int)(curr_frame))
			{
				return (*it);
			}
		}
	
	SDL_Rect ret = NULLRECT;
	return ret;
}

SDL_Rect& Animation::GetCurrentFrame()
{
	int counter = 0;
	
	for (list<SDL_Rect>::iterator it = frames.begin(); it != frames.end(); it++, counter++)
	{
		if (counter == (int)curr_frame)
		{
			return (*it);
		}
	}

	SDL_Rect ret = NULLRECT;
	return ret;
}

float Animation::GetFrameIndex() const
{
	return (int)curr_frame;
}

SDL_Rect & Animation::GetFrame(int frame)
{
	int counter = 0;

	
		for (list<SDL_Rect>::iterator it = frames.begin(); it != frames.end(); it++, counter++)
		{
			if (counter == frame)
			{
				return (*it);
			}
		}
	
		SDL_Rect ret = NULLRECT;
	
	return ret;
}

void Animation::SetCurrFrame(int frame)
{
	curr_frame = frame;
}

void Animation::SetSpeed(float _speed)
{
	speed = _speed;
}

void Animation::Pause() {

	//if (App->player->pause_status)
	
}

void Animation::SetLoop(bool _loop)
{
	loop = _loop;
}

bool Animation::IsCurrFrame(int frame)
{
	return (frame == curr_frame);
}

bool Animation::Finished() const
{
	return loops > 0;
}

void Animation::Reset()
{
	curr_frame = 0.0f;
	loops = 0;
}

const char* Animation::GetName()
{
	if (name.size() > 0)
		return name.c_str();
	else
		return "";
}

Animator::Animator()
{
}

Animator::~Animator()
{
	for (list<Animation*>::iterator it = animations.begin(); it != animations.end(); it++)
		delete (*it);
}

void Animator::AddAnimation(Animation* animation)
{
	animations.push_back(animation);
}

void Animator::LoadAnimationsFromXML(pugi::xml_node & node)
{
	for (pugi::xml_node anim = node.child("file").child("animations").child("anim"); anim != NULL; anim = anim.next_sibling("anim"))
	{
		list<SDL_Rect> anim_rects;
		float speed = anim.attribute("speed").as_float(1.0f);
		string name = anim.attribute("name").as_string("null");
		bool loop = anim.attribute("loop").as_bool(true);

		for (pugi::xml_node frame = anim.child("rect"); frame != NULL; frame = frame.next_sibling("rect"))
		{
			SDL_Rect new_frame = { frame.attribute("x").as_int(0),frame.attribute("y").as_int(0),frame.attribute("w").as_int(0),frame.attribute("h").as_int(0) };
			anim_rects.push_back(new_frame);
		}

		Animation* animation = new Animation(name.c_str(), anim_rects, speed, loop);
		AddAnimation(animation);
	}
}

void Animator::LoadFireAnimationsFromParticlesXML(pugi::xml_node & node, Fire* fire)
{
	pugi::xml_node _node;
	fire->set_0.frames.clear();
	fire->set_1.frames.clear();
	fire->set_2.frames.clear();
	fire->set_3.frames.clear();
	fire->set_4.frames.clear();

	for (pugi::xml_node rect = node.child("set_0").child("rect"); rect != NULL; rect = rect.next_sibling("rect")) {
		fire->set_0.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("set_0").attribute("w").as_int(), node.child("set_0").attribute("h").as_int() });
		fire->set_0.SetSpeed(node.child("set_0").attribute("speed").as_float());
	}
	for (pugi::xml_node rect = node.child("set_1").child("rect"); rect != NULL; rect = rect.next_sibling("rect")) {
		fire->set_1.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("set_1").attribute("w").as_int(), node.child("set_1").attribute("h").as_int() });
		fire->set_1.SetSpeed(node.child("set_1").attribute("speed").as_float());
	}
	for (pugi::xml_node rect = node.child("set_2").child("rect"); rect != NULL; rect = rect.next_sibling("rect")) {
		fire->set_2.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("set_2").attribute("w").as_int(), node.child("set_2").attribute("h").as_int() });
		fire->set_2.SetSpeed(node.child("set_2").attribute("speed").as_float());
	}
	for (pugi::xml_node rect = node.child("set_3").child("rect"); rect != NULL; rect = rect.next_sibling("rect")) {
		fire->set_3.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("set_3").attribute("w").as_int(), node.child("set_3").attribute("h").as_int() });
		fire->set_3.SetSpeed(node.child("set_3").attribute("speed").as_float());
	}
	for (pugi::xml_node rect = node.child("set_4").child("rect"); rect != NULL; rect = rect.next_sibling("rect")) {
		fire->set_4.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("set_4").attribute("w").as_int(), node.child("set_4").attribute("h").as_int() });
		fire->set_4.SetSpeed(node.child("set_4").attribute("speed").as_float());
	}
}

void Animator::LoadAnimationsFromUnitsXML(pugi::xml_node & node, Unit* unit)
{
	unit->i_south.frames.clear();
	unit->i_south_west.frames.clear();
	unit->i_west.frames.clear();
	unit->i_north_west.frames.clear();
	unit->i_north.frames.clear();
	unit->m_south.frames.clear();
	unit->m_south_west.frames.clear();
	unit->m_west.frames.clear();
	unit->m_north_west.frames.clear();
	unit->m_north.frames.clear();
	unit->a_south.frames.clear();
	unit->a_south_west.frames.clear();
	unit->a_west.frames.clear();
	unit->a_north_west.frames.clear();
	unit->a_north.frames.clear();
	unit->d_south.frames.clear();
	unit->d_south_west.frames.clear();
	unit->d_west.frames.clear();
	unit->d_north_west.frames.clear();
	unit->d_north.frames.clear();
	unit->de_south.frames.clear();
	unit->de_south_west.frames.clear();
	unit->de_west.frames.clear();
	unit->de_north_west.frames.clear();
	unit->de_north.frames.clear();
	//Idle
	for (pugi::xml_node rect = node.child("idle").child("south").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->i_south.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("idle").attribute("w").as_int(), node.child("idle").attribute("h").as_int() });
		unit->i_south.SetSpeed(node.child("idle").attribute("speed").as_float());
	}
	for (pugi::xml_node rect = node.child("idle").child("south_west").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->i_south_west.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("idle").attribute("w").as_int(), node.child("idle").attribute("h").as_int() });
		unit->i_south_west.SetSpeed(node.child("idle").attribute("speed").as_float());
	}
	for (pugi::xml_node rect = node.child("idle").child("west").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->i_west.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("idle").attribute("w").as_int(), node.child("idle").attribute("h").as_int() });
		unit->i_west.SetSpeed(node.child("idle").attribute("speed").as_float());
	}
	for (pugi::xml_node rect = node.child("idle").child("north_west").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->i_north_west.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("idle").attribute("w").as_int(), node.child("idle").attribute("h").as_int() });
		unit->i_north_west.SetSpeed(node.child("idle").attribute("speed").as_float());
	}
	for (pugi::xml_node rect = node.child("idle").child("north").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->i_north.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("idle").attribute("w").as_int(), node.child("idle").attribute("h").as_int() });
		unit->i_north.SetSpeed(node.child("idle").attribute("speed").as_float());
	}
	for (pugi::xml_node rect = node.child("move").child("south").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->m_south.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("move").attribute("w").as_int(), node.child("move").attribute("h").as_int() });
		unit->m_south.SetSpeed(node.child("move").attribute("speed").as_float());
	}
	for (pugi::xml_node rect = node.child("move").child("south_west").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->m_south_west.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("move").attribute("w").as_int(), node.child("move").attribute("h").as_int() });
		unit->m_south_west.SetSpeed(node.child("move").attribute("speed").as_float());
	}
	for (pugi::xml_node rect = node.child("move").child("west").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->m_west.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("move").attribute("w").as_int(), node.child("move").attribute("h").as_int() });
		unit->m_west.SetSpeed(node.child("move").attribute("speed").as_float());
	}
	for (pugi::xml_node rect = node.child("move").child("north_west").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->m_north_west.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("move").attribute("w").as_int(), node.child("move").attribute("h").as_int() });
		unit->m_north_west.SetSpeed(node.child("move").attribute("speed").as_float());
	}
	for (pugi::xml_node rect = node.child("move").child("north").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->m_north.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("move").attribute("w").as_int(), node.child("move").attribute("h").as_int() });
		unit->m_north.SetSpeed(node.child("move").attribute("speed").as_float());
	}
	for (pugi::xml_node rect = node.child("attack").child("south").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->a_south.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("attack").attribute("w").as_int(), node.child("attack").attribute("h").as_int() });
		unit->a_south.SetSpeed(node.child("attack").attribute("speed").as_float());
	}
	for (pugi::xml_node rect = node.child("attack").child("south_west").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->a_south_west.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("attack").attribute("w").as_int(), node.child("attack").attribute("h").as_int() });
		unit->a_south_west.SetSpeed(node.child("attack").attribute("speed").as_float());
	}
	for (pugi::xml_node rect = node.child("attack").child("west").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->a_west.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("attack").attribute("w").as_int(), node.child("attack").attribute("h").as_int() });
		unit->a_west.SetSpeed(node.child("attack").attribute("speed").as_float());
	}
	for (pugi::xml_node rect = node.child("attack").child("north_west").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->a_north_west.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("attack").attribute("w").as_int(), node.child("attack").attribute("h").as_int() });
		unit->a_north_west.SetSpeed(node.child("attack").attribute("speed").as_float());
	}
	for (pugi::xml_node rect = node.child("attack").child("north").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->a_north.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("attack").attribute("w").as_int(), node.child("attack").attribute("h").as_int() });
		unit->a_north.SetSpeed(node.child("attack").attribute("speed").as_float());
	}
	for (pugi::xml_node rect = node.child("death").child("south").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->d_south.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("death").attribute("w").as_int(), node.child("death").attribute("h").as_int() });
		unit->d_south.SetSpeed(node.child("death").attribute("speed").as_float());
		unit->d_south.SetLoop(false);
	}
	for (pugi::xml_node rect = node.child("death").child("south_west").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->d_south_west.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("death").attribute("w").as_int(), node.child("death").attribute("h").as_int() });
		unit->d_south_west.SetSpeed(node.child("death").attribute("speed").as_float());
		unit->d_south_west.SetLoop(false);
	}
	for (pugi::xml_node rect = node.child("death").child("west").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->d_west.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("death").attribute("w").as_int(), node.child("death").attribute("h").as_int() });
		unit->d_west.SetSpeed(node.child("death").attribute("speed").as_float());
		unit->d_west.SetLoop(false);

	}
	for (pugi::xml_node rect = node.child("death").child("north_west").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->d_north_west.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("death").attribute("w").as_int(), node.child("death").attribute("h").as_int() });
		unit->d_north_west.SetSpeed(node.child("death").attribute("speed").as_float());
		unit->d_north_west.SetLoop(false);

	}
	for (pugi::xml_node rect = node.child("death").child("north").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->d_north.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("death").attribute("w").as_int(), node.child("death").attribute("h").as_int() });
		unit->d_north.SetSpeed(node.child("death").attribute("speed").as_float());
		unit->d_north.SetLoop(false);

	}
	for (pugi::xml_node rect = node.child("decompose").child("south").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->de_south.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("decompose").attribute("w").as_int(), node.child("decompose").attribute("h").as_int() });
		unit->de_south.SetSpeed(node.child("decompose").attribute("speed").as_float());
		unit->de_south.SetLoop(false);

	}
	for (pugi::xml_node rect = node.child("decompose").child("south_west").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->de_south_west.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("decompose").attribute("w").as_int(), node.child("decompose").attribute("h").as_int() });
		unit->de_south_west.SetSpeed(node.child("decompose").attribute("speed").as_float());
		unit->de_south_west.SetLoop(false);

	}
	for (pugi::xml_node rect = node.child("decompose").child("west").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->de_west.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("decompose").attribute("w").as_int(), node.child("decompose").attribute("h").as_int() });
		unit->de_west.SetSpeed(node.child("decompose").attribute("speed").as_float());
		unit->de_west.SetLoop(false);

	}
	for (pugi::xml_node rect = node.child("decompose").child("north_west").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->de_north_west.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("decompose").attribute("w").as_int(), node.child("decompose").attribute("h").as_int() });
		unit->de_north_west.SetSpeed(node.child("decompose").attribute("speed").as_float());
		unit->de_north_west.SetLoop(false);

	}
	for (pugi::xml_node rect = node.child("decompose").child("north").child("rect"); rect; rect = rect.next_sibling("rect"))
	{
		unit->de_north.frames.push_back({ rect.attribute("x").as_int(), rect.attribute("y").as_int(), node.child("decompose").attribute("w").as_int(), node.child("decompose").attribute("h").as_int() });
		unit->de_north.SetSpeed(node.child("decompose").attribute("speed").as_float());
		unit->de_north.SetLoop(false);

	}
}

void Animator::SetAnimation(const char* name)
{
	if (next_animation != nullptr && TextCmp(next_animation->GetName(), name))
		return;

	for (list<Animation*>::iterator it = animations.begin(); it != animations.end(); it++)
	{
		if (TextCmp(name, (*it)->GetName()))
		{
			next_animation = (*it);
			break;
		}
	}
}

void Animator::SetAnimationTransition(const char * transition_name, const char * a1, const char * a2)
{
	anim_trans at(transition_name, a1, a2);
	anim_trans_list.push_back(at);
}

Animation* Animator::GetAnimation(const char * name)
{
	Animation* ret = nullptr;

	for (list<Animation*>::iterator it = animations.begin(); it != animations.end(); it++)
	{
		if (TextCmp(name, (*it)->GetName()))
		{
			ret = (*it);
			break;
		}
	}

	return ret;
}

Animation* Animator::GetCurrentAnimation()
{
	for (list<anim_trans>::iterator it = anim_trans_list.begin(); it != anim_trans_list.end(); it++)
	{
		if (TextCmp(next_animation->GetName(), (*it).a2.c_str()) && TextCmp(current_animation->GetName(), (*it).a1.c_str()))
		{
			if (!GetAnimation((*it).transition_name.c_str())->Finished())
				return GetAnimation((*it).transition_name.c_str());
			else
				GetAnimation((*it).transition_name.c_str())->Reset();
		}
	}

	if(current_animation != next_animation || current_animation == nullptr || anim_trans_list.size() == 0)
		current_animation = next_animation;

	return current_animation;
}

bool Animator::IsCurrentAnimation(const char * name)
{
	if (current_animation != nullptr)
	{
		if (TextCmp(current_animation->GetName(), name))
			return true;
	}
	return false;
}
