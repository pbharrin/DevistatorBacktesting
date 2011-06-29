/*
 *  MomentumStrat3.cpp
 *  Momentum
 *
 *  Created by Peter Harrington on 6/28/11.
 *  Copyright 2011 Clean Micro, LLC. All rights reserved.
 *
 */
//#define DEBUG

#include "MomentumStrat3.h"
#include "tools.h"

const double ENTRY_THRESH = 0.0005;
const double STOP_LOSS = 0.0015;
const double PROFIT_OBJ = 2*STOP_LOSS;

MomentumStrat3::MomentumStrat3(double entryThreshIn, double stopLossIn, double ratioIn){
	currPos = 0;
	barsSeen = 0;
	entryThresh = entryThreshIn;
	stopLoss = stopLossIn;
	profitObj = ratioIn * stopLossIn;
}

int MomentumStrat3::execute(timeSlice currentBar)
{
	if (barsSeen > 0){
		if (currPos == 0){  //see if we should enter a trade
			double deltaClose = currentBar.close - prevClose;
			if (deltaClose > entryThresh){					//go long
				currPos = 1;  
				entryPrice = currentBar.close;
				PRINTD("found a trade to enter long\n");
			}
			else if (deltaClose < -1*entryThresh){			//short
				currPos = -1; 
				entryPrice = currentBar.close;
				PRINTD("found a trade to enter short\n");
			}
		}
		else {  //see if we should exit the trade
			double priceMovement = currentBar.close - entryPrice;
			if ((currPos * priceMovement) > profitObj){
				currPos = 0; //profit obj hit
				PRINTD("hit the profit obj\n");
			}
			if ((currPos * priceMovement) < -1*stopLoss){
				currPos = 0;	//hit the stop loss
				PRINTD("hit the stop loss\n");
			}
		}
	}
	prevClose = currentBar.close;
	barsSeen++;
		
	return currPos;
}