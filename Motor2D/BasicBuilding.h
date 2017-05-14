#ifndef _BASICBUILDING_H_
#define _BASICBUILDING_H_

#include "Building.h"

class BasicBuilding : public Building
{

public:
	BasicBuilding(entity_type _type, int _building_rect_number); // rect_number 1,2 or 3
	~BasicBuilding();

	bool LoadEntity(iPoint pos, entity_name name);
};

#endif