#include "MainMgmt.h"

MainMgmt::MainMgmt()
{
}


MainMgmt::~MainMgmt()
{
}

int MainMgmt::MainMenu() 
{
	Message::Out("Main Menu page", 1, false);
	int MenuStatus;

	switch (curUser) {
	case 0:
		Message::Out("press (1)Login\npress (2)Register\npress (0)Exit", 2, false);
		break;
	case 1:
		Message::Out("press (1)View Info\npress (2)View Students List\npress (3)Join Course\npress (4)Leave Course\npress (5)Introduce Course\npress (6)View Course\npress (7)Do Report\npress (8)Do Test\npress (9)View Grades\npress (0)Exit", 2, false);
		break;
	case 2:
		Message::Out("press (1)View Info\npress (2)View Teachers List\npress (3)Make Course\npress (4)Delete a Course\npress (5)View Your Courses\npress (6)Assign a report\npress (7)Assign a test\npress (8)Judge a Report\npress (9)Judge a Test\npress (0)Exit", 2, false);
		break;
	}

	Message::Out("Choice: ", 0, false);

	cin >> MenuStatus;

	return MenuStatus;
}

int MainMgmt::_Login()
{
	Message::Out("Login Page", 2, true);

	string choice;

	while (1) {
		Message::Out("Are you logging in as a student(1) or teacher(2)?", 2, false);
		Message::Out("Choice: ", 0, false);

		getchar();
		getline(cin, choice);

		switch(stoi(choice)){
		case 1:
			studMgmt = StudentMgmt();
			curUser = studMgmt.Login();
			return curUser;
		case 2:
			teachMgmt = TeacherMgmt();
			curUser = teachMgmt.Login();
			return curUser;
		default:
			Message::Out("That is not a choice. Please try again.", 1, true);
			break;
		}
	}

	return 0;
}

int MainMgmt::_Register()
{
	Message::Out("Registration Page", 2, true);

	int choice;

	while (1) {
		Message::Out("Are you registering as a student(1) or teacher(2): ", 0, false);
		cin >> choice;

		switch (choice) {
		case 1:
			studMgmt = StudentMgmt();
			return curUser = studMgmt.AppendStudentInfo();
			break;
		case 2:
			teachMgmt = TeacherMgmt();
			return curUser = teachMgmt.AppendTeacherInfo();
			break;
		default:
			Message::Out("That is not a choice. Please try again.", 1, true);
			break;
		}
	}
	
	return false;
}

void MainMgmt::_StudentLogOut() {
	if (studMgmt.LogOut()) {
		curUser = 0;
		Message::Out("Logged off successfully", 2, true);
	}
}

void MainMgmt::_ViewStudentInfo() {
	Message::Out("View Info Page", 2, true);
	studMgmt.ViewInfo();
	cout << endl;
}

void MainMgmt::_ViewStudentsList() {
	Message::Out("View Students List Page", 2, true);
	studMgmt.ViewStudentsList();
	cout << endl;
}

void MainMgmt::_JoinCourse() {
	Message::Out("Join Course Page", 2, true);
	studMgmt.JoinCourse();
	cout << endl;
}

void MainMgmt::_LeaveCourse() {
	Message::Out("Leave Course Page", 2, true);
	studMgmt.LeaveCourse();
	cout << endl;
}

void MainMgmt::_IntroduceCourse() {
	Message::Out("Introduce Course Page", 2, true);
	studMgmt.IntroduceCourse();
	cout << endl;
}

void MainMgmt::_ViewStudentCourses() {
	Message::Out("View Courses Page", 2, true);
	studMgmt.ViewCourses();
	cout << endl;
}

void MainMgmt::_DoReport() {
	Message::Out("Do Report Page", 2, true);
	studMgmt.DoReport();
	cout << endl;
}

void MainMgmt::_DoTest() {
	Message::Out("Do Test Page", 2, true);
	studMgmt.DoTest();
	cout << endl;
}

void MainMgmt::_ViewGrades() {
	Message::Out("View Grades Page", 2, true);
	studMgmt.ViewGrades();
	cout << endl;
}

//
//------------------------------ TEACHERS
//

void MainMgmt::_TeacherLogOut() {
	if (teachMgmt.LogOut()) {
		curUser = 0;
		Message::Out("Logged off successfully", 2, true);
	}
}

void MainMgmt::_ViewTeacherInfo() {
	Message::Out("View Info Page", 2, true);
	teachMgmt.ViewInfo();
	cout << endl;
}

void MainMgmt::_ViewTeachersList() {
	Message::Out("View Teachers List Page", 2, true);
	teachMgmt.ViewTeachersList();
	cout << endl;
}

void MainMgmt::_MakeCourse() {
	Message::Out("Make a course page", 2, true);
	teachMgmt.AppendCourse();
	cout << endl;
}

void MainMgmt::_DeleteCourse() {
	Message::Out("Delete Course page", 2, true);
	teachMgmt.DeleteCourse();
	cout << endl;
}

void MainMgmt::_ViewTeacherCourses() {
	Message::Out("Courses Taught page", 2, true);
	teachMgmt.ViewCourses();
	cout << endl;
}

void MainMgmt::_MakeReport() {
	Message::Out("Make a report page", 2, true);
	teachMgmt.MakeReport();
	cout << endl;
}

void MainMgmt::_MakeTest() {
	Message::Out("Make a test page", 2, true);
	teachMgmt.MakeTest();
	cout << endl;
}

void MainMgmt::_JudgeReport() {
	Message::Out("Judge a report page", 2, true);
	teachMgmt.JudgeReport();
	cout << endl;
}

void MainMgmt::_JudgeTest() {
	Message::Out("Judge a test page", 2, true);
	teachMgmt.JudgeTest();
	cout << endl;
}