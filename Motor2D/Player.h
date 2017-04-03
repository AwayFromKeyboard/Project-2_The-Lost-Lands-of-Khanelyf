#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "j1Module.h"
#include "SDL/include/SDL.h"

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

};


#endif // !_PLAYER_H_
