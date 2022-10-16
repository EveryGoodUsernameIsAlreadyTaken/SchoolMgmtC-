#include "mssqldb.h"

// define handles and variables
//SQLHANDLE sqlConnHandle;
SQLHENV henv;
SQLHDBC hdbc1;
SQLHSTMT hstmt1;

SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];
RETCODE retcode;

void mssqldb::ConnectDB()
{
	hstmt1 = NULL;
	hdbc1 = SQL_NULL_HDBC;

	cout << "Attempting connection to SQL Serer..." << endl;

	// Allocate the ODBC environment and save handle.  
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);
	if ((retcode != SQL_SUCCESS_WITH_INFO) && (retcode != SQL_SUCCESS)) {
		printf("SQLAllocHandle(Env) Failed\n\n");
		getchar();
		DisconnectDB();
	}

	// Notify ODBC that this is an ODBC 3.0 app.  
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	if ((retcode != SQL_SUCCESS_WITH_INFO) && (retcode != SQL_SUCCESS)) {
		printf("SQLSetEnvAttr(ODBC version) Failed\n\n");
		getchar();
		DisconnectDB();
	}

	// Allocate ODBC connection handle and connect.  
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc1);
	if ((retcode != SQL_SUCCESS_WITH_INFO) && (retcode != SQL_SUCCESS)) {
		printf("SQLAllocHandle(hdbc1) Failed\n\n");
		getchar();
		DisconnectDB();
	}

	SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];
	// This sample use Integrated Security. Create the SQL Server DSN by using the Windows NT authentication.   
	//retcode = SQLConnect(hdbc1, L"DRIVER={SQL Server};SERVER=noesys, 1433; DATABASE=CPPDB; UID=cpptest; PWD=cpptest;", SQL_NTS, (SQLWCHAR*)"", SQL_NTS, (SQLWCHAR*)"", SQL_NTS);

	retcode = SQLDriverConnect(hdbc1,
		NULL,
		GetWString("DRIVER={SQL Server};SERVER=noesys, 1433; DATABASE=CPPDB; UID=cpptest; PWD=cpptest;"),
		SQL_NTS,
		retconstring, 1024, NULL, SQL_DRIVER_NOPROMPT);
	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)) {
		printf("SQLConnect() Failed\n\n");
		getchar();
		DisconnectDB();
	}

	//ConnectStetement();
}

void mssqldb::ConnectStatement()
{
	// Allocate statement handle.  
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc1, &hstmt1);
	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)) {
		printf("SQLAllocHandle(hstmt1) Failed\n\n");
		getchar();
		DisconnectDB();
	}
}

void mssqldb::DisconnectDB()
{
	if (hstmt1 != SQL_NULL_HSTMT)
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt1);

	if (hdbc1 != SQL_NULL_HDBC) {
		SQLDisconnect(hdbc1);
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc1);
	}

	if (henv != SQL_NULL_HENV)
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

void mssqldb::DisconnectStatement()
{
	if (hstmt1 != SQL_NULL_HSTMT)
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt1);
}

SQLWCHAR* mssqldb::GetWString(string szStr)
{
	int iLen = strlen(szStr.c_str()) + 1;
	SQLWCHAR* wStr = new SQLWCHAR[iLen];

	for (int i = 0; i < iLen; i++)
	{
		wStr[i] = szStr[i];
	}
	return wStr;
}

void mssqldb::ShowErrorMsg(char* fn, SQLHANDLE handle, SQLSMALLINT type)
{
	SQLSMALLINT i = 0;
	SQLINTEGER NativeError;
	SQLWCHAR SQLState[7];
	SQLWCHAR MessageText[256];
	SQLSMALLINT TextLength;
	SQLRETURN ret;

	fprintf(stderr, "\nThe driver reported the following error [%s]\n", fn);
	do
	{
		ret = SQLGetDiagRec(type, handle, ++i, SQLState, &NativeError,
			MessageText, sizeof(MessageText), &TextLength);
		if (SQL_SUCCEEDED(ret)) {
			printf_s("%s:%ld:%ld:%s\n\n", SQLState, (long)i, (long)NativeError, MessageText);
		}
	} while (ret == SQL_SUCCESS);
}

// -------------------------------------------------------

bool mssqldb::CheckUser(string user, string table)
{
	bool bRet = false;

	try {
		string szSQL = "SELECT * FROM " + table + " WHERE USERNAME = '" + user + "'";
		ConnectStatement();
		SQLWCHAR* szWSQL = GetWString(szSQL);
		if (SQL_SUCCESS == SQLExecDirect(hstmt1, szWSQL, SQL_NTS))
			if (SQLFetch(hstmt1) == SQL_SUCCESS) bRet = true;
	}
	catch (...)
	{
		cout << "cannot find account" << endl;
	}
	DisconnectStatement();

	return bRet;
}

bool mssqldb::CheckPass(string pass, string table)
{
	bool bRet = false;

	try {
		string szSQL = "SELECT * FROM " + table + " WHERE PASSWORD = '" + pass + "'";
		ConnectStatement();
		SQLWCHAR* szWSQL = GetWString(szSQL);
		if (SQL_SUCCESS == SQLExecDirect(hstmt1, szWSQL, SQL_NTS))
			if (SQLFetch(hstmt1) == SQL_SUCCESS) bRet = true;
	}
	catch (...)
	{
		cout << "cannot find account" << endl;
	}
	DisconnectStatement();

	return bRet;
}

bool mssqldb::CheckRoom(string room, string table)
{
	bool bRet = false;

	try {
		string szSQL = "SELECT * FROM " + table + " WHERE ROOMNO = '" + room + "'";
		ConnectStatement();
		SQLWCHAR* szWSQL = GetWString(szSQL);
		if (SQL_SUCCESS == SQLExecDirect(hstmt1, szWSQL, SQL_NTS))
			if (SQLFetch(hstmt1) == SQL_SUCCESS) bRet = true;
	}
	catch (...)
	{
		cout << "cannot find account" << endl;
	}
	DisconnectStatement();

	return bRet;
}

