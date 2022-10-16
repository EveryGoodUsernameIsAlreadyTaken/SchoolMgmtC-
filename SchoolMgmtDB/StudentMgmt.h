#pragma once
#include <fstream>
#include <iostream>
#include <exception>
#include <string>
#include "MyUtils.h"
#include "mssqldb.h"
#include "Student.h"
#include "Course.h"
#include "CourseHist.h"

class StudentMgmt
{
private:
	Student curStudent = Student();

public:
	StudentMgmt();

	~StudentMgmt();

	int Login();
	
	bool LogOut();

	int AppendStudentInfo();

	void ViewInfo();

	void ViewStudentsList();

	void JoinCourse();

	void LeaveCourse();

	void IntroduceCourse();

	void ViewCourses();

	void DoReport();

	void DoTest();
	
	void ViewGrades();
};

