#pragma once
#include <iostream>
#include <string>

using namespace std;

class Course {
public:
	int CID, MaxSize, RoomNo, CollegeID;
	string ClassName, TextBook,  Category;

	Course() {};

	Course(int cid, int clgid, string classname, string textbook, int maxsize, int roomNo, string category) {
		CID = cid;
		CollegeID = clgid;
		ClassName = classname;
		TextBook = textbook;
		MaxSize = maxsize;
		RoomNo = roomNo;
		Category = category;
	}
};
