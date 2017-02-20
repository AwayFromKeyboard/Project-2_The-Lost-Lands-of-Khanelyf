#ifndef _Test_
#define _Test_

#include "Entity.h"
#define	COLLISION_ADJUSTMENT iPoint(7, 15)

class GameObject;
class Test : public Entity
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
	GameObject* test_go = nullptr;

	float last_height = 0.0f;
};

#endif