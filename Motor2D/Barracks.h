#ifndef _BARRACKS_H_
#define _BARRACKS_H_

#include "Building.h"

class Barracks : public Building
{

public:
	Barracks(entity_type _type);
	~Barracks();

	bool LoadEntity(iPoint pos);

	bool Start();
};

#endif