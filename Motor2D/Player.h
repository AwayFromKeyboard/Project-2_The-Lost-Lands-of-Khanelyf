#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "j1Module.h"
#include "SDL/include/SDL.h"

#include "Point.h"

class Unit;

class Player : public j1Module {
public:
	Player();
	~Player();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

private:
	void MoveToTile(iPoint tile);
	void SetAttackingEnemy(Unit* enemy);

};


#endif // !_PLAYER_H_
