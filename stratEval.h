/*
 *  stratEval.cpp
 *  Momentum
 *
 *  Created by Peter Harrington on 6/2/11.
 *  Copyright 2011 Clean Micro, LLC. All rights reserved.
 *
 *  This file contains the strategy evaluation code.  
 *
 */
#include "Strategy.h"

struct stratResu {
	double totalRet;		//total returns
	double percInMkt;		//percentage of time in market
	int partStatN;			//number of returns
	double partStatSumSq;	//sum of the squares of returns (used for global std calc)
	int numberTrades;		//total number of times it trades
};
struct maxDDStore {			//data struct to hold Max Draw Down info
	int ddTime;				//this is used to pass maxDD info from one day
	int maxDDtime;			//to the next.
	double drawDown;
	double maxDrawDown;
};

void calcPNL(double pnlArr[], double returnsArr[], vector<int> positionVec);
void calcReturns(double returnsArr[], vector<timeSlice> histData);
void sumCumReturns(stratResu* retStruct, double pnlArr[], int arrSize, maxDDStore* maxDDPass);
double calcPerentMkt(vector<int> positionVec);
int calcNumTrades(vector<int> positionVec);

stratResu stratEval(Strategy * stratUnderTest, vector<timeSlice> testData, maxDDStore* maxDDPass)
{
	vector<int> positionVec;		//position vector to be used in step 4
	positionVec.push_back(0);		//initial position is 0
	int tradingSignal = 0;
	for (vector<timeSlice>::const_iterator citer=testData.begin(); citer!=testData.end()-1; ++citer)
	{
		//get trading signal for each timeSlice
		tradingSignal = stratUnderTest->execute(*citer); 
		positionVec.push_back(tradingSignal);
	}
	
	//calculate the returns: price(t)-price(t-1)
	double returnsArr[testData.size()-1];
	calcReturns(returnsArr, testData);
	
	//calc pnl: position(t)*returns(t)
	double pnlArr[testData.size()-1];
	calcPNL(pnlArr, returnsArr, positionVec);
	
	stratResu retResults;
	sumCumReturns(&retResults, pnlArr, positionVec.size(), maxDDPass);
	retResults.percInMkt = calcPerentMkt(positionVec);
	
	retResults.numberTrades = calcNumTrades(positionVec);

	return retResults;
}
void sumCumReturns(stratResu* retStruct, double pnlArr[], int arrSize, maxDDStore* maxDDPass){
	retStruct->totalRet = 0.0;  //init data structure 
	
	double cumSum = 0.0;
	double cumSumSq = 0.0;
	double peakCumSum = maxDDPass->drawDown;    //value to hold the peak for maxDD calc
	int maxDDtime = maxDDPass->maxDDtime;
	int ddTime = maxDDPass->ddTime;
	double drawDown = 0.0;
	double maxDrawDown = maxDDPass->maxDrawDown;
	
	for (int i=0; i<arrSize; i++){ //sum returns
		cumSum += pnlArr[i];
		cumSumSq += pnlArr[i]*pnlArr[i];
		if (cumSum > peakCumSum){
			peakCumSum = cumSum;
			ddTime = 0;
		}
		else {
			drawDown = peakCumSum - cumSum;
			if (drawDown > maxDrawDown) maxDrawDown = drawDown;
			ddTime++;
			if (ddTime > maxDDtime) maxDDtime = ddTime;
		}
	}
	retStruct->totalRet = cumSum;
	
	retStruct->partStatN = arrSize;
	retStruct->partStatSumSq = cumSumSq;
	
	maxDDPass->drawDown = drawDown;
	maxDDPass->ddTime = ddTime;
	maxDDPass->maxDDtime = maxDDtime;
	maxDDPass->maxDrawDown = maxDrawDown;
}
void initDDstore(maxDDStore* maxDDPass){
	maxDDPass->ddTime = 0;
	maxDDPass->maxDDtime =0;
	maxDDPass->drawDown = 0.0;
	maxDDPass->maxDrawDown = 0.0;
}
double calcPerentMkt(vector<int> positionVec){	//count the % of time in the market
	int minsInPos = 0;
	for (int i=0; i<positionVec.size()-1; i++) {
		if (positionVec[i]!=0) minsInPos++;
	}
	return (double)minsInPos/(double)(positionVec.size()-1);
}
int calcNumTrades(vector<int> positionVec){  //calc the number of times the position changes
	int totalTrades = 0; 
	int lastEl = positionVec.size()-1;
	for (int i=1; i<lastEl; i++) {
		if (positionVec[i] != positionVec[i-1]) totalTrades++;
	}
	if (positionVec[lastEl]!= 0) totalTrades++;
	return totalTrades;
}
void calcPNL(double pnlArr[], double returnsArr[], vector<int> positionVec){
	for (int i=0; i<positionVec.size()-1; i++) {
		pnlArr[i] = returnsArr[i] * (float)positionVec[i];
		//cout << "position returns profit "<<positionVec[i]<<","<<returnsArr[i]<<","<<pnlArr[i]<<endl;
	}
}
void calcReturns(double returnsArr[], vector<timeSlice> histData){
	//float returnsArr[histData.size()-1];
	for (int i=1; i<histData.size(); i++) {
		returnsArr[i-1] = (histData[i].close - histData[i-1].close) / histData[i-1].close;
	}
}