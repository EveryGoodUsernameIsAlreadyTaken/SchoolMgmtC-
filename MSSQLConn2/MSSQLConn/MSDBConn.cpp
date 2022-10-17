#include "stdafx.h"
#include "MSDBConn.h"



MSDBConn::MSDBConn()
{
}

MSDBConn::MSDBConn(SQLWCHAR* connString)
{
	sqlConnHandle = NULL;
	sqlStmtHandle = NULL;

	//allocations
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
		 Disconnection();

	if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
		Disconnection();

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
		Disconnection();

	cout << "Attempting connection to SQL Serer..." << endl;

	switch (SQLDriverConnect(sqlConnHandle,
		NULL,
		connString,
		//(SQLWCHAR*)L"DRIVER={SQL Server};SERVER=localhost, 1433; DATABASE=TN2Mgmt; UID=sa; PWD=mingunook2;",
		SQL_NTS,
		retconstring, 1024, NULL, SQL_DRIVER_NOPROMPT))
	{
	case SQL_SUCCESS:
		std::cout << "successfully connected to SQL Server" << endl << endl; break;
	case SQL_SUCCESS_WITH_INFO:
		std::cout << "Successfully connected to SQL Server" << endl << endl; break;
	case SQL_INVALID_HANDLE:
		std::cout << "Cound not connect to SQL Server" << endl << endl;
		Disconnection();
		break;
	case SQL_ERROR:
		std::cout << "Could not connect to SQL Server" << endl << endl;
		Disconnection();
		break;
	default:
		break;
	}

	// if there is a problem connecting then exit application
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
		Disconnection();

}


MSDBConn::~MSDBConn()
{
}

