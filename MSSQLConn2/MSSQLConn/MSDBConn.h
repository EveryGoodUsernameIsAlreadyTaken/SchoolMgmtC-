#pragma once
#include <string>
#include <iostream>
#include <Windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>

enum DExecType { TABLE = 0, INS=1, UPD=2, DEL=3 };

#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000

using namespace std;

class MSDBConn
{
private:
	// define handles and variables
	SQLHANDLE sqlConnHandle;
	SQLHANDLE sqlStmtHandle;
	SQLHANDLE sqlEnvHandle;
	SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];

public:
	MSDBConn();
	MSDBConn(SQLWCHAR* connString);
	~MSDBConn();

	void Disconnection();
	void QueryData(SQLWCHAR* szSQL);
	void CreateTable(SQLWCHAR* szSQL);
	void InsertData(SQLWCHAR* szSQL);
	void ExecDBData(SQLWCHAR* szSQL, DExecType DEType);
	void ShowErrorMsg(char *fn, SQLHANDLE handle, SQLSMALLINT type);

	void ExecSPDBData(SQLWCHAR* szSQL);
	void QueryDataBySP(SQLWCHAR* szSPName);
	void QueryDataBySPParam(SQLWCHAR* szSPName);
};

