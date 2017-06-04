#ifndef _ANIMATION__H__
#define _ANIMATION__H__

#include "j1App.h"
#include "j1Textures.h"
#include "SDL\include\SDL.h"

struct SDL_Texture;
class Animation;
class Unit;
class Fire;
class CursorAnimations;
class BattlecryBuff;
class UndWillBuff;

// -----------------------------------------
// -----------------------------------------

	    // ANIMATIOR / ANIMATION //

// -----------------------------------------
// -----------------------------------------

struct anim_trans
{
	anim_trans() {};
	anim_trans(const char* _transition_name, const char* _a1, const char* _a2)
	{
		transition_name =  _transition_name;
		a1 = _a1;
		a2 = _a2;
	}
	string transition_name;
	string a1;
	string a2;
};

class Animator
{
public:
	Animator();
	~Animator();

	// Add an animation
	void AddAnimation(Animation* animation);

	// Load Animations from XML
	void LoadAnimationsFromXML(pugi::xml_node &node);
	void LoadFireAnimationsFromParticlesXML(pugi::xml_node & node, Fire * fire);
	void LoadAnimationsFromUnitsXML(pugi::xml_node &node, Unit* unit);
	void LoadCursorAnimationsFromParticlesXML(pugi::xml_node &node, CursorAnimations* cursor_anim);
	void LoadBattlecryBuffFromParticlesXML(pugi::xml_node &node, BattlecryBuff* battlecry_buff);
	void LoadUndWillBuffFromParticlesXML(pugi::xml_node &node, UndWillBuff* undwill_buff);
	
	// Set a current animation
	void SetAnimation(const char* name);

	void SetAnimationTransition(const char* transition_name, const char* a1, const char* a2);

	// Returns the animation using the name
	Animation* GetAnimation(const char* name);

	Animation* GetCurrentAnimation();

	bool IsCurrentAnimation(const char * name);

private:
	Animation*           current_animation = nullptr;
	Animation*			 next_animation = nullptr;
	list<Animation*>     animations;
	list<anim_trans>     anim_trans_list;
};

class Animation
{
public:
	Animation();
	Animation(const char* name, list<SDL_Rect>& rects, float speed, bool loop = true);
	~Animation();

	// Gets the animation frame adding speed
	SDL_Rect& GetAnimationFrame(float dt);

	// Get the current animation frame
	SDL_Rect& GetCurrentFrame();

	// Get the current fame index
	float GetFrameIndex() const;

	// Get the current frame rect
	SDL_Rect& GetFrame(int frame);

	// Set the current frame
	void SetCurrFrame(int frame);

	// Set the frame speed
	void SetSpeed(float speed);

	void Pause();

	// Set if the animation loops
	void SetLoop(bool loop);

	// Checks if it's the current frame
	bool IsCurrFrame(int frame);

	// Checks if the animation has finished
	bool Finished() const;

	// Resets the animation
	void Reset();

	// Returns the animation name
	const char* GetName();

public:
	list<SDL_Rect>      frames;
private:
	string		    	name;
	float			    speed = 0.0f;
	bool			    loop = true;
	int				    loops = 0;
	float			    curr_frame = 0.0f;
};

#endif // !_ANIMATION__H__