void MSDBConn::Disconnection()
{
	SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
	SQLDisconnect(sqlConnHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
}

void MSDBConn::QueryData(SQLWCHAR* szSQL)
{

	//output
	std::cout << "\n";
	std::cout << "Executing T_SQL Query..." << endl;

	//if there is a problem execuing the query then exit application
	//else display query result
	if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, szSQL, SQL_NTS))
	{
		std::cout << "Error quering SQL Server" << endl;
		Disconnection();
	}
	else {
		//declare output variable and pointer
		SQLCHAR sqlResult[SQL_RESULT_LEN];
		SQLINTEGER ptrSqlVersion;


		SQLCHAR sqlCustNo[10];
		SQLCHAR sqlFirstName[30];
		SQLCHAR sqlLastName[30];
		SQLCHAR sqlCity[30];
		SQLCHAR sqlAddress[30];
		SQLCHAR sqlPhone[13];


		char buffer[1000];

		// Print Title
		std::cout << endl;
		sprintf_s(buffer, "%-10s %-30s %-30s %-30s %-30s %-12s", "CUSTNO", "FIRSTNAME", "LASTNAME", "CITY", "ADDRESS", "PHONE");
		std::cout << buffer << endl;
		sprintf_s(buffer, "%-10s %-30s %-30s %-30s %-30s %-12s", "------", "---------", "--------", "----", "-------", "-----");
		std::cout << buffer << endl;

		while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS)
		{
			SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlCustNo, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(sqlStmtHandle, 2, SQL_CHAR, sqlFirstName, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(sqlStmtHandle, 3, SQL_CHAR, sqlLastName, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(sqlStmtHandle, 4, SQL_CHAR, sqlCity, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(sqlStmtHandle, 5, SQL_CHAR, sqlAddress, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(sqlStmtHandle, 6, SQL_CHAR, sqlPhone, SQL_RESULT_LEN, &ptrSqlVersion);


			sprintf_s(buffer, "%-10s %-30s %-30s %-30s %-30s %-12s", sqlCustNo, sqlFirstName, sqlLastName, sqlCity, sqlAddress, sqlPhone);
			std::cout << buffer << endl;

		}
	}
}


void MSDBConn::QueryDataBySP(SQLWCHAR* szSPName)
{

	//output
	std::cout << "\n";
	std::cout << "Executing T_SQL Query..." << endl;

	//if there is a problem execuing the query then exit application
	//else display query result

	if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, szSPName, SQL_NTS))
	{
		std::cout << "Error quering SQL Server" << endl;
		Disconnection();
	}
	else {
		//declare output variable and pointer
		SQLCHAR sqlResult[SQL_RESULT_LEN];
		SQLINTEGER ptrSqlVersion;


		SQLCHAR sqlCustNo[10];
		SQLCHAR sqlFirstName[30];
		SQLCHAR sqlLastName[30];
		SQLCHAR sqlCity[30];
		SQLCHAR sqlAddress[30];
		SQLCHAR sqlPhone[13];


		char buffer[1000];

		// Print Title
		std::cout << endl;
		sprintf_s(buffer, "%-10s %-30s %-30s %-30s %-30s %-12s", "CUSTNO", "FIRSTNAME", "LASTNAME", "CITY", "ADDRESS", "PHONE");
		std::cout << buffer << endl;
		sprintf_s(buffer, "%-10s %-30s %-30s %-30s %-30s %-12s", "------", "---------", "--------", "----", "-------", "-----");
		std::cout << buffer << endl;

		while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS)
		{
			SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlCustNo, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(sqlStmtHandle, 2, SQL_CHAR, sqlFirstName, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(sqlStmtHandle, 3, SQL_CHAR, sqlLastName, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(sqlStmtHandle, 4, SQL_CHAR, sqlCity, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(sqlStmtHandle, 5, SQL_CHAR, sqlAddress, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(sqlStmtHandle, 6, SQL_CHAR, sqlPhone, SQL_RESULT_LEN, &ptrSqlVersion);


			sprintf_s(buffer, "%-10s %-30s %-30s %-30s %-30s %-12s", sqlCustNo, sqlFirstName, sqlLastName, sqlCity, sqlAddress, sqlPhone);
			std::cout << buffer << endl;

		}
	}
}

void extract_error(char *fn, SQLHANDLE handle, SQLSMALLINT type)
{
	SQLINTEGER i = 0;
	SQLINTEGER NativeError;
	SQLWCHAR SQLState[7];
	SQLWCHAR MessageText[256];
	SQLSMALLINT TextLength;
	SQLRETURN ret;

	fprintf(stderr, "\nThe driver reported the following error %s\n", fn);
	do
	{
		ret = SQLGetDiagRec(type, handle, ++i, SQLState, &NativeError,
			MessageText, sizeof(MessageText), &TextLength);
		if (SQL_SUCCEEDED(ret)) {
			printf("%s:%ld:%ld:%s\n",
				SQLState, (long)i, (long)NativeError, MessageText);
		}
	} while (ret == SQL_SUCCESS);
}

void MSDBConn::QueryDataBySPParam(SQLWCHAR* szSPName)
{
	SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);

	//output
	std::cout << "\n";
	std::cout << "Executing T_SQL Query..." << endl;

	//if there is a problem execuing the query then exit application
	//else display query result

	SQLWCHAR* szCustNo = L"00002";
	SQLINTEGER cbOrderDate = 0;
	//SQLPOINTER PVPTR = L"00002";

	SWORD sParm1 = 0, sParm2 = 1;
	SDWORD cbParm1 = SQL_NTS, cbParm2 = SQL_NTS;
	
	SQLUSMALLINT PINDEX = 1;
	SQLULEN ZERO = 0;
	SQLULEN SIX = 0;

	SQLPOINTER PVPTR = "00002";
	SQLLEN cbPm;



	SQLSMALLINT INPUT = 1;
	//SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, PVPTR, 0, &cbParm1);
	//SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_LONGVARCHAR, 0, 0, szCustNo, 0, &cbParm1);

	//SQLBindParam(SQLHSTMT StatementHandle, SQLUSMALLINT ParameterNumber, SQLSMALLINT ValueType, SQLSMALLINT ParameterType,
	//	SQLULEN ColumnSize, SQLSMALLINT DecimalDigits,
	//	SQLPOINTER ParameterValuePtr, SQLLEN *StrLen_or_Ind);

	SQLCHAR szQuote[6] = "00003";


	//SQLSetStmtAttr(sqlStmtHandle, SQL_ATTR_CURSOR_SCROLLABLE, (SQLPOINTER)SQL_NONSCROLLABLE, SQL_IS_UINTEGER);
	//SQLSetStmtAttr(sqlStmtHandle, SQL_ATTR_CURSOR_SENSITIVITY, (SQLPOINTER)SQL_SENSITIVE, SQL_IS_UINTEGER);

	SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, szCustNo, 0, NULL);
	//if(rc != SQL_SUCCESS) extract_error("AA", sqlEnvHandle, SQL_HANDLE_ENV);

	//SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, ZERO, ZERO, &PVPTR, 0, &cbPm);
	//SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, PVPTR, 0, NULL);
	//SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &sParm1, 0, &cbParm1);
	//SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_LONGVARCHAR, 0, 0, szCustNo, 0, &cbParm1);
	
	if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, szSPName, SQL_NTS))
	{
		std::cout << "Error quering SQL Server" << endl;
		Disconnection();
	}
	else {
		//declare output variable and pointer
		SQLCHAR sqlResult[SQL_RESULT_LEN];
		SQLINTEGER ptrSqlVersion;


		SQLCHAR sqlCustNo[10];
		SQLCHAR sqlFirstName[30];
		SQLCHAR sqlLastName[30];
		SQLCHAR sqlCity[30];
		SQLCHAR sqlAddress[30];
		SQLCHAR sqlPhone[13];


		char buffer[1000];

		// Print Title
		std::cout << endl;
		sprintf_s(buffer, "%-10s %-30s %-30s %-30s %-30s %-12s", "CUSTNO", "FIRSTNAME", "LASTNAME", "CITY", "ADDRESS", "PHONE");
		std::cout << buffer << endl;
		sprintf_s(buffer, "%-10s %-30s %-30s %-30s %-30s %-12s", "------", "---------", "--------", "----", "-------", "-----");
		std::cout << buffer << endl;

		if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS)
		{
			SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlCustNo, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(sqlStmtHandle, 2, SQL_CHAR, sqlFirstName, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(sqlStmtHandle, 3, SQL_CHAR, sqlLastName, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(sqlStmtHandle, 4, SQL_CHAR, sqlCity, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(sqlStmtHandle, 5, SQL_CHAR, sqlAddress, SQL_RESULT_LEN, &ptrSqlVersion);
			SQLGetData(sqlStmtHandle, 6, SQL_CHAR, sqlPhone, SQL_RESULT_LEN, &ptrSqlVersion);


			sprintf_s(buffer, "%-10s %-30s %-30s %-30s %-30s %-12s", sqlCustNo, sqlFirstName, sqlLastName, sqlCity, sqlAddress, sqlPhone);
			std::cout << buffer << endl;

		}
	}
}

void MSDBConn::InsertData(SQLWCHAR* szSQL)
{
	//output
	std::cout <<endl << "Inserting Data to DB..." << endl;

	//if there is a problem execuing the query then exit application
	//else display query result
	RETCODE rc= SQLExecDirect(sqlStmtHandle, szSQL, SQL_NTS);
	if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
		ShowErrorMsg("[Insert Data]", sqlStmtHandle, SQL_HANDLE_STMT);
		//Disconnection();
	}
	else {
		std::cout << "inserted successfully" << endl;
	}
}

