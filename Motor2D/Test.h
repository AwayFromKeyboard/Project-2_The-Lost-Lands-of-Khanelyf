#ifndef _Test_
#define _Test_

#include "Unit.h"
#define	COLLISION_ADJUSTMENT iPoint(23, 25)
#define Y_OFFSET 32 // Number to add in blit for pushing the texture up
#define X_OFFSET 32 // Number to add in blit for pushing the texture up

class GameObject;
class Test : public Unit
{
public:
	Test();
	~Test();

	// Load animations
	bool LoadEntity();

	// Start variables
	bool Start();

	// Draw and change animations
	bool Draw(float dt);

	// Post Update
	bool PostUpdate();

	// CleanUp
	bool CleanUp();

	// On Collision
	void OnColl(PhysBody* bodyA, PhysBody* bodyB, b2Fixture* fixtureA, b2Fixture* fixtureB);

public:
	iPoint mouse_position;

	int life = 0;
	int cost = 0;
	float speed = 0;
	int damage = 0;
	int armor = 0;
	int  pierce_armor = 0;
	int range = 0;

	Animation* i_south;
	Animation* i_south_west;
	Animation* i_west;
	Animation* i_north_west;
	Animation* i_north;
};

#endif