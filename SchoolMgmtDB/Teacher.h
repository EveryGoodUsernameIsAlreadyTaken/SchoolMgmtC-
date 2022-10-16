#pragma once
#include <iostream>
#include <string>

using namespace std;

class Teacher {
private:

public:
	int TID;
	string User, Pass, FName, LName, Email, Dptmt, College, Subj, PhoneNo, Website;

	Teacher() {};

	Teacher(int tid, string user, string pass, string fname, string lname, string email, string dptmt, string college, string subj, string phoneno, string website) {
		TID = tid;
		User = user;
		Pass = pass;
		FName = fname;
		LName = lname;
		Email = email;
		Dptmt = dptmt;
		College = college;
		Subj = subj;
		PhoneNo = phoneno;
		Website = website;
	}
};
