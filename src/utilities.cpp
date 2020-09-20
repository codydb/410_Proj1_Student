/***
 * Author: Cody Baker
 * Date: 9/19/2020
 *
 *
 * KP Utils
 */
#include <numeric>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "utilities.h"
#include "stdlib.h"

using namespace std;

//********************** private to this compilation unit **********************

std::vector<process_stats> pStats;

//if myString does not contain a string rep of number returns o
//if int not large enough has undefined behaviour, very fragile
int stringToInt(const char *myString) {
	return atoi(myString);
}

int loadData(const char *filename, bool ignoreFirstRow) {

	pStats.clear();

	ifstream myfstream;
	myfstream.open(filename, ios::in);

	if (!myfstream.is_open()) {
		return COULD_NOT_OPEN_FILE;
	}

	std::string fileLine;

	if (ignoreFirstRow) {
		getline(myfstream, fileLine);
	}

	while (getline(myfstream, fileLine)) {
		process_stats tempStats;
		bool err = false;

		std::stringstream ss(fileLine);
		std::string tempToken;

		//cout << "\n";

		getline(ss, tempToken, CHAR_TO_SEARCH_FOR);
		if (tempToken.size() == 0) {
			err = true;
		}

		tempStats.process_number = stringToInt(tempToken.c_str());
//		cout << tempStats.process_number;
//		cout << " 1 ";

		if (!err) {
			getline(ss, tempToken, CHAR_TO_SEARCH_FOR);
			if (tempToken.size() == 0) {
				err = true;
			} else {
				tempStats.start_time = stringToInt(tempToken.c_str());
//			cout << tempStats.start_time;
//			cout << " 2 ";
			}
		}

		if (!err) {
			getline(ss, tempToken, CHAR_TO_SEARCH_FOR);
			if (tempToken.size() == 0) {
				err = true;
			} else {
				tempStats.cpu_time = stringToInt(tempToken.c_str());
//			cout << tempStats.cpu_time;
//			cout << " 3 ";
			}
		}

		if (!err) {
			if (getline(ss, tempToken, CHAR_TO_SEARCH_FOR)) {
				if (tempToken.size() == 0) {
					err = true;
				} else {
					tempStats.io_time = stringToInt(tempToken.c_str());
//			cout << tempStats.io_time;
//			cout << " 4 ";
				}
			} else {
				err = true;
			}
		}

		if (!err) {
			pStats.push_back(tempStats);
		}

	}

	return SUCCESS;
}

bool compCPUTime(process_stats a1, process_stats b2) {
	return a1.cpu_time < b2.cpu_time;
}

bool compPNum(process_stats a1, process_stats b2) {
	return a1.process_number < b2.process_number;
}

bool compSTime(process_stats a1, process_stats b2) {
	return a1.start_time < b2.start_time;
}

bool compIOTime(process_stats a1, process_stats b2) {
	return a1.io_time < b2.io_time;
}

//will sort according to user preference
void sortData(SORT_ORDER mySortOrder) {

	switch (mySortOrder) {
	case CPU_TIME:
		std::sort(pStats.begin(), pStats.end(), compCPUTime);
		break;
	case PROCESS_NUMBER:
		std::sort(pStats.begin(), pStats.end(), compPNum);
		break;
	case START_TIME:
		std::sort(pStats.begin(), pStats.end(), compSTime);
		break;
	case IO_TIME:
		std::sort(pStats.begin(), pStats.end(), compIOTime);
		break;
	default:
		break;
	}

}

process_stats getNext() {
	process_stats myFirst;

	myFirst = pStats.front();
	pStats.erase(pStats.begin());

	return myFirst;
}

//returns number of process_stats structs in the vector holding them
int getNumbRows() {
	return pStats.size();
}

