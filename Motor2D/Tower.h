#ifndef _TOWER_H_
#define _TOWER_H_

#include "Building.h"

class Tower : public Building
{

public:
	Tower(entity_type _type);
	~Tower();

	bool LoadEntity(iPoint pos); // rect_number 1,2 or 3
};