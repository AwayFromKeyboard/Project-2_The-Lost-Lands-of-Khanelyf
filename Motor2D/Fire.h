#ifndef _FIRE_H_
#define _FIRE_H_

#include "Particle.h"

class Fire : public Particle {
public:
	Fire(int set);
	~Fire();

	bool LoadEntity(iPoint pos);

private:
	int set = 0;
};



#endif // !_FIRE_H_
