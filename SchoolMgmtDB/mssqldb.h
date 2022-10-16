#pragma once
#include <string>
#include <iostream>
#include <Windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <vector>
#include "Student.h"
#include "Teacher.h"
#include "Course.h"
#include "CourseHist.h"
#include "Report.h"
#include "Test.h"

enum DExecType { TABLE = 0, INS = 1, UPD = 2, DEL = 3 };

#define SQL_RESULT_LEN 50
#define SQL_RETURN_CODE_LEN 1000

using namespace std;

class mssqldb
{
public:
	static void ConnectDB();
	static void DisconnectDB();
	static void ConnectStatement();
	static void DisconnectStatement();

	static SQLWCHAR* GetWString(string szStr);
	static void ShowErrorMsg(char* fn, SQLHANDLE handle, SQLSMALLINT type);

	static bool CheckUser(string user, string table);
	static bool CheckPass(string pass, string table);
	static bool CheckRoom(string room, string table);
	static int GetNewIDFromTable(string idCol, string table);
	static void ExecDBData(string szSQL, DExecType DEType);
	//static Customer QueryCustomerInfo(string szSQL);
	//static vector<Customer> QueryCustomerList(string szSQL);

	// For Orders
	//static bool CheckOrderId(string orderNo, string custNo);
	//static int NextOrderNo();
	//static Order QueryOrderInfo(string szSQL);
	//static vector<Order> QueryOrderList(string szSQL);

	static void SQLInputStr(int ipar, int coldef, string* sInput);
	static void SQLInputInt(int ipar, int* iInput);
	static void SQLInputFloat(int ipar, float* dInput);
	static void SQLInputBit(int ipar, bool* bInput);

	// stored procedure
//------------------------------------------ STUDENTS -----------------------------------------------
//---------------------------------------------------------------------------------------------------
	static vector<string> SPCollegeList(SQLWCHAR* szSPName);
	static int SPInputStudent(SQLWCHAR* szSPName, Student student);
	static Student SPStudentInfo(SQLWCHAR* szSPName, string usern, string passw);
	static vector<Student> SPAllStudentList(SQLWCHAR* szSPName);
	static void SPInsertCourseHist(SQLWCHAR* szSPName, CourseHist courseh);

	
//------------------------------------------ TEACHERS -----------------------------------------------
//---------------------------------------------------------------------------------------------------
	static int SPInputTeacher(SQLWCHAR* szSPName, Teacher teacher);
	static Teacher SPTeacherInfo(SQLWCHAR* szSPName, string usern, string passw);
	static vector<Teacher> SPAllTeacherList(SQLWCHAR* szSPName);
	static vector<string> SPGetCategories(SQLWCHAR* szSPName);
	static void SPInsertCourse(SQLWCHAR* szSPNAME, Course course);
	static void SPInsertLecture(SQLWCHAR* szSPNAME, int cid, int tid, string time);
	static vector<pair<int, string>> SPGetCoursesByTID(SQLWCHAR* szSPName, int tid);
	static vector<pair<int, string>> SPGetCoursesBySID(SQLWCHAR* szSPName, int sid);
	static void SPDeleteCourse(SQLWCHAR* szSPName, int cid);

//------------------------------------------ COURSES ------------------------------------------------
//---------------------------------------------------------------------------------------------------
	static int SPGetCIDFromName(SQLWCHAR* szSPName, string name);
	static string SPGetTime(SQLWCHAR* szSPName, int cid);
	static string SPGetCourseName(SQLWCHAR* szSPName, int id);
	static vector<Course> SPAllCoursesList(SQLWCHAR* szSPName);
	static vector<string> SPGetClassesByCat(SQLWCHAR* szSPName, string category);
	static vector<Course> SPGetCoursesByCat(SQLWCHAR* szSPName, string category);

//--------------------------------------- REPORTS/JUDGES --------------------------------------------
//---------------------------------------------------------------------------------------------------
	static vector<int> SPGetSIDListFromCID(SQLWCHAR* szSPName, int cid);
	static void SPMakeReport(SQLWCHAR* szSPName, Report report, int sid);
	static void SPMakeTest(SQLWCHAR* szSPName, Test test, int sid);
	static vector<Report> SPGetReportListWithCID(SQLWCHAR* szSPName, int cid, bool finished);
	static vector<Test> SPGetTestListWithCID(SQLWCHAR* szSPName, int cid, bool finished);
	static void SPAnswerReport(SQLWCHAR* szSPName, Report report);
	static void SPAnswerTest(SQLWCHAR* szSPName, Test test);
	static vector<Report> SPGetReportListWithSIDandCID(SQLWCHAR* szSPName, int sid, int cid);
	static vector<Test> SPGetTestListWithSIDandCID(SQLWCHAR* szSPName, int sid, int cid);
	static string SPGetNameWithSID(SQLWCHAR* szSPName, int sid);
	static void SPUpdateGrade(SQLWCHAR* szSPName, int sid, int cid, int grade);
	static vector<pair<string, float>> SPViewGrades(SQLWCHAR* szSPName, int sid);

	//--REFERENCE
	static void CreateTable(string szSQL);

	static void TT();
};

