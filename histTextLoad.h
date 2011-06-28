/*
 *  histTextLoad.h
 *  AutoTrade
 *
 *  Created by Peter Harrington on 1/15/11.
 *  Copyright 2011 Clean Micro, LLC. All rights reserved.
 *
 */

#ifndef HIST_TEXT_LOAD_H
#define HIST_TEXT_LOAD_H 1

#include <string>
#include <vector>

using namespace std;

struct timeSlice 
{
	long int date;
	double close,high,low,open;
	int volume;
	bool validBar;
};

vector<timeSlice> histTextLoad (string fileName);
timeSlice parseLine(string inputLine);
string splitCSVline(string inputLine, size_t *commLoc, size_t *prevCommaLoc);
bool timeSliceSortPred(timeSlice t1, timeSlice t2);

#endif