int mssqldb::GetNewIDFromTable(string idCol, string table)
{
	SQLLEN ptrSqlVersion;
	int sqlNewID = 1;

	try {
		string szSQL = "SELECT IF(COUNT(idCol) > 0, MAX(" + idCol + ") + 1 FROM " + table + ", 1";
		SQLWCHAR* szWSQL = GetWString(szSQL);
		ConnectStatement();
		if (SQL_SUCCESS == SQLExecDirect(hstmt1, szWSQL, SQL_NTS))
			if (SQLFetch(hstmt1) == SQL_SUCCESS)
			{
				SQLGetData(hstmt1, 1, SQL_INTEGER, &sqlNewID, SQL_RESULT_LEN, &ptrSqlVersion);
			}
			else cout << "No data" << endl;
	}
	catch (...)
	{
		cout << "No data" << endl;
	}

	DisconnectStatement();
	return sqlNewID;
}

void mssqldb::ExecDBData(string szSQL, DExecType DEType)
{
	string arAlert[]{ "Creating table...", "Inserting data...", "Updating Data...", "Deleting Data..." };
	cout << arAlert[(int)DEType] << endl;

	//if there is a problem execuing the query then exit application
	//else display query result
	SQLWCHAR* szWSQL = GetWString(szSQL);
	ConnectStatement();
	RETCODE rc = SQLExecDirect(hstmt1, szWSQL, SQL_NTS);
	if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
		ShowErrorMsg((char*)(arAlert[(int)DEType]).c_str(), hstmt1, SQL_HANDLE_STMT);
		//Disconnection();
	}
	else {
		string arAlert[]{ "Created table", "Inserted data", "Updated data", "Deleted data" };
		std::cout << arAlert[(int)DEType] << " successfully" << endl << endl;
	}
	DisconnectStatement();
}

void mssqldb::SQLInputStr(int ipar, int coldef, string* sInput)
{
	retcode = SQLBindParameter(hstmt1, ipar, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, coldef, 0, (SQLPOINTER)(*sInput).c_str(), 0, NULL);
	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)) {
		printf("SQLBindParameter(sParm%d) Failed\n\n", ipar);
		getchar();
	}
}

void mssqldb::SQLInputInt(int ipar, int* iInput)
{
	retcode = SQLBindParameter(hstmt1, ipar, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, iInput, 0, NULL);
	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)) {
		printf("SQLBindParameter(sParm%d) Failed\n\n", ipar);
		getchar();
	}
}

void mssqldb::SQLInputFloat(int ipar, float* dInput)
{
	retcode = SQLBindParameter(hstmt1, ipar, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, dInput, 0, NULL);
	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)) {
		printf("SQLBindParameter(sParm%d) Failed\n\n", ipar);
		getchar();
	}
}

void mssqldb::SQLInputBit(int ipar, bool* bInput)
{
	retcode = SQLBindParameter(hstmt1, ipar, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 0, 0, bInput, 0, NULL);
	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)) {
		printf("SQLBindParameter(sParm%d) Failed\n\n", ipar);
		getchar();
	}
}


//------------------------------ Stored Procedure
//------------------------------------------ STUDENTS -----------------------------------------------
//---------------------------------------------------------------------------------------------------

vector<string> mssqldb::SPCollegeList(SQLWCHAR* szSPName)
{
	ConnectStatement();

	vector<string> CollegeList;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS)) 
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else
	{
		SQLLEN ptrSqlVersion;

		SQLCHAR sqlCollegeName[30];
		
		while (SQLFetch(hstmt1) == SQL_SUCCESS) {
			SQLGetData(hstmt1, 2, SQL_CHAR, sqlCollegeName, sizeof(sqlCollegeName), &ptrSqlVersion);

			CollegeList.push_back((char*)sqlCollegeName);
		}
	}

	DisconnectStatement();
	
	return CollegeList;
}

int mssqldb::SPInputStudent(SQLWCHAR* szSPName, Student student) {
	ConnectStatement();
	SQLInputInt(1, &student.SID);
	SQLInputStr(2, 30, &student.User);
	SQLInputStr(3, 30, &student.Pass);
	SQLInputStr(4, 30, &student.FName);
	SQLInputStr(5, 30, &student.LName);
	SQLInputStr(6, 10, &student.DOB);
	SQLInputStr(7, 30, &student.Addr1);
	SQLInputStr(8, 30, &student.Addr2);
	SQLInputStr(9, 20, &student.City);
	SQLInputStr(10, 2, &student.State);
	SQLInputStr(11, 5, &student.Zip);
	SQLInputStr(12, 30, &student.Email);
	SQLInputStr(13, 15, &student.PhoneNo);
	SQLInputFloat(14, &student.GPA);
	SQLInputBit(15, &student.Fixed);

	int sid = 0;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLINTEGER sqlStudentId;
		while (SQLFetch(hstmt1) == SQL_SUCCESS)
		{
			SQLGetData(hstmt1, 1, SQL_INTEGER, &sqlStudentId, SQL_RESULT_LEN, &ptrSqlVersion);

			sid = sqlStudentId;
		}

		cout << "Inserted Student Successfully!" << endl;
	}
	DisconnectStatement();
	
	return sid;
}

