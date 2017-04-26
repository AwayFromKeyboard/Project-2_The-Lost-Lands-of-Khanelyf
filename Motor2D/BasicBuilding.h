#ifndef _BASICBUILDING_H_
#define _BASICBUILDING_H_

#include "Building.h"

class BasicBuilding : public Building
{

public:
	BasicBuilding(entity_type _type);
	~BasicBuilding();

	bool LoadEntity(iPoint pos);

	bool Start();
};

#endif