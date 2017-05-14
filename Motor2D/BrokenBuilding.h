#ifndef _BrokenBuilding_H_
#define _BrokenBuilding_H_

#include "Building.h"

class BrokenBuilding : public Building
{

public:
	BrokenBuilding(entity_type _type);
	~BrokenBuilding();

	bool LoadEntity(iPoint pos, entity_name name);

};

#endif