Student mssqldb::SPStudentInfo(SQLWCHAR* szSPName, string username, string password) {
	ConnectStatement();
	mssqldb::SQLInputStr(1, 30, &username);
	mssqldb::SQLInputStr(2, 30, &password);

	Student curStudent;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLINTEGER sqlStudentId;
		SQLCHAR sqlUserName[30];
		SQLCHAR sqlPassWord[30];
		SQLCHAR sqlFirstName[30];
		SQLCHAR sqlLastName[30];
		SQLCHAR sqlDOB[10];
		SQLCHAR sqlADDR1[30];
		SQLCHAR sqlADDR2[30];
		SQLCHAR sqlCity[30];
		SQLCHAR sqlState[3];
		SQLCHAR sqlZIP[5];
		SQLCHAR sqlEmail[30];
		SQLCHAR sqlPhoneNo[15];
		SQLREAL sqlGPA;
		UCHAR sqlFixed;

		while (SQLFetch(hstmt1) == SQL_SUCCESS)
		{
			SQLGetData(hstmt1, 1, SQL_INTEGER, &sqlStudentId, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 2, SQL_CHAR, sqlUserName, sizeof(sqlUserName), &ptrSqlVersion);
			SQLGetData(hstmt1, 3, SQL_CHAR, sqlPassWord, sizeof(sqlPassWord), &ptrSqlVersion);
			SQLGetData(hstmt1, 4, SQL_CHAR, sqlFirstName, sizeof(sqlFirstName), &ptrSqlVersion);
			SQLGetData(hstmt1, 5, SQL_CHAR, sqlLastName, sizeof(sqlLastName), &ptrSqlVersion);
			SQLGetData(hstmt1, 6, SQL_CHAR, sqlDOB, sizeof(sqlDOB), &ptrSqlVersion);
			SQLGetData(hstmt1, 7, SQL_CHAR, sqlADDR1, sizeof(sqlADDR1), &ptrSqlVersion);
			SQLGetData(hstmt1, 8, SQL_CHAR, sqlADDR2, sizeof(sqlADDR2), &ptrSqlVersion);
			SQLGetData(hstmt1, 9, SQL_CHAR, sqlCity, sizeof(sqlCity), &ptrSqlVersion);
			SQLGetData(hstmt1, 10, SQL_CHAR, sqlState, sizeof(sqlState), &ptrSqlVersion);
			SQLGetData(hstmt1, 11, SQL_CHAR, sqlZIP, sizeof(sqlZIP), &ptrSqlVersion);
			SQLGetData(hstmt1, 12, SQL_CHAR, sqlEmail, sizeof(sqlEmail), &ptrSqlVersion);
			SQLGetData(hstmt1, 13, SQL_CHAR, sqlPhoneNo, sizeof(sqlPhoneNo), &ptrSqlVersion);
			SQLGetData(hstmt1, 14, SQL_C_FLOAT, &sqlGPA, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 15, SQL_C_BIT, &sqlFixed, SQL_RESULT_LEN, &ptrSqlVersion);

			/*		cout << sqlGPA << endl;*/

			curStudent = Student((int)sqlStudentId, (char*)sqlUserName, (char*)sqlPassWord, (char*)sqlFirstName, (char*)sqlLastName, (char*)sqlDOB, (char*)sqlADDR1, (char*)sqlADDR2, (char*)sqlCity, (char*)sqlState, (char*)sqlZIP, (char*)sqlEmail, (char*)sqlPhoneNo);
			curStudent.GPA = sqlGPA;
			curStudent.Fixed = sqlFixed;
		}
	}
	DisconnectStatement();

	return curStudent;
}

vector<Student> mssqldb::SPAllStudentList(SQLWCHAR* szSPName) {
	ConnectStatement();

	vector<Student> StudentList;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLINTEGER sqlStudentId;
		SQLCHAR sqlUserName[30];
		SQLCHAR sqlPassWord[30];
		SQLCHAR sqlFirstName[30];
		SQLCHAR sqlLastName[30];
		SQLCHAR sqlDOB[10];
		SQLCHAR sqlADDR1[30];
		SQLCHAR sqlADDR2[30];
		SQLCHAR sqlCity[30];
		SQLCHAR sqlState[3];
		SQLCHAR sqlZIP[5];
		SQLCHAR sqlEmail[30];
		SQLCHAR sqlPhoneNo[15];
		SQLREAL sqlGPA;
		UCHAR sqlFixed;

		while (SQLFetch(hstmt1) == SQL_SUCCESS)
		{
			SQLGetData(hstmt1, 1, SQL_INTEGER, &sqlStudentId, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 2, SQL_CHAR, sqlUserName, sizeof(sqlUserName), &ptrSqlVersion);
			SQLGetData(hstmt1, 3, SQL_CHAR, sqlPassWord, sizeof(sqlPassWord), &ptrSqlVersion);
			SQLGetData(hstmt1, 4, SQL_CHAR, sqlFirstName, sizeof(sqlFirstName), &ptrSqlVersion);
			SQLGetData(hstmt1, 5, SQL_CHAR, sqlLastName, sizeof(sqlLastName), &ptrSqlVersion);
			SQLGetData(hstmt1, 6, SQL_CHAR, sqlDOB, sizeof(sqlDOB), &ptrSqlVersion);
			SQLGetData(hstmt1, 7, SQL_CHAR, sqlADDR1, sizeof(sqlADDR1), &ptrSqlVersion);
			SQLGetData(hstmt1, 8, SQL_CHAR, sqlADDR2, sizeof(sqlADDR2), &ptrSqlVersion);
			SQLGetData(hstmt1, 9, SQL_CHAR, sqlCity, sizeof(sqlCity), &ptrSqlVersion);
			SQLGetData(hstmt1, 10, SQL_CHAR, sqlState, sizeof(sqlState), &ptrSqlVersion);
			SQLGetData(hstmt1, 11, SQL_CHAR, sqlZIP, sizeof(sqlZIP), &ptrSqlVersion);
			SQLGetData(hstmt1, 12, SQL_CHAR, sqlEmail, sizeof(sqlEmail), &ptrSqlVersion);
			SQLGetData(hstmt1, 13, SQL_CHAR, sqlPhoneNo, sizeof(sqlPhoneNo), &ptrSqlVersion);
			SQLGetData(hstmt1, 14, SQL_C_FLOAT, &sqlGPA, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 15, SQL_C_BIT, &sqlFixed, SQL_RESULT_LEN, &ptrSqlVersion);

			Student curStudent = Student((int)sqlStudentId, (char*)sqlUserName, (char*)sqlPassWord, (char*)sqlFirstName, (char*)sqlLastName, (char*)sqlDOB, (char*)sqlADDR1, (char*)sqlADDR2, (char*)sqlCity, (char*)sqlState, (char*)sqlZIP, (char*)sqlEmail, (char*)sqlPhoneNo);
			curStudent.GPA = sqlGPA;
			curStudent.Fixed = sqlFixed;
			StudentList.push_back(curStudent);
		}
	}
	DisconnectStatement();

	return StudentList;
}

void mssqldb::SPInsertCourseHist(SQLWCHAR* szSPName, CourseHist courseh) {
	ConnectStatement();

	SQLInputInt(1, &courseh.CID);
	SQLInputInt(2, &courseh.SID);
	SQLInputFloat(3, &courseh.Points);
	SQLInputStr(4, 20, &courseh.Time);
	SQLInputStr(5, 2, &courseh.W_Grade);
	SQLInputInt(6, &courseh.Year);

	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		printf("Joined Course Successfully!\n\n");
	}
	DisconnectStatement();
}

