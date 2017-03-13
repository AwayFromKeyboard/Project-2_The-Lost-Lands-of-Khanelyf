#ifndef __j1TIMER_H__
#define __j1TIMER_H__

#include "Defs.h"

class j1Timer
{
public:

	// Constructors
	j1Timer();
	~j1Timer();

	void Start();
	uint32 Read() const;
	float ReadSec() const;

private:
	uint32	started_at = 0;
};

#endif //__j1TIMER_H__