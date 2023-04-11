#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#endif

// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;

/*
Helper function that helps find the amount of days worked by
any single worker within one solution 
*/
void workingDays (DailySchedule& sched, int counter, 
int& nichi,int &row,int &col) { 
  if(counter == static_cast<int>(sched.size())) {
    return;
  }
  else {
    nichi = nichi + count(sched[counter].begin(), sched[counter].end(), sched[row][col]);
    workingDays(sched, counter + 1, nichi, row, col);
  }
}

/*
Helper function that helps find the amount of workers
are working on this day and if we get to the end of the list 
of workers and we have not already reached the required amount
of workers we simply return false since a schedule that isn't
full is not valid
*/
bool workersToday(DailySchedule& sched, int counter, int& shigotoWoShimasu,int &row,int &col) {
	if(counter==col+1) {
		return true;
	}
	else {
		if(sched[row][counter] != INVALID_ID) {
      if(sched[row][counter]==sched[row][col] && counter!=col) {
        return false;
      }
      shigotoWoShimasu++;
    }
	}
	return(workersToday(sched,counter+1,shigotoWoShimasu,row,col));
}

/*
Helper function that checks whether or not the combination of numWorkers
is a valid work schedule
*/
bool isValid (const AvailabilityMatrix& avail, const size_t dailyNeed,
const size_t maxShifts, DailySchedule& sched, int &row,int &col) {
  int nichi = 0;

	// call helper function to find out how many days are worked for 
	// the worker in the current position
  workingDays(sched, 0, nichi, row, col);

	// if the worker has worked more days than max shifts in this current
	// iteration of schedule then return false so that it does not fill the
	// rest of the schedule since we already know with how many we filled in that
	// the worker is working too many days
  if(nichi>static_cast<int>(maxShifts)) {
          return false;
  }


	// here we check whether or not the day we are on has enough
	// workers
  int shigotoWoShimasu =0;

	if(workersToday(sched, 0, shigotoWoShimasu, row, col)==false) {
		return false;
	}

	// checks whether or not the day has enough workers 
  if(col == static_cast<int>(sched[0].size()-1) && shigotoWoShimasu < static_cast<int>(dailyNeed)) {
      return false;
  }

	//if the current worker we are on has already been set
	// we will check whether or not this worker is worker is
	// available to actually work that day
  if(sched[row][col]!=INVALID_ID) {
    if(!avail[row][sched[row][col]]) {
      return false;
    }
  }
  return true;
}

// Add prototypes for any helper functions here
bool helpMeMakeSchedule( const AvailabilityMatrix& avail, const size_t dailyNeed,
const size_t maxShifts, DailySchedule& sched, int row, int col) {
	// this if statetment handles the case where we are at the very
	// last position in the schedule matrix and will return true since
	// we were able to reach the end of the matrix without returning false
	// in an earlier recursive call
  if(row == static_cast<int>((sched.size()-1)) && col>static_cast<int>((sched[0].size()-1))) {
    return true;
  }
	// this else if handles the case where we reach the end of one
	// of the days amount of workers so it moves to the next day
	// by increasing row and reseting col before calling the function
  else if (row < static_cast<int>(sched.size()-1) && col>(static_cast<int>(sched[0].size()-1))) {
    row++;
    col=0;
    return helpMeMakeSchedule(avail, dailyNeed, maxShifts, sched, row, col);
  }
	
	// iterate through the set of workers
  for(Worker_T i = 0; i < avail[0].size(); i++) {
		//set a worker in a shift in the schedule
    sched[row][col]=i;
		// check if it is valid for the worker to be in that position
    if(isValid(avail, dailyNeed, maxShifts, sched, row, col)) {
			// move onto the next position in the schedule matrix
      if(helpMeMakeSchedule(avail, dailyNeed, maxShifts, sched, row, col+1)) {
        return true;
      }

    }
  }
  sched[row][col]=INVALID_ID;
  return false;
}

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();

		//size schedule to fit the dimensions of n * d
    sched.resize(avail.size());
    for(int i = 0; i < static_cast<int>(avail.size()); i++) {
      sched[i].resize(dailyNeed,INVALID_ID);
    }
		// make call to recursive helper function
    return helpMeMakeSchedule(avail, dailyNeed,maxShifts,sched,0,0);
}