//------------------------------------------ TEACHERS -----------------------------------------------
//---------------------------------------------------------------------------------------------------
int mssqldb::SPInputTeacher(SQLWCHAR* szSPName, Teacher teacher) {
	ConnectStatement();
	SQLInputInt(1, &teacher.TID);
	SQLInputStr(2, 30, &teacher.User);
	SQLInputStr(3, 30, &teacher.Pass);
	SQLInputStr(4, 30, &teacher.FName);
	SQLInputStr(5, 30, &teacher.LName);
	SQLInputStr(6, 30, &teacher.Email);
	SQLInputStr(7, 30, &teacher.Dptmt);
	SQLInputStr(8, 30, &teacher.College);
	SQLInputStr(9, 50, &teacher.Subj);
	SQLInputStr(10, 15, &teacher.PhoneNo);
	SQLInputStr(11, 30, &teacher.Website);

	int tid = 0;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLINTEGER sqlTeacherId;
		while (SQLFetch(hstmt1) == SQL_SUCCESS)
		{
			SQLGetData(hstmt1, 1, SQL_INTEGER, &sqlTeacherId, SQL_RESULT_LEN, &ptrSqlVersion);

			tid = sqlTeacherId;
		}
		
		cout << "Inserted Student Successfully!" << endl;
	}
	DisconnectStatement();

	return tid;
}

Teacher mssqldb::SPTeacherInfo(SQLWCHAR* szSPName, string username, string password) {
	ConnectStatement();
	mssqldb::SQLInputStr(1, 30, &username);
	mssqldb::SQLInputStr(2, 30, &password);

	Teacher curTeacher;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLINTEGER sqlTeacherId;
		SQLCHAR sqlUserName[30];
		SQLCHAR sqlPassWord[30];
		SQLCHAR sqlFirstName[30];
		SQLCHAR sqlLastName[30];
		SQLCHAR sqlEmail[30];
		SQLCHAR sqlDepartment[30];
		SQLCHAR sqlCollege[30];
		SQLCHAR sqlSubjects[30];
		SQLCHAR sqlPhoneNo[15];
		SQLCHAR sqlWebsite[30];

		while (SQLFetch(hstmt1) == SQL_SUCCESS)
		{
			SQLGetData(hstmt1, 1, SQL_INTEGER, &sqlTeacherId, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 2, SQL_CHAR, sqlUserName, sizeof(sqlUserName), &ptrSqlVersion);
			SQLGetData(hstmt1, 3, SQL_CHAR, sqlPassWord, sizeof(sqlPassWord), &ptrSqlVersion);
			SQLGetData(hstmt1, 4, SQL_CHAR, sqlFirstName, sizeof(sqlFirstName), &ptrSqlVersion);
			SQLGetData(hstmt1, 5, SQL_CHAR, sqlLastName, sizeof(sqlLastName), &ptrSqlVersion);
			SQLGetData(hstmt1, 6, SQL_CHAR, sqlEmail, sizeof(sqlEmail), &ptrSqlVersion);
			SQLGetData(hstmt1, 7, SQL_CHAR, sqlDepartment, sizeof(sqlDepartment), &ptrSqlVersion);
			SQLGetData(hstmt1, 8, SQL_CHAR, sqlCollege, sizeof(sqlCollege), &ptrSqlVersion);
			SQLGetData(hstmt1, 9, SQL_CHAR, sqlSubjects, sizeof(sqlSubjects), &ptrSqlVersion);
			SQLGetData(hstmt1, 10, SQL_CHAR, sqlPhoneNo, sizeof(sqlPhoneNo), &ptrSqlVersion);
			SQLGetData(hstmt1, 11, SQL_CHAR, sqlWebsite, sizeof(sqlWebsite), &ptrSqlVersion);

			/*		cout << sqlGPA << endl;*/

			curTeacher = Teacher(sqlTeacherId, (char*)sqlUserName, (char*)sqlPassWord, (char*)sqlFirstName, (char*)sqlLastName, (char*)sqlEmail, (char*)sqlDepartment, (char*)sqlCollege, (char*)sqlSubjects, (char*)sqlPhoneNo, (char*)sqlWebsite);
		}
	}
	DisconnectStatement();

	return curTeacher;
}

vector<Teacher> mssqldb::SPAllTeacherList(SQLWCHAR* szSPName) {
	ConnectStatement();

	vector<Teacher> TeacherList;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLINTEGER sqlTeacherId;
		SQLCHAR sqlUserName[30];
		SQLCHAR sqlPassWord[30];
		SQLCHAR sqlFirstName[30];
		SQLCHAR sqlLastName[30];
		SQLCHAR sqlEmail[30];
		SQLCHAR sqlDepartment[30];
		SQLCHAR sqlCollege[30];
		SQLCHAR sqlSubjects[30];
		SQLCHAR sqlPhoneNo[15];
		SQLCHAR sqlWebsite[30];

		while (SQLFetch(hstmt1) == SQL_SUCCESS)
		{
			SQLGetData(hstmt1, 1, SQL_INTEGER, &sqlTeacherId, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 2, SQL_CHAR, sqlUserName, sizeof(sqlUserName), &ptrSqlVersion);
			SQLGetData(hstmt1, 3, SQL_CHAR, sqlPassWord, sizeof(sqlPassWord), &ptrSqlVersion);
			SQLGetData(hstmt1, 4, SQL_CHAR, sqlFirstName, sizeof(sqlFirstName), &ptrSqlVersion);
			SQLGetData(hstmt1, 5, SQL_CHAR, sqlLastName, sizeof(sqlLastName), &ptrSqlVersion);
			SQLGetData(hstmt1, 6, SQL_CHAR, sqlEmail, sizeof(sqlEmail), &ptrSqlVersion);
			SQLGetData(hstmt1, 7, SQL_CHAR, sqlDepartment, sizeof(sqlDepartment), &ptrSqlVersion);
			SQLGetData(hstmt1, 8, SQL_CHAR, sqlCollege, sizeof(sqlCollege), &ptrSqlVersion);
			SQLGetData(hstmt1, 9, SQL_CHAR, sqlSubjects, sizeof(sqlSubjects), &ptrSqlVersion);
			SQLGetData(hstmt1, 10, SQL_CHAR, sqlPhoneNo, sizeof(sqlPhoneNo), &ptrSqlVersion);
			SQLGetData(hstmt1, 11, SQL_CHAR, sqlWebsite, sizeof(sqlWebsite), &ptrSqlVersion);

			Teacher curTeacher = Teacher((int)sqlTeacherId, (char*)sqlUserName, (char*)sqlPassWord, (char*)sqlFirstName, (char*)sqlLastName, (char*)sqlEmail, (char*)sqlDepartment, (char*)sqlCollege, (char*)sqlSubjects, (char*)sqlPhoneNo, (char*)sqlWebsite);
			TeacherList.push_back(curTeacher);
		}
	}
	DisconnectStatement();

	return TeacherList;
}

