#pragma once
#include <iostream>
#include <stdlib.h>
#include "MyUtils.h"
#include "StudentMgmt.h"
#include "TeacherMgmt.h"

using namespace std;

class MainMgmt
{
private:
	int curUser = 0;
	StudentMgmt studMgmt;
	TeacherMgmt teachMgmt;
public:
	MainMgmt();
	~MainMgmt();
	
	int MainMenu();

	int _Login();

	int _Register();

	void _StudentLogOut();

	void _ViewStudentInfo();

	void _ViewStudentsList();

	void _JoinCourse();

	void _LeaveCourse();
	
	void _IntroduceCourse();

	void _ViewStudentCourses();

	void _DoReport();

	void _DoTest();

	void _ViewGrades();

	void _TeacherLogOut();

	void _ViewTeacherInfo();

	void _ViewTeachersList();

	void _MakeCourse();

	void _DeleteCourse();

	void _ViewTeacherCourses();

	void _MakeReport();

	void _MakeTest();

	void _JudgeReport();

	void _JudgeTest();
};