void MSDBConn::ExecDBData(SQLWCHAR* szSQL, DExecType DEType)
{
	//output
	string arAlert[]{ "Creating table...", "Inserting data...", "Updating Data...", "Deleting Data..." };
	cout << arAlert[(int)DEType] << endl;

	//if there is a problem execuing the query then exit application
	//else display query result
	RETCODE rc = SQLExecDirect(sqlStmtHandle, szSQL, SQL_NTS);
	if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
		ShowErrorMsg((char*)(arAlert[(int)DEType]).c_str(), sqlStmtHandle, SQL_HANDLE_STMT);
		//Disconnection();
	}
	else {
		string arAlert[]{ "Created table", "Inserted data", "Updated data", "Deleted data" };
		std::cout << arAlert[(int)DEType] << " successfully" << endl << endl;
	}
}

void MSDBConn::ExecSPDBData(SQLWCHAR* szSPName)
{
	//output
	//SQLWCHAR * strCallSP = L"PQ_CRTTBL_ORDERHISTORY";
	//char    * strProcName = "PQ_CRTTBL_ORDERHISTORY";

	//if there is a problem execuing the query then exit application
	//else display query result

	//RETCODE rc = DropProcedure(hdbc, strProcName);
	//rc = CreateProcedure(hdbc, strProcName);
	//rc = ListProcedure(hdbc, strProcName);

	//rc = SQLPrepare(sqlStmtHandle, strCallSP, SQL_NTS);

	//rc = SQLExecute(sqlStmtHandle);
	//if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
	//	ShowErrorMsg("Error", sqlStmtHandle, SQL_HANDLE_STMT);
	//}
	//else {
	//	std::cout << "Call SP - " << " successfully" << endl << endl;
	//}

	RETCODE rc = SQLExecDirect(sqlStmtHandle, szSPName, SQL_NTS);
	if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
		ShowErrorMsg("Error", sqlStmtHandle, SQL_HANDLE_STMT);
		//Disconnection();
	}
	else {
		string arAlert[]{ "Created table", "Inserted data", "Updated data", "Deleted data" };
		std::cout << "Call SP - " << " successfully" << endl << endl;
	}
}

void MSDBConn::CreateTable(SQLWCHAR* szSQL)
{
	//output
	std::cout << endl << "Creating Table to DB..." << endl;

	//if there is a problem execuing the query then exit application
	//else display query result
	RETCODE rc = SQLExecDirect(sqlStmtHandle, szSQL, SQL_NTS);
	if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO) {
		ShowErrorMsg("[Create Table]", sqlStmtHandle, SQL_HANDLE_STMT);
		//Disconnection();
	}
	else {
		std::cout << "Created table successfully" << endl;
	}
}

void MSDBConn::ShowErrorMsg(char *fn, SQLHANDLE handle, SQLSMALLINT type)
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