vector<string> mssqldb::SPGetCategories(SQLWCHAR* szSPName) {
	ConnectStatement();

	vector<string> categories;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLCHAR sqlCategory[30];
		while (SQLFetch(hstmt1) == SQL_SUCCESS)
		{
			SQLGetData(hstmt1, 1, SQL_CHAR, sqlCategory, sizeof(sqlCategory), &ptrSqlVersion);

			categories.push_back((char*)sqlCategory);
		}
	}

	DisconnectStatement();

	return categories;
}

void mssqldb::SPInsertCourse(SQLWCHAR* szSPName, Course course) {
	ConnectStatement();

	SQLInputStr(1, 30, &course.ClassName);
	SQLInputStr(2, 30, &course.TextBook);
	SQLInputInt(3, &course.MaxSize);
	SQLInputInt(4, &course.RoomNo);
	SQLInputStr(5, 20, &course.Category);

	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		cout << "Inserted Course Successfully!" << endl;
	}

	DisconnectStatement();
}

void mssqldb::SPInsertLecture(SQLWCHAR* szSPName, int cid, int tid, string time) {
	ConnectStatement();

	SQLInputInt(1, &cid);
	SQLInputInt(2, &tid);
	SQLInputStr(3, 20, &time);

	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		printf("Inserted Lecture Successfully!\n\n");
		getchar();
	}

	DisconnectStatement();
}

void mssqldb::SPDeleteCourse(SQLWCHAR* szSPName, int cid) {
	ConnectStatement();

	SQLInputInt(1, &cid);

	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		printf("Deleted course successfully\n\n");
		getchar();
	}

	DisconnectStatement();
}

//------------------------------------------ COURSES ------------------------------------------------
//---------------------------------------------------------------------------------------------------

int mssqldb::SPGetCIDFromName(SQLWCHAR* szSPName, string name) {
	ConnectStatement();

	SQLInputStr(1, 30, &name);

	int cid;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLINTEGER sqlCID;
		while (SQLFetch(hstmt1) == SQL_SUCCESS)
		{
			SQLGetData(hstmt1, 1, SQL_INTEGER, &sqlCID, SQL_RESULT_LEN, &ptrSqlVersion);

			cid = sqlCID;
		}
	}
	DisconnectStatement();

	return cid;
}

string mssqldb::SPGetTime(SQLWCHAR* szSPName, int cid) {
	ConnectStatement();

	SQLInputInt(1, &cid);

	string time;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLCHAR sqlTime[20];
		while (SQLFetch(hstmt1) == SQL_SUCCESS)
		{
			SQLGetData(hstmt1, 1, SQL_CHAR, sqlTime, sizeof(sqlTime), &ptrSqlVersion);

			time = (char*)sqlTime;
		}
	}
	DisconnectStatement();

	return time;
}

string mssqldb::SPGetCourseName(SQLWCHAR* szSPName, int id) {
	ConnectStatement();

	SQLInputInt(1, &id);
	string collegeName;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLCHAR sqlCollegeName[30];
		while (SQLFetch(hstmt1) == SQL_SUCCESS)
		{
			SQLGetData(hstmt1, 1, SQL_CHAR, sqlCollegeName, sizeof(sqlCollegeName), &ptrSqlVersion);

			collegeName = (char*)sqlCollegeName;
		}

		cout << "Inserted Student Successfully!" << endl;
	}

	DisconnectStatement();

	return collegeName;
}

vector<Course> mssqldb::SPAllCoursesList(SQLWCHAR* szSPName) {
	ConnectStatement();

	vector<Course> CourseList;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLINTEGER sqlClassId;
		SQLINTEGER sqlCollegeId;
		SQLCHAR sqlClassName[30];
		SQLCHAR sqlTextbook[30];
		SQLINTEGER sqlMaxSize;
		SQLINTEGER sqlRoomNo;
		SQLCHAR sqlCategory[30];

		while (SQLFetch(hstmt1) == SQL_SUCCESS)
		{
			SQLGetData(hstmt1, 1, SQL_INTEGER, &sqlClassId, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 2, SQL_INTEGER, &sqlCollegeId, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 3, SQL_CHAR, sqlClassName, sizeof(sqlClassName), &ptrSqlVersion);
			SQLGetData(hstmt1, 4, SQL_CHAR, sqlTextbook, sizeof(sqlTextbook), &ptrSqlVersion);
			SQLGetData(hstmt1, 5, SQL_INTEGER, &sqlMaxSize, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 6, SQL_INTEGER, &sqlRoomNo, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 7, SQL_CHAR, sqlCategory, sizeof(sqlCategory), &ptrSqlVersion);

			Course curCourse = Course(sqlClassId, sqlCollegeId, (char*)sqlClassName, (char*)sqlTextbook, sqlMaxSize, sqlRoomNo, (char*)sqlCategory);
			CourseList.push_back(curCourse);
		}
	}
	DisconnectStatement();

	return CourseList;
}

vector<string> mssqldb::SPGetClassesByCat(SQLWCHAR* szSPName, string category) {
	ConnectStatement();

	SQLInputStr(1, 20, &category);

	vector<string> classes;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLCHAR sqlClassName[30];
		while (SQLFetch(hstmt1) == SQL_SUCCESS)
		{
			SQLGetData(hstmt1, 1, SQL_CHAR, sqlClassName, sizeof(sqlClassName), &ptrSqlVersion);

			classes.push_back((char*)sqlClassName);
		}
	}

	DisconnectStatement();

	return classes;
}

