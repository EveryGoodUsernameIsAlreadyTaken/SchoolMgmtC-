// SchoolMgmtDB.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "mssqldb.h"
#include "MainMgmt.h"

using namespace std;

int main()
{
    mssqldb::ConnectDB();

    //mssqldb::TT();

    MainMgmt mgmt = MainMgmt();
    int choice = 0, logged = 0;

    do
    {
        if (logged == 0) 
        {
            switch (choice) {
            case 1:
                logged = mgmt._Login();
                break;
            case 2:
                logged = mgmt._Register();
                break;
            default:
                break;
            }
        }
        else if (logged == 1)
        {
            switch (choice) {
            case 0:
                mgmt._StudentLogOut();
                logged = 0;
                break;
            case 1:
                mgmt._ViewStudentInfo();
                break;
            case 2:
                mgmt._ViewStudentsList();
                break;
            case 3:
                mgmt._JoinCourse();
                break;
            case 4:
                mgmt._LeaveCourse();
                break;
            case 5:
                mgmt._IntroduceCourse();
                break;
            case 6:
                mgmt._ViewStudentCourses();
                break;
            case 7:
                mgmt._DoReport();
                break;
            case 8:
                mgmt._DoTest();
                break;
            case 9:
                mgmt._ViewGrades();
                break;
            default:
                break;
            }
        }
        else if (logged == 2)
        {
            switch (choice) {
            case 0:
                mgmt._TeacherLogOut();
                logged = 0;
                break;
            case 1:
                mgmt._ViewTeacherInfo();
                break;
            case 2:
                mgmt._ViewTeachersList();
                break;
            case 3:
                mgmt._MakeCourse();
                break;
            case 4:
                mgmt._DeleteCourse();
                break;
            case 5:
                mgmt._ViewTeacherCourses();
                break;
            case 6:
                mgmt._MakeReport();
                break;
            case 7:
                mgmt._MakeTest();
                break;
            case 8:
                mgmt._JudgeReport();
                break;
            case 9:
                mgmt._JudgeTest();
                break;
            default:
                break;
            }
        }
        choice = mgmt.MainMenu();
    } while (!(choice == 0 && logged == 0));

    mssqldb::DisconnectDB();
}