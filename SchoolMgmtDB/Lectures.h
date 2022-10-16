#pragma once
#include <iostream>
#include <string>

using namespace std;

class Lectures {


public:
	int CID, TID;
	string Time;

	Lectures() {};

	Lectures(int cid, int tid, string time) {
		CID = cid;
		TID = tid;
		Time = time;
	}
};
