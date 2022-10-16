#pragma once
#include <iostream>
#include <string>

using namespace std;

class CourseHist {


public:
	int CID, SID, Year;
	string Time, W_Grade;
	float Points = 0;

	CourseHist() {};

	CourseHist(int cid, int sid, string time, string w_grade, int year, float points = 0.0) {
		CID = cid;
		SID = sid;
		Time = time;
		W_Grade = w_grade;
		Year = year;
		Points = points;
	}
};
