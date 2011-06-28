//	This file parses a comma-delimited text file into a vector of time slices
//
//	To do:	1. have the number of fields be variable/user selectable so we do not waste memory

#include <iostream>
#include <fstream>
#include "histTextLoad.h"

#define DEBUG false
#define DELIMITER '\t'

vector<timeSlice> histTextLoad (string fileName) 
{
	timeSlice currentBar;
	vector<timeSlice> returnVector;
	string line;
	ifstream myfile;
	myfile.open(fileName.c_str());
	if (myfile.is_open())
	{
		while (myfile.good())//good is true until EOF hit
		{
			getline(myfile,line);
			currentBar = parseLine(line);
			if (currentBar.validBar == false)
			{
				#if DEBUG
				cout<<"bad data line: "<<line<<endl; 
				#endif
			}
			else
			{
				//append this mktBar to the vector
				returnVector.push_back(currentBar);
			}
		}
		myfile.close();
		#if DEBUG
		cerr << "Finished parsing, sucessfullly parsed "<<returnVector.size()<<" time slices.\n";
		#endif
		return returnVector;
	}
	else cerr<<"Unable to open the file, man!";
	return returnVector;
}
timeSlice parseLine(string inputLine)//should return something 
{
	timeSlice retBar;
	size_t commaLoc = -1;
	size_t prevCommaLoc = -1;
	string parsedString;
	
	parsedString = splitCSVline(inputLine, &commaLoc, &prevCommaLoc);
	if (parsedString.empty())
	{
		retBar.validBar = false;					//set time
		return retBar;
	}
	else //parse minutes from '20110727  13:58:00' full time obj is overkill
	{
		long int i0, i1, i2, i3;
		sscanf(parsedString.c_str(), "%ld  %ld:%ld:%ld", &i0, &i1, &i2, &i3);
		retBar.date = 10000*i0 + 60*i1 + i2; //date is YYYYMMDD + minutes since midnight ok to sort
	}
	#if DEBUG
	cout << "Date: "<< retBar.date<<"\t";
	#endif
	
	parsedString = splitCSVline(inputLine, &commaLoc, &prevCommaLoc);
	if (parsedString.empty())
	{
		retBar.validBar = false;
		return retBar;
	}
	else retBar.open = atof(parsedString.c_str());		//set open
	#if DEBUG
	cout << "Open: "<<retBar.open <<"\t";
	#endif
	
	parsedString = splitCSVline(inputLine, &commaLoc, &prevCommaLoc);
	if (parsedString.empty())
	{
		retBar.validBar = false;
		return retBar;
	}
	else retBar.high = atof(parsedString.c_str());		//set high
	#if DEBUG
	cout << "High: "<<retBar.high <<"\t";
	#endif
	
	parsedString = splitCSVline(inputLine, &commaLoc, &prevCommaLoc);
	if (parsedString.empty())
	{
		retBar.validBar = false;
		return retBar;
	}
	else retBar.low = atof(parsedString.c_str());		//set low
	#if DEBUG
	cout << "Low: "<<retBar.low <<"\t";
	#endif
	
	parsedString = splitCSVline(inputLine, &commaLoc, &prevCommaLoc);
	if (parsedString.empty())
	{
		retBar.validBar = false;
		return retBar;
	}
	else retBar.close = atof(parsedString.c_str());		//set close
	#if DEBUG
	cout << "Close: "<<retBar.close <<"\t";
	#endif
	
	commaLoc = inputLine.find_first_of(',',commaLoc+1);	//special case for parsing string
	parsedString = inputLine.substr(prevCommaLoc+1);
	if (parsedString.empty())
	{
		retBar.validBar = false;
		return retBar;
	}
	else retBar.volume = atoi(parsedString.c_str());		//set adj close
	#if DEBUG
	cout << "Volume: " << retBar.volume <<endl;
	#endif
	
	retBar.validBar = true;//if we have gotten this far it's a valid bar
	return retBar;
	
}
string splitCSVline(string inputLine, size_t *commaLoc, size_t *prevCommaLoc)
{
	string retString;
	*commaLoc = inputLine.find_first_of(DELIMITER,*commaLoc+1);
	retString = inputLine.substr(*prevCommaLoc+1, *commaLoc-*prevCommaLoc-1);
	*prevCommaLoc = *commaLoc;
	return retString;
}

bool timeSliceSortPred(timeSlice t1, timeSlice t2)
{
	return t1.date < t2.date;
}