vector<Course> mssqldb::SPGetCoursesByCat(SQLWCHAR* szSPName, string category) {
	ConnectStatement();

	SQLInputStr(1, 20, &category);

	vector<Course> CourseList;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLINTEGER sqlClassId;
		SQLINTEGER sqlCollegeId;
		SQLCHAR sqlClassName[30];
		SQLCHAR sqlTextbook[30];
		SQLINTEGER sqlMaxSize;
		SQLINTEGER sqlRoomNo;
		SQLCHAR sqlCategory[30];

		while (SQLFetch(hstmt1) == SQL_SUCCESS)
		{
			SQLGetData(hstmt1, 1, SQL_INTEGER, &sqlClassId, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 2, SQL_INTEGER, &sqlCollegeId, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 3, SQL_CHAR, sqlClassName, sizeof(sqlClassName), &ptrSqlVersion);
			SQLGetData(hstmt1, 4, SQL_CHAR, sqlTextbook, sizeof(sqlTextbook), &ptrSqlVersion);
			SQLGetData(hstmt1, 5, SQL_INTEGER, &sqlMaxSize, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 6, SQL_INTEGER, &sqlRoomNo, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 7, SQL_CHAR, sqlCategory, sizeof(sqlCategory), &ptrSqlVersion);

			Course curCourse = Course(sqlClassId, sqlCollegeId, (char*)sqlClassName, (char*)sqlTextbook, sqlMaxSize, sqlRoomNo, (char*)sqlCategory);
			CourseList.push_back(curCourse);
		}
	}
	DisconnectStatement();

	return CourseList;
}

vector<pair<int, string>> mssqldb::SPGetCoursesByTID(SQLWCHAR* szSPName, int tid) {
	ConnectStatement();

	SQLInputInt(1, &tid);

	vector<pair<int, string>> CourseInfo;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLINTEGER sqlCID;
		SQLCHAR sqlClassName[30];

		while (SQLFetch(hstmt1) == SQL_SUCCESS)
		{
			SQLGetData(hstmt1, 1, SQL_INTEGER, &sqlCID, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 2, SQL_CHAR, sqlClassName, sizeof(sqlClassName), &ptrSqlVersion);

			CourseInfo.push_back({ sqlCID, (char*)sqlClassName });
		}
	}
	DisconnectStatement();

	return CourseInfo;
}

vector<pair<int, string>> mssqldb::SPGetCoursesBySID(SQLWCHAR* szSPName, int sid) {
	ConnectStatement();

	SQLInputInt(1, &sid);

	vector<pair<int, string>> CourseInfo;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLINTEGER sqlCID;
		SQLCHAR sqlClassName[30];

		while (SQLFetch(hstmt1) == SQL_SUCCESS)
		{
			SQLGetData(hstmt1, 1, SQL_INTEGER, &sqlCID, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 2, SQL_CHAR, sqlClassName, sizeof(sqlClassName), &ptrSqlVersion);

			CourseInfo.push_back({ sqlCID, (char*)sqlClassName });
		}
	}
	DisconnectStatement();

	return CourseInfo;
}


//--------------------------------------- REPORTS/JUDGES --------------------------------------------
//---------------------------------------------------------------------------------------------------
vector<int> mssqldb::SPGetSIDListFromCID(SQLWCHAR* szSPName, int cid) {
	ConnectStatement();

	SQLInputInt(1, &cid);

	vector<int> sidList;

	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLINTEGER sqlSID;

		while (SQLFetch(hstmt1) == SQL_SUCCESS) {
			SQLGetData(hstmt1, 1, SQL_INTEGER, &sqlSID, SQL_RESULT_LEN, &ptrSqlVersion);
			sidList.push_back(sqlSID);
		}
	}

	DisconnectStatement();
	
	return sidList;
}

void mssqldb::SPMakeReport(SQLWCHAR* szSPName, Report report, int sid) {
	ConnectStatement();

	SQLInputInt(1, &sid);
	SQLInputInt(2, &report.TID);
	SQLInputInt(3, &report.CID);
	SQLInputStr(4, 30, &report.Title);
	SQLInputStr(5, 200, &report.Task);
	SQLInputStr(6, 10, &report.DueDate);
	SQLInputInt(7, &report.Year);


	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	DisconnectStatement();
}

void mssqldb::SPMakeTest(SQLWCHAR* szSPName, Test test, int sid) {
	ConnectStatement();

	SQLInputInt(1, &sid);
	SQLInputInt(2, &test.TID);
	SQLInputInt(3, &test.CID);
	SQLInputStr(4, 30, &test.Subject);
	SQLInputStr(5, 200, &test.Task);
	SQLInputStr(6, 10, &test.TakeDate);
	SQLInputInt(7, &test.Year);

	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	DisconnectStatement();
}

vector<Report> mssqldb::SPGetReportListWithCID(SQLWCHAR* szSPName, int cid, bool finished) {
	ConnectStatement();

	SQLInputInt(1, &cid);
	SQLInputBit(2, &finished);

	vector<Report> reportList;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLINTEGER sqlReportID;
		SQLINTEGER sqlSID;
		SQLINTEGER sqlTID;
		SQLINTEGER sqlCID;
		SQLCHAR sqlTitle[30];
		SQLCHAR sqlTask[200];
		SQLCHAR sqlAnswers[2000];
		SQLCHAR sqlDueDate[10];
		SQLREAL sqlGrade;
		SQLINTEGER sqlYear;
		UCHAR sqlGraded;

		while (SQLFetch(hstmt1) == SQL_SUCCESS) {
			SQLGetData(hstmt1, 1, SQL_INTEGER, &sqlReportID, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 2, SQL_INTEGER, &sqlSID, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 3, SQL_INTEGER, &sqlTID, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 4, SQL_INTEGER, &sqlCID, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 5, SQL_CHAR, sqlTitle, sizeof(sqlTitle), &ptrSqlVersion);
			SQLGetData(hstmt1, 6, SQL_CHAR, sqlTask, sizeof(sqlTask), &ptrSqlVersion);
			SQLGetData(hstmt1, 7, SQL_CHAR, sqlAnswers, sizeof(sqlAnswers), &ptrSqlVersion);
			SQLGetData(hstmt1, 8, SQL_CHAR, sqlDueDate, sizeof(sqlDueDate), &ptrSqlVersion);
			SQLGetData(hstmt1, 9, SQL_FLOAT, &sqlGrade, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 10, SQL_INTEGER, &sqlYear, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 11, SQL_BIT, &sqlGraded, SQL_RESULT_LEN, &ptrSqlVersion);

			Report newReport;
			newReport.ReportID = sqlReportID;
			newReport.SID = sqlSID;
			newReport.TID = sqlTID;
			newReport.CID = sqlCID;
			newReport.Title = (char*)sqlTitle;
			newReport.Task = (char*)sqlTask;
			newReport.Answers = (char*)sqlAnswers;
			newReport.DueDate = (char*)sqlDueDate;
			newReport.Grade = sqlGrade;
			newReport.Year = sqlYear;
			newReport.Graded = sqlGraded;

			reportList.push_back(newReport);
		}
	}
	DisconnectStatement();

	return reportList;
}

