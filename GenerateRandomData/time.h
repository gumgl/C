// time.h

#ifndef TIME
#define TIME

#include <fstream>

using namespace std;

struct TimeStamp
{
  int seconds;
  int minutes;
  int hours;
};

/* ************************************************** */

void initATS(TimeStamp &);

void printATS(TimeStamp aTimeStamp, ofstream &);
TimeStamp readATSFromFile(ifstream&);

TimeStamp convertSecondsToTS(int seconds);
int convertTSToSeconds(TimeStamp aTimeStamp);

TimeStamp addSecondsToTS(TimeStamp aTimeStamp, int seconds);
TimeStamp addTwoTS(TimeStamp timeStampOne, TimeStamp timeStampTwo);

int compareTwoTS(TimeStamp timeStampOne, TimeStamp timeStampTwo);

/* ************************************************** */

#endif
