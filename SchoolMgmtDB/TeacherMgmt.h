#pragma once
#include <fstream>
#include <iostream>
#include <exception>
#include <string>
#include "MyUtils.h"
#include "mssqldb.h"
#include "Teacher.h"
#include "Test.h"
#include "Report.h"

class TeacherMgmt
{
private:
	Teacher curTeacher = Teacher();

public:
	TeacherMgmt();

	~TeacherMgmt();

	int Login();

	bool LogOut();

	int AppendTeacherInfo();

	void ViewInfo();

	void ViewTeachersList();

	void AppendCourse();

	void DeleteCourse();

	void ViewCourses();

	void MakeReport();

	void JudgeReport();

	void MakeTest();

	void JudgeTest();
};

