#ifndef _Test_
#define _Test_

#include "Unit.h"
#define	COLLISION_ADJUSTMENT iPoint(23, 25)
#define T_HEIGHT -35 // Number to add in blit for pushing the texture up

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

	// PreUpdate
	bool PreUpdate();

	// Update
	bool Update(float dt);

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
};

#endif