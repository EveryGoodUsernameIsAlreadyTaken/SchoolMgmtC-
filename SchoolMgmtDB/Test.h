#pragma once
#include <iostream>
#include <string>

using namespace std;

class Test {


public:
	int SID, TID, Year, CID, TestID;
	string Subject, Task, Answers, TakeDate;
	float Grade = 0;
	bool Graded;

	Test() {};
};
