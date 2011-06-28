/*
 *  Strategy.h
 *  AutoTrade
 *
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