vector<Test> mssqldb::SPGetTestListWithCID(SQLWCHAR* szSPName, int cid, bool finished) {
	ConnectStatement();

	SQLInputInt(1, &cid);
	SQLInputBit(2, &finished);

	vector<Test> testList;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLINTEGER sqlTestID;
		SQLINTEGER sqlSID;
		SQLINTEGER sqlTID;
		SQLINTEGER sqlCID;
		SQLCHAR sqlSubject[30];
		SQLCHAR sqlTask[200];
		SQLCHAR sqlAnswers[2000];
		SQLCHAR sqlTakeDate[10];
		SQLREAL sqlGrade;
		SQLINTEGER sqlYear;
		UCHAR sqlGraded;

		while (SQLFetch(hstmt1) == SQL_SUCCESS) {
			SQLGetData(hstmt1, 1, SQL_INTEGER, &sqlTestID, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 2, SQL_INTEGER, &sqlSID, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 3, SQL_INTEGER, &sqlTID, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 4, SQL_INTEGER, &sqlCID, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 5, SQL_CHAR, sqlSubject, sizeof(sqlSubject), &ptrSqlVersion);
			SQLGetData(hstmt1, 6, SQL_CHAR, sqlTask, sizeof(sqlTask), &ptrSqlVersion);
			SQLGetData(hstmt1, 7, SQL_CHAR, sqlAnswers, sizeof(sqlAnswers), &ptrSqlVersion);
			SQLGetData(hstmt1, 8, SQL_CHAR, sqlTakeDate, sizeof(sqlTakeDate), &ptrSqlVersion);
			SQLGetData(hstmt1, 9, SQL_FLOAT, &sqlGrade, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 10, SQL_INTEGER, &sqlYear, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 11, SQL_BIT, &sqlGraded, SQL_RESULT_LEN, &ptrSqlVersion);

			Test newTest;
			newTest.TestID = sqlTestID;
			newTest.SID = sqlSID;
			newTest.TID = sqlTID;
			newTest.CID = sqlCID;
			newTest.Subject = (char*)sqlSubject;
			newTest.Task = (char*)sqlTask;
			newTest.Answers = (char*)sqlAnswers;
			newTest.TakeDate = (char*)sqlTakeDate;
			newTest.Grade = sqlGrade;
			newTest.Year = sqlYear;
			newTest.Graded = sqlGraded;

			testList.push_back(newTest);
		}
	}
	DisconnectStatement();

	return testList;
}

void mssqldb::SPAnswerReport(SQLWCHAR* szSPName, Report report) {
	ConnectStatement();

	SQLInputInt(1, &report.ReportID);
	SQLInputStr(2, 2000, &report.Answers);

	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	
	DisconnectStatement();
}

void mssqldb::SPAnswerTest(SQLWCHAR* szSPName, Test test) {
	ConnectStatement();

	SQLInputInt(1, &test.TestID);
	SQLInputStr(2, 2000, &test.Answers);

	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}

	DisconnectStatement();
}

vector<Report> mssqldb::SPGetReportListWithSIDandCID(SQLWCHAR* szSPName, int sid, int cid) {
	ConnectStatement();

	SQLInputInt(1, &sid);
	SQLInputInt(2, &cid);

	vector<Report> reportList;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLINTEGER sqlReportID;
		SQLINTEGER sqlSID;
		SQLINTEGER sqlTID;
		SQLINTEGER sqlCID;
		SQLCHAR sqlTitle[30];
		SQLCHAR sqlTask[200];
		SQLCHAR sqlAnswers[2000];
		SQLCHAR sqlDueDate[10];
		SQLREAL sqlGrade;
		SQLINTEGER sqlYear;

		while (SQLFetch(hstmt1) == SQL_SUCCESS) {
			SQLGetData(hstmt1, 1, SQL_INTEGER, &sqlReportID, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 2, SQL_INTEGER, &sqlSID, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 3, SQL_INTEGER, &sqlTID, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 4, SQL_INTEGER, &sqlCID, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 5, SQL_CHAR, sqlTitle, sizeof(sqlTitle), &ptrSqlVersion);
			SQLGetData(hstmt1, 6, SQL_CHAR, sqlTask, sizeof(sqlTask), &ptrSqlVersion);
			SQLGetData(hstmt1, 7, SQL_CHAR, sqlAnswers, sizeof(sqlAnswers), &ptrSqlVersion);
			SQLGetData(hstmt1, 8, SQL_CHAR, sqlDueDate, sizeof(sqlDueDate), &ptrSqlVersion);
			SQLGetData(hstmt1, 9, SQL_FLOAT, &sqlGrade, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 10, SQL_INTEGER, &sqlYear, SQL_RESULT_LEN, &ptrSqlVersion);

			Report newReport;
			newReport.ReportID = sqlReportID;
			newReport.SID = sqlSID;
			newReport.TID = sqlTID;
			newReport.CID = sqlCID;
			newReport.Title = (char*)sqlTitle;
			newReport.Task = (char*)sqlTask;
			newReport.Answers = (char*)sqlAnswers;
			newReport.DueDate = (char*)sqlDueDate;
			newReport.Grade = sqlGrade;
			newReport.Year = sqlYear;

			reportList.push_back(newReport);
		}
	}
	DisconnectStatement();

	return reportList;
}

