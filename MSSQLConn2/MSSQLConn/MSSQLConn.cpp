// MSSQLConn.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include "MSDBConn.h"

using namespace std;

int main()
{
	// connection string
	SQLWCHAR* connStr = L"DRIVER={SQL Server};SERVER=noesys, 1433; DATABASE=CPPDB; UID=cpptest; PWD=cpptest;";

	MSDBConn* myDBConn = new MSDBConn(connStr);

	// create table
	SQLWCHAR szCreateTSQL[] = L"CREATE TABLE ORDERHISTORY (ORDERNO		INT PRIMARY KEY NOT NULL, \
				CUSTNO		INT NOT NULL, \
				ITEM		VARCHAR(20) NOT NULL DEFAULT(''), \
				QUANTITY	INT, \
				UNITPRICE	DECIMAL, \
				REMARKS		VARCHAR(30) ) ";
	//myDBConn->ExecDBData(szCreateTSQL, TABLE);
	//
	//cout << "call PQ_CRTTBL_ORDERHISTORY" << endl;
	//myDBConn->ExecSPDBData(L"PQ_CRTTBL_ORDERHISTORY");
	
	//cout << "call PQ_CUSTOMERLIST" << endl;
	//myDBConn->QueryDataBySP(L"PQ_CUSTOMERLIST");

	cout << "call PQ_CUSTOMERINFO" << endl;
	//myDBConn->QueryDataBySP(L"PQ_CUSTOMERINFO");
	myDBConn->QueryDataBySPParam(L"PQ_CUSTOMERINFO");


	
	

	/*
	// insert data
	SQLWCHAR szInsertSQL[] = L"INSERT INTO CUSTOMERS VALUES(11, 'JJ', 'Park', 'Franklin Lakes', '100 Main St', '555-111-9999');";
	myDBConn->ExecDBData(szInsertSQL, INS);
	
	// update data
	SQLWCHAR szUpdateSQL[] = L"UPDATE CUSTOMERS SET FIRSTNAME='PPeter' WHERE CUSTNO =2;";
	myDBConn->ExecDBData(szUpdateSQL, UPD);
	
	// delete data
	SQLWCHAR szDeleteSQL[] = L"DELETE FROM CUSTOMERS WHERE CUSTNO =11;";
	myDBConn->ExecDBData(szDeleteSQL, DEL);

	// select data from db
	SQLWCHAR szSQL[]= L"SELECT * FROM CUSTOMERS";
	myDBConn->QueryData(szSQL);
	SQLWCHAR szWSQL[] = L"SELECT FIRSTNAME FROM CUSTOMERS";
	myDBConn->QueryData(szWSQL);
	*/

	myDBConn->Disconnection();
	//pause the console window

	std::cout << "\nPress any key to exit..." << endl;
	getchar();
    return 0;
}

