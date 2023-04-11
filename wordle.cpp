#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
//The reason the runtime is bad is def because not considering cases where blanks are before the end
// WORK MAINLY ON THIS ONE
void helper(const std::string& in, const std::string& floating, const std::set<std::string>& dict, 
std::string& temp, std::set<std::string>& combinations, int& usedFloats, std::string& angostura, int& KESHI) {
	//start the function by finding the amount of blanks that have already been used
	int usedBlanks = 0;
	for(size_t i=0; i < temp.length(); i++) {
		if(in[i] == '-') {
			if(angostura.find(temp[i]) != std::string::npos) {
				angostura.erase(angostura.find(temp[i]),1);
				usedFloats++;
			}
			usedBlanks++;
		}
	}

	// if there are more floats than there are spaces to fill then return
	// because this should not be possible
	if((floating.length() - usedFloats) > (in.length() - temp.length())) {
		return;
	}

	// if the temp string is already the length of the word we are
	// looking for then we can check if it is in the dictionary
	// the other parts of the function already check if the
	// floating letters are included
	if(temp.length() == in.length()) { 
			if(dict.find(temp)!=dict.end()) {
				combinations.insert(temp);
		}
	}
	// this handles the "green" letters from the wordle and 
	// makes sure that we place the green letter into temp
	// and later do not change it when we are trying to find 
	// the other letters
	else if(in[temp.length()]!='-' && !(temp.length()>=in.length())) {
		// add the green letter to temp
		temp = temp + in[temp.length()];
		//reset counters
		usedFloats=0;
		angostura=floating;
		// call helper function again
		helper(in, floating, dict, temp, combinations, usedFloats, angostura, KESHI);
		// remove whatever letter was added by the helper call to move
		// onto the next letter in this letters place
		if(temp.length()==1) {
				temp = "";
			}
			else {
				temp.pop_back();
			}
	}
	// this handles the case where the amount of blanks left are
	// equal to the amount of floating characters we have left which
	// means that the remaining letters have to be the floating letters
	else if(((floating.length() - usedFloats) == (in.length() - temp.length())) || (KESHI - usedBlanks <= static_cast<int>(angostura.length()))) {
		for(int i =0; i < static_cast<int>(angostura.length()); i++ ) {
			// angostura is holding the unused floating characters so
			// we loop through the missing floating characters in the 
			// remaining places
			temp = temp + angostura[i];
			// reset counters
			usedFloats=0;
			angostura=floating;
			helper(in, floating, dict, temp, combinations, usedFloats, angostura, KESHI);
			if(temp.length()==1) {
				temp = "";
			}
			else {
				temp.pop_back();
			}
		}
	}
	// this handles all other cases where the remaining letters do not
	// equal floaters because the possible letter could be anything in the alphabet
	else {
		for(char i = 'a'; i <= 'z'; i++) { // for loop that iterates through the alphabet
			temp = temp + i;
			usedFloats=0;
			angostura=floating;
			helper(in, floating, dict, temp, combinations, usedFloats, angostura, KESHI);
			if(temp.length()==1) {
				temp = "";
			}
			else {
				temp.pop_back();
			}
		}
	}
	//at the end of each helper call we have to set the amount of used floats to zero
	//because when we backtrack to the letter before the letter we are currently on
	//we may be removing letters that are floats so we have to count
	//floats again for the next call to the function
	usedFloats=0;
}


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here

		//create empty string to test words without affecting in or floating 
		std::string temp = "";

		//create a set of combinations to hold all words found in dictionary
		std::set<std::string> combinations;

		//create a counter to count the amount of floats used
		int usedFloats = 0;

		//create a string to be able to make changes to floating each
		//iteration without altering floating
		std::string floting = floating;

		//get the number of missing letters that are unknown
		int KESHI = 0;
		for(size_t i =0; i < in.length(); i++) {
			if(in[i]=='-') {
				KESHI++;
			}
		}

		//call the helper function that will find the dictionary words
		helper(in, floating, dict, temp, combinations, usedFloats, floting, KESHI);
		return combinations;
}

// Define any helper functions here






























