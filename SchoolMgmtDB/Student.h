#pragma once
#include <iostream>
#include <string>

using namespace std;

class Student {


public:
	int SID;
	string User, Pass, FName, LName, Addr1, Addr2, City, State, Zip, Email, PhoneNo, DOB;
	float GPA = 0;
	bool Fixed = false;

	Student() {};

	Student(int sid, string user, string pass, string fname, string lname, string dob, string addr1, string addr2, string city, string state, string zip, string email, string phoneno, float gpa = 0.0, bool fixed = false) {
		SID = sid;
		User = user;
		Pass = pass;
		FName = fname;
		LName = lname;
		DOB = dob;
		Addr1 = addr1;
		Addr2 = addr2;
		City = city;
		State = state;
		Zip = zip;
		Email = email;
		PhoneNo = phoneno;
		GPA = gpa;
		Fixed = fixed;
	}
};
