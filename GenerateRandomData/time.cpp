// time.cpp

#include <iostream>
#include <iomanip>
#include <fstream>
#include "time.h"

using namespace std;

const int SECONDS_IN_HOUR = 3600;
const int SECONDS_IN_MINUTE = 60;

/* ************************************************** */

TimeStamp convertSecondsToTS(int seconds)
{
  TimeStamp aTimeStamp = {0, 0, 0};

  if ( seconds > 0 )
    {
      aTimeStamp.hours=seconds/SECONDS_IN_HOUR;
      seconds%=SECONDS_IN_HOUR;
      aTimeStamp.minutes=seconds/SECONDS_IN_MINUTE;
      seconds%=SECONDS_IN_MINUTE;
      aTimeStamp.seconds=seconds;
    }

  return aTimeStamp;
}

/* ************************************************** */

int convertTSToSeconds(TimeStamp aTimeStamp)
{
  return aTimeStamp.hours * SECONDS_IN_HOUR + aTimeStamp.minutes * SECONDS_IN_MINUTE + aTimeStamp.seconds;
}

/* ************************************************** */

void printATS(TimeStamp aTimeStamp, ofstream &outFile)
{
  outFile.unsetf(ios::left);
  outFile.setf(ios::right);
  outFile.fill('0');
  outFile << 
    setw(2) << 
    aTimeStamp.hours << 
    ":" << 
    setw(2) << 
    aTimeStamp.minutes << 
    ":" << 
    setw(2) << 
    aTimeStamp.seconds;
}

/* ************************************************** */

TimeStamp addSecondsToTS(TimeStamp aTimeStamp, int seconds)
{
  return convertSecondsToTS(convertTSToSeconds(aTimeStamp) + seconds);
}

/* ************************************************** */

TimeStamp addTwoTS(TimeStamp timeStampOne, TimeStamp timeStampTwo)
{
  return convertSecondsToTS(convertTSToSeconds(timeStampOne) + convertTSToSeconds(timeStampTwo));
}

/* ************************************************** */

TimeStamp readATSFromFile(ifstream &inFile)
{
  TimeStamp aTimeStamp;
  char delimeter;

  inFile >> aTimeStamp.hours;
  inFile >> delimeter;
  inFile >> aTimeStamp.minutes;
  inFile >> delimeter;
  inFile >> aTimeStamp.seconds;

  return aTimeStamp;
}

/* ************************************************** */

void initATS(TimeStamp &aTimeStamp)
{
 
  aTimeStamp.hours = 0;
  aTimeStamp.minutes = 0;
  aTimeStamp.seconds = 0;
}

/* ************************************************** */

int compareTwoTS(TimeStamp timeStampOne, TimeStamp timeStampTwo)
{
 // return 0 if timeStampOne == timeStampTwo
 // return a -ve if timeStampOne < timeStampTwo
 // return a +ve if timeStampOne > timeStampTwo
	return convertTSToSeconds(timeStampOne) - convertTSToSeconds(timeStampTwo);
}
    
/* ************************************************** */