vector<Test> mssqldb::SPGetTestListWithSIDandCID(SQLWCHAR* szSPName, int sid, int cid) {
	ConnectStatement();

	SQLInputInt(1, &sid);
	SQLInputInt(2, &cid);

	vector<Test> testList;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLINTEGER sqlTestID;
		SQLINTEGER sqlSID;
		SQLINTEGER sqlTID;
		SQLINTEGER sqlCID;
		SQLCHAR sqlSubject[30];
		SQLCHAR sqlTask[200];
		SQLCHAR sqlAnswers[2000];
		SQLCHAR sqlTakeDate[10];
		SQLREAL sqlGrade;
		SQLINTEGER sqlYear;

		while (SQLFetch(hstmt1) == SQL_SUCCESS) {
			SQLGetData(hstmt1, 1, SQL_INTEGER, &sqlTestID, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 2, SQL_INTEGER, &sqlSID, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 3, SQL_INTEGER, &sqlTID, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 4, SQL_INTEGER, &sqlCID, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 5, SQL_CHAR, sqlSubject, sizeof(sqlSubject), &ptrSqlVersion);
			SQLGetData(hstmt1, 6, SQL_CHAR, sqlTask, sizeof(sqlTask), &ptrSqlVersion);
			SQLGetData(hstmt1, 7, SQL_CHAR, sqlAnswers, sizeof(sqlAnswers), &ptrSqlVersion);
			SQLGetData(hstmt1, 8, SQL_CHAR, sqlTakeDate, sizeof(sqlTakeDate), &ptrSqlVersion);
			SQLGetData(hstmt1, 9, SQL_FLOAT, &sqlGrade, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(hstmt1, 10, SQL_INTEGER, &sqlYear, SQL_RESULT_LEN, &ptrSqlVersion);

			Test newTest;
			newTest.TestID = sqlTestID;
			newTest.SID = sqlSID;
			newTest.TID = sqlTID;
			newTest.CID = sqlCID;
			newTest.Subject = (char*)sqlSubject;
			newTest.Task = (char*)sqlTask;
			newTest.Answers = (char*)sqlAnswers;
			newTest.TakeDate = (char*)sqlTakeDate;
			newTest.Grade = sqlGrade;
			newTest.Year = sqlYear;

			testList.push_back(newTest);
		}
	}
	DisconnectStatement();

	return testList;
}

string mssqldb::SPGetNameWithSID(SQLWCHAR* szSPName, int sid) {
	ConnectStatement();

	SQLInputInt(1, &sid);

	string name = "";
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else
	{
		SQLLEN ptrSqlVersion;

		SQLCHAR sqlFirstName[30];
		SQLCHAR sqlLastName[30];

		string fname = "", lname = "";
		while (SQLFetch(hstmt1) == SQL_SUCCESS) {
			SQLGetData(hstmt1, 1, SQL_CHAR, sqlFirstName, sizeof(sqlFirstName), &ptrSqlVersion);
			SQLGetData(hstmt1, 2, SQL_CHAR, sqlLastName, sizeof(sqlLastName), &ptrSqlVersion);

			fname = (char*)sqlFirstName;
			lname = (char*)sqlLastName;
			name = fname + " " + lname;
		}
	}

	DisconnectStatement();

	return name;
}

void mssqldb::SPUpdateGrade(SQLWCHAR* szSPName, int sid, int cid, int grade) {
	ConnectStatement();

	SQLInputInt(1, &sid);
	SQLInputInt(2, &cid);
	SQLInputInt(3, &grade);

	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}

	DisconnectStatement();
}

vector<pair<string, float>> mssqldb::SPViewGrades(SQLWCHAR* szSPName, int sid) {
	ConnectStatement();

	SQLInputInt(1, &sid);

	vector<pair<string, float>> grades;
	if (SQL_SUCCESS != SQLExecDirect(hstmt1, szSPName, SQL_NTS))
	{
		printf("SQLExecDirect Failed\n\n");
		getchar();
	}
	else {
		SQLLEN ptrSqlVersion;

		SQLCHAR sqlClassName[30];
		SQLREAL sqlGrades;

		while (SQLFetch(hstmt1) == SQL_SUCCESS) {
			SQLGetData(hstmt1, 1, SQL_CHAR, sqlClassName, sizeof(sqlClassName), &ptrSqlVersion);
			SQLGetData(hstmt1, 2, SQL_C_FLOAT, &sqlGrades, SQL_RESULT_LEN, &ptrSqlVersion);

			grades.push_back({ (char*)sqlClassName, sqlGrades });
		}
	}

	DisconnectStatement();

	return grades;
}

//-- REFERENCE
void mssqldb::CreateTable(string szSQL)
{
	std::cout << endl << "Creating Table to DB..." << endl;

	SQLWCHAR* szWSQL = GetWString(szSQL);
	ConnectStatement();
	RETCODE rc = SQLExecDirect(hstmt1, szWSQL, SQL_NTS);
	if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
		ShowErrorMsg((char*)"[Create Table]", hstmt1, SQL_HANDLE_STMT);
	}
	else {
		std::cout << "Created table successfully" << endl;
	}
	DisconnectStatement();

}


void mssqldb::TT() {
	vector<Student> sList = mssqldb::SPAllStudentList(mssqldb::GetWString("{?=CALL PQ_VIEW_STUDENTS}"));

	char buffer[1000];
	sprintf_s(buffer, "%-3s %-10s %-10s %-10s %-10s %-10s %-15s %-10s %-10s %-5s %-5s %-25s %-15s %5s %5s", "SID", "USERNAME", "PASSWORD", "FIRSTNAME", "LASTNAME", "DOB", "ADDR1", "ADDR2", "CITY", "STATE", "ZIP", "EMAIL", "PHONENO", "GPA", "FIXED");
	std::cout << buffer << endl;
	sprintf_s(buffer, "%-3s %-10s %-10s %-10s %-10s %-10s %-15s %-10s %-10s %-5s %-5s %-25s %-15s %5s %5s", "---", "--------", "--------", "---------", "--------", "---", "-----", "-----", "----", "-----", "---", "-----", "-------", "---", "-----");
	std::cout << buffer << endl;
	for (int i = 0; i < sList.size(); i++) {
		sprintf_s(buffer, "%-3d %-10s %-10s %-10s %-10s %-10s %-15s %-10s %-10s %-5s %-5s %-25s %-15s %5.2lf %5s", sList[i].SID, sList[i].User.c_str(), sList[i].Pass.c_str(), sList[i].FName.c_str(), sList[i].LName.c_str(), sList[i].DOB.c_str(), sList[i].Addr1.c_str(), (sList[i].Addr2.empty() ? "EMPTY" : sList[i].Addr2).c_str(), sList[i].City.c_str(), sList[i].State.c_str(), sList[i].Zip.c_str(), sList[i].Email.c_str(), sList[i].PhoneNo.c_str(), sList[i].GPA, (sList[i].Fixed ? "TRUE" : "FALSE"));
		std::cout << buffer << endl;
	}
}