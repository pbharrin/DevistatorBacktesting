/*
 *  MomentumStrat2.cpp
 *  Momentum
 *
 *  Created by Peter Harrington on 5/24/11.
 *  Copyright 2011 Clean Micro, LLC. All rights reserved.
 *
 *	This uses average true range as the breakout heuristic.
 *  This acheived retruns of 33% over 9/10 of a year. 
 *  The underlying gained 16% during that time.
 *  This strategy did bad during down times.  It has a huge 
 *  drawdown and is in the market most of the time. 
 *  
 *	Additionally this stratgey is wildly unprofitable when
 *  transaction costs are considered.  The Anualized Sharpe's 
 *  ratio w/o transacion costs is ~2 with tranaction costs it's -10.
 */

#include "MomentumStrat2.h"
#include <iostream>

using namespace std;

#define DEBUG false
const float STOP_LOSS = 0.0015; //0.0015 works best
const int NMINS = 1;		//number of days for range calculation			

MomentumStrat2::MomentumStrat2(int lowerBound)
{
	barsSeen = 0;
	currPos = 0;
	lastXbars = deque<timeSlice>();
	stopLoss = STOP_LOSS;
	profitOb = 0.53;
	rangeMult = 0.90;
	slReduceThresh = 180;
	stopLossHits = 0;
	profObHits = 0;
}

int MomentumStrat2::execute(timeSlice currentBar)
{
	//if ((barsSeen > NMINS) && ((barsSeen > 100) && (barsSeen < 500) || (barsSeen > 900) && (barsSeen < 2000)) )
	if (barsSeen > NMINS)
	{	
		//calculate Average True Range
		timeSlice barToEval;
		double aveTrueRange, trueRange;
		double rangeSum = 0.0;
		for (int i=1; i < NMINS+1; i++)  //could rewrite barToEval as lastXbars[1]
		{								 //and lastXbars[i-1] as lastXbars[0]
			barToEval = lastXbars[i];
			if ((barToEval.high - barToEval.low) > (barToEval.high - lastXbars[i-1].close))
			{
				trueRange = (barToEval.high - barToEval.low);
			}
			else trueRange = (barToEval.high - lastXbars[i-1].close);
			if (trueRange < (lastXbars[i-1].close - barToEval.close))
			{
				trueRange = (lastXbars[i-1].close - barToEval.close);
			}
			rangeSum += trueRange;
		}
		aveTrueRange = rangeSum/NMINS;
		if (currPos == 0)		//see if we should enter a trade
		{
			if (currentBar.open > (lastXbars[NMINS-1].open + rangeMult*aveTrueRange))
			{
				currPos = 1;  //go long
				entryPrice = currentBar.open;  //the true entry price would be the next bar open
				entryTime = barsSeen;
			}
			else if (currentBar.open < (lastXbars[NMINS-1].open - rangeMult*aveTrueRange))
			{
				currPos = -1; //go short
				entryPrice = currentBar.open;
				entryTime = barsSeen;
			}
			if (currPos != 0)//see if one of the two conditions has been met
			{
				#if DEBUG 
				cout << "found a trade to enter "<< barsSeen <<" "<< currPos << endl; 
				#endif
				stopLoss = STOP_LOSS;  //reset stopLoss in case it was reduced earlier
				//currPos = 0; //reset for testing entry points
			}
		}
		else if (currPos == 1)//check to see if we should exit a trade
		{
			//absolute stop loss 
			if (currentBar.open < (entryPrice - stopLoss))
			{
				currPos = 0;
				#if DEBUG
				stopLossHits++;
				cout << "hit the abs stop loss exit "<< barsSeen <<" stopLossHits "<< stopLossHits<<endl;
				#endif
			}
			//profit objective
			if (currentBar.open > (entryPrice + 0.5*STOP_LOSS)) 
			{
				currPos = 0;
				#if DEBUG
				profObHits++;
				cout << "hit the profit objective while long "<< barsSeen <<" profObHits "<< profObHits <<endl;
				#endif
			}
			/*
			//stop loss reduction exp.
			if (slReduceThresh < (barsSeen - entryTime)) 
			{
				stopLoss = 0.75 * STOP_LOSS;  //best performing
				#if DEBUG
				cout << "stop loss reduced"<< endl;
				#endif
			}
			 */
		}
		else if (currPos == -1)//check to see if we should exit a trade
		{
			//absolute stop loss 
			if (currentBar.open > (entryPrice + stopLoss))
			{
				currPos = 0;
				#if DEBUG
				stopLossHits++;
				cout << "hit the abs stop loss exit "<< barsSeen <<" stopLossHits "<< stopLossHits <<endl;
				#endif
			}
			//profit objective 
			if (currentBar.open < (entryPrice - 0.5*STOP_LOSS)) 
			{
				currPos = 0;
				#if DEBUG
				profObHits++;
				cout << "hit the profit objective while short "<< barsSeen <<" profObHits "<< profObHits <<endl;
				#endif
			}
			/*
			//stop loss reduction exp.
			if (slReduceThresh < (barsSeen - entryTime)) 
			{
				stopLoss = 0.75 * STOP_LOSS;
				#if DEBUG
				cout << "stop loss reduced"<< endl;
				#endif
			}
			*/
		}
		lastXbars.pop_front();
	}
	//else if (barsSeen > NMINS) lastXbars.pop_front();
	
	lastXbars.push_back(currentBar);
	barsSeen++;
	
	return currPos;
}
