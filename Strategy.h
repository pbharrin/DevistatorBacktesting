/*
 *  Strategy.h
 *  AutoTrade
 *
 *  Created by marc on 1/17/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef STRATEGY_H
#define STRATEGY_H 1

#include "histTextLoad.h"

class Strategy
{	
public:
	Strategy() { } // to be used later
	virtual ~Strategy() { } // to be used later
	
	virtual int execute(timeSlice currentBar) = 0;
};

#endif
