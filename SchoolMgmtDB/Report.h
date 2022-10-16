#pragma once
#include <iostream>
#include <string>

using namespace std;

class Report {


public:
	int SID, TID, Year, CID, ReportID;
	string Title, Task, Answers, DueDate;
	float Grade = 0;
	bool Graded;

	Report() {};
};
