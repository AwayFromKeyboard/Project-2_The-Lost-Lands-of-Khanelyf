#ifndef _BLACKSMITH_H_
#define _BLACKSMITH_H_

#include "Building.h"

class Blacksmith : public Building
{

public:
	Blacksmith(entity_type _type);
	~Blacksmith();

	bool LoadEntity(iPoint pos, entity_name name); // rect_number 1,2 or 3
};

#endif
