/*
 *  MomentumStrat3.h
 *  Momentum
 *
 *  Created by Peter Harrington on 6/28/11.
 *  Copyright 2011 Clean Micro, LLC. All rights reserved.
 *
 */
#ifndef MOMENT3_STRATEGY_H
#define MOMENT3_STRATEGY_H 1

#include <deque>
#include "Strategy.h"
#include <math.h>
#include <cmath>

class MomentumStrat3 : public Strategy
{
	int barsSeen;  //stores how many bars we have seen
	int currPos;   //stores current position -1, 0, +1
	double prevClose, entryPrice, entryThresh, stopLoss, profitObj; 
	
public:
	MomentumStrat3(double entryThreshIn, double stopLossIn, double ratioIn);
	virtual ~MomentumStrat3() {}
	virtual int execute(timeSlice currentBar);
};
#endif
