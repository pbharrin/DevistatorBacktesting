/*
 *  MomentumStrat2.h
 *  Momentum
 *
 *  Created by Peter Harrington on 5/24/11.
 *  Copyright 2011 Clean Micro, LLC. All rights reserved.
 *
 */
#ifndef MOMENT2_STRATEGY_H
#define MOMENT2_STRATEGY_H 1

#include <deque>
#include "Strategy.h"
#include <math.h>
#include <cmath>

class MomentumStrat2 : public Strategy
{
	deque<timeSlice> lastXbars;
	int barsSeen;  //stores how many bars we have seen
	int currPos;   //stores current position -1, 0, +1
	double entryPrice, stopLoss, profitOb, rangeMult; 
	int slReduceThresh, entryTime, profObHits, stopLossHits;
	
public:
	MomentumStrat2(int lowerBound);
	virtual ~MomentumStrat2() {}
	virtual int execute(timeSlice currentBar);
};
#endif
