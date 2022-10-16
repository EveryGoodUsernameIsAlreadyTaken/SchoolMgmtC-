#include "StudentMgmt.h"

StudentMgmt::StudentMgmt()
{

}

StudentMgmt::~StudentMgmt()
{

}

int StudentMgmt::Login()
{
	string username, pwd;
	
	Message::Out("Username: ", 0, false);
	getline(cin, username);
	Message::Out("Password: ", 0, false);
	getline(cin, pwd);

	if (mssqldb::CheckUser(username, "STUDENTS") && mssqldb::CheckPass(pwd, "STUDENTS"))
	{
		curStudent = mssqldb::SPStudentInfo(mssqldb::GetWString("{CALL PQ_LOG_STUDENT(?,?)}"), username, pwd);
		Message::Out("Login successful!", 1, true);

		//cout << curStudent.SID << endl;
		//cout << curStudent.User << endl;
		//cout << curStudent.Pass << endl;
		//cout << curStudent.FName << endl;
		//cout << curStudent.LName << endl;
		//cout << curStudent.DOB << endl;
		//cout << curStudent.Addr1 << endl;
		//cout << curStudent.Addr2 << endl;
		//cout << curStudent.City << endl;
		//cout << curStudent.State << endl;
		//cout << curStudent.Zip << endl;
		//cout << curStudent.Email << endl;
		//cout << curStudent.PhoneNo << endl;
		//cout << curStudent.GPA << endl;
		//cout << curStudent.Fixed << endl;

		return 1;
	}
	else
	{
		Message::Out("That username and password does not exist.", 1, true);
		return 0;
	}
}

bool StudentMgmt::LogOut() 
{
	curStudent = Student();
	return true;
}

int StudentMgmt::AppendStudentInfo()
{
	auto checkuser = [&](string user) {
		if (mssqldb::CheckUser(user, "STUDENTS"))
		{
			Message::Out("Username is already in use! Please use a different username.", 2, true);
			return true;
		}
		curStudent.User = user;
		return false;
	};
	auto checkpass = [&](string pass) {
		if (mssqldb::CheckPass(pass, "STUDENTS"))
		{
			Message::Out("Password is already in use! Please use a different password.", 2, true);
			Message::Out("Username: " + curStudent.User, 1, false);
			return true;
		}
		curStudent.Pass = pass;
		return false;
	};
	auto checkempty = [&](string input, string info) {
		if (input.empty()) {
			Message::Out(info + " cannot be empty!", 2, true);
			return true;
		}
		cout << endl;
		return false;
	};
	auto checknumber = [&](string input, int max) {
		int ret;
		if (input.find_first_not_of("0123456789") == string::npos) {
			ret = stoi(input);
			if (ret >= 1 && ret <= max)
			{
				curStudent.DOB += to_string(ret);
				cout << endl;
				return false;
			}
		}
		Message::Out("Please enter a valid number between 1 and " + to_string(max) + "!", 2, true);
		return true;
	};
	auto checkzip = [&](string zip) {
		if (zip.size() != 5) {
			Message::Out("Please enter a valid Zip code!", 2, true);
			return true;
		}
		return false;
	};

	string input;
	getchar();
	Message::Out("Please type in your username and password", 2, true);
	do 
	{
		Message::Out("Username: ", 0, false);
		getline(cin, input);
	} while (checkempty(input, "Username") || checkuser(input));
	do 
	{
		Message::Out("Password: ", 0, false);
		getline(cin, input);
	} while (checkempty(input, "Password") || checkpass(input));

	do
	{
		Message::Out("First name: ", 0, false);
		getline(cin, curStudent.FName);
	} while (checkempty(curStudent.FName, "First Name"));
	do
	{
		Message::Out("Last name: ", 0, false);
		getline(cin, curStudent.LName);
	} while (checkempty(curStudent.LName, "Last Name"));

	do {
		Message::Out("Please enter your date of birth.\n\tMonth: ", 0, false);
		getline(cin, input);
	} while (checknumber(input, 12));
	curStudent.DOB += "/";
	do {
		Message::Out("Please enter your date of birth.\n\tDay: " + curStudent.DOB, 0, false);
		getline(cin, input);
	} while (checknumber(input, 30));
	curStudent.DOB += "/";
	do {
		Message::Out("Please enter your date of birth.\n\tYear: " + curStudent.DOB, 0, false);
		getline(cin, input);
	} while (checknumber(input, 2022));

	do
	{
		Message::Out("Address 1: ", 0, false);
		getline(cin, curStudent.Addr1);
	} while (checkempty(curStudent.Addr1, "Address 1"));
	Message::Out("Address 2 (Optional): ", 0, false);
	getline(cin, curStudent.Addr2);
	cout << endl;

	do
	{
		Message::Out("City: ", 0, false);
		getline(cin, curStudent.City);
	} while (checkempty(curStudent.City, "City"));
	do
	{
		Message::Out("State: ", 0, false);
		getline(cin, curStudent.State);
	} while (checkempty(curStudent.State, "State"));
	do
	{
		Message::Out("Zip code: ", 0, false);
		getline(cin, curStudent.Zip);
	} while (checkempty(curStudent.Zip, "Zip code") || checkzip(curStudent.Zip));
	do
	{
		Message::Out("Email: ", 0, false);
		getline(cin, curStudent.Email);
	} while (checkempty(curStudent.Email, "Email"));
	do
	{
		Message::Out("Phone Number: ", 0, false);
		getline(cin, curStudent.PhoneNo);
	} while (checkempty(curStudent.PhoneNo, "Phone Number"));

	curStudent.GPA = 0;
	curStudent.Fixed = false;

	curStudent.SID = mssqldb::SPInputStudent(mssqldb::GetWString("{CALL PQ_REG_STUDENT(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"), curStudent);
	
	Message::Out("Registration Successful!", 1, true);
	return 1;
}

void StudentMgmt::ViewInfo() {
	char buffer[1000];
	sprintf_s(buffer, "%-3s %-10s %-10s %-10s %-10s %-10s %-15s %-10s %-10s %-5s %-5s %-25s %-15s %5s %5s", "SID", "USERNAME", "PASSWORD", "FIRSTNAME", "LASTNAME", "DOB", "ADDR1", "ADDR2", "CITY", "STATE", "ZIP", "EMAIL", "PHONENO", "GPA", "FIXED");
	std::cout << buffer << endl;
	sprintf_s(buffer, "%-3s %-10s %-10s %-10s %-10s %-10s %-15s %-10s %-10s %-5s %-5s %-25s %-15s %5s %5s", "---", "--------", "--------", "---------", "--------", "---", "-----", "-----", "----", "-----", "---", "-----", "-------", "---", "-----");
	std::cout << buffer << endl;
	sprintf_s(buffer, "%-3s %-10s %-10s %-10s %-10s %-10s %-15s %-10s %-10s %-5s %-5s %-25s %-15s %5.2lf %5s", "XXX", curStudent.User.c_str(), curStudent.Pass.c_str(), curStudent.FName.c_str(), curStudent.LName.c_str(), curStudent.DOB.c_str(), curStudent.Addr1.c_str(), (curStudent.Addr2.empty() ? "EMPTY" : curStudent.Addr2).c_str(), curStudent.City.c_str(), curStudent.State.c_str(), curStudent.Zip.c_str(), curStudent.Email.c_str(), curStudent.PhoneNo.c_str(), curStudent.GPA, (curStudent.Fixed ? "TRUE" : "FALSE"));
	std::cout << buffer << endl;
}

void StudentMgmt::ViewStudentsList() {
	vector<Student> sList = mssqldb::SPAllStudentList(mssqldb::GetWString("{CALL PQ_STUDENT_LIST}"));
	
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

void StudentMgmt::JoinCourse() {	
	vector<string> categories = mssqldb::SPGetCategories(mssqldb::GetWString("{CALL PQ_GET_CATEGORIES}"));
	string input;
	int ichoice;

	auto getClassChoice = [&](vector<string> list) {
		for (int i = 0; i < list.size(); i++) {
			cout << "\t(" << (i + 1) << ") " << list[i] << endl;
		}

		cout << endl;
		Message::Out("Choice: ", 0, false);
		getline(cin, input);
		return stoi(input);
	};

	Message::Out("What course category do you want to join?", 2, false);

	getchar();
	ichoice = getClassChoice(categories);
	if (ichoice > 0 && ichoice <= categories.size()) {
		vector<string> classes = mssqldb::SPGetClassesByCat(mssqldb::GetWString("{CALL PQ_COURSES_IN_CAT(?)}"), categories[ichoice-1]);
		
		Message::Out("Which class would you like to take?", 2, false);
		ichoice = getClassChoice(classes);

		if (ichoice > 0 && ichoice <= classes.size()) {
			int cid = mssqldb::SPGetCIDFromName(mssqldb::GetWString("{CALL PQ_GET_CID_FROM_CLASSNAME(?)}"), classes[ichoice - 1]);
			input = mssqldb::SPGetTime(mssqldb::GetWString("{CALL PQ_GET_TIME(?)}"), cid);
			
			CourseHist CourseH = CourseHist(cid, curStudent.SID, input, "F1", 2022);
			mssqldb::SPInsertCourseHist(mssqldb::GetWString("{CALL PQ_JOIN_COURSE(?,?,?,?,?,?)}"), CourseH);

			Message::Out("Joined Course Successfully!", 2, true);
			return;
		}
	}
	Message::Out("Invalid choices!", 2, true);
}

void StudentMgmt::LeaveCourse() {
	auto isnumber = [&](string input) {
		if (input.find_first_not_of("0123456789") == string::npos) {
			return false;
		}
		Message::Out("Please enter a number!", 2, true);
		return true;
	};

	vector<pair<int, string>> classes = mssqldb::SPGetCoursesBySID(mssqldb::GetWString("{CALL PQ_VIEW_COURSES_IN_SID(?)}"), curStudent.SID);
	string input;
	int ichoice;
	
	Message::Out("Choose the class you want to delete.", 2, false);
	cout << "\t(" << 0 << ") " << "Cancel" << endl;
	for (int i = 0; i < classes.size(); i++) {
		cout << "\t(" << (i + 1) << ") " << classes[i].second << endl;
	}
	cout << endl;
	
	getchar();
	Message::Out("Choice: ", 0, false);
	getline(cin, input);
	if (!input.empty() && !isnumber(input)) {
		ichoice = stoi(input);

		if (ichoice == 0) {
			Message::Out("Cancelled Leave", 2, true);
		}
		else if (ichoice >= 1 && ichoice <= classes.size()) {
			mssqldb::ExecDBData("DELETE FROM COURSEHIST WHERE CID = '" + to_string(classes[ichoice - 1].first) + "'", DEL);
		}
		else {
			Message::Out("That is not a valid input!", 2, true);
		}
	}
	else {
		Message::Out("That is not a valid input!", 2, true);
	}
}

void StudentMgmt::IntroduceCourse() {
	vector<string> categories = mssqldb::SPGetCategories(mssqldb::GetWString("{CALL PQ_GET_CATEGORIES}"));
	string input;
	int ichoice;

	auto getClassChoice = [&](vector<string> list) {
		for (int i = 0; i < list.size(); i++) {
			cout << "\t(" << (i + 1) << ") " << list[i] << endl;
		}

		cout << endl;
		Message::Out("Choice: ", 0, false);
		getline(cin, input);
		return stoi(input);
	};
	auto isnumber = [&](string input) {
		if (input.find_first_not_of("0123456789") == string::npos) {
			return false;
		}
		Message::Out("Please enter a number!", 2, true);
		return true;
	};

	Message::Out("What course category do you want to explore?", 2, false);

	getchar();
	ichoice = getClassChoice(categories);
	if (ichoice > 0 && ichoice <= categories.size()) {
		vector<Course> courses = mssqldb::SPGetCoursesByCat(mssqldb::GetWString("{CALL PQ_COURSES_BY_CAT(?)}"), categories[ichoice-1]);
		
		Message::Out("What course would you like to explore? ", 2, false);
		for (int i = 0; i < courses.size(); i++) {
			cout << "\t(" << (i + 1) << ") " << courses[i].ClassName << endl;
		}

		cout << endl;
		Message::Out("Choice: ", 0, false);
		getline(cin, input);

		if (!input.empty() && !isnumber(input)) {
			ichoice = stoi(input);

			if (ichoice >= 1 && ichoice <= courses.size()) {
				char buffer[1000];
				sprintf_s(buffer, "%-30s %-30s %-10s %-15s %-20s", "CLASS NAME", "TEXTBOOK", "MAX SIZE", "ROOM NUMBER", "CATEGORY");
				cout << buffer << endl;
				sprintf_s(buffer, "%-30s %-30s %-10s %-15s %-20s", "----------", "--------", "--------", "-----------", "--------");
				cout << buffer << endl;
				sprintf_s(buffer, "%-30s %-30s %-10d %-15d %-20s", courses[ichoice-1].ClassName.c_str(), courses[ichoice - 1].TextBook.c_str(), courses[ichoice - 1].MaxSize, courses[ichoice - 1].RoomNo, courses[ichoice - 1].Category.c_str());
				cout << buffer << endl;
			}
			else {
				Message::Out("That is not a valid input!", 2, true);
			}
		}
	}
}

void StudentMgmt::ViewCourses() {
	vector<pair<int, string>> classes = mssqldb::SPGetCoursesBySID(mssqldb::GetWString("{CALL PQ_SELECT_COURSES_BY_SID(?)}"), curStudent.SID);

	for (int i = 0; i < classes.size(); i++) {
		cout << "Course " << i + 1 << ": " << classes[i].second << endl;
	}

	cout << endl;
}

void StudentMgmt::DoReport() {
	vector<pair<int, string>> classes = mssqldb::SPGetCoursesBySID(mssqldb::GetWString("{CALL PQ_SELECT_COURSES_BY_SID(?)}"), curStudent.SID);
	string input;
	int ichoice;

	auto checkempty = [&](string input, string info) {
		if (input.empty()) {
			Message::Out(info + " cannot be empty!", 2, true);
			return true;
		}
		cout << endl;
		return false;
	};
	auto isnumber = [&](string input) {
		if (input.find_first_not_of("0123456789") == string::npos) {
			return false;
		}
		Message::Out("Please enter a number!", 2, true);
		return true;
	};

	Message::Out("Which class are you looking for?", 2, false);
	for (int i = 0; i < classes.size(); i++) {
		cout << "Course " << i + 1 << ": " << classes[i].second << endl;
	}
	cout << endl;

	getchar();
	Message::Out("Choice: ", 0, false);
	getline(cin, input);
	if (!input.empty() && !isnumber(input)) {
		ichoice = stoi(input);
		if (ichoice >= 1 && ichoice <= classes.size()) {
			vector<Report> reportList = mssqldb::SPGetReportListWithSIDandCID(mssqldb::GetWString("{CALL PQ_FIND_REPORT_IN_CID_WITH_SID(?,?)}"), curStudent.SID, classes[ichoice - 1].first);

			Message::Out("Which report will you do?", 2, false);
			for (int i = 0; i < reportList.size(); i++) {
				cout << "Report " << i + 1 << ": " << reportList[i].Title << endl;
			}
			cout << endl;

			Message::Out("Choice: ", 0, false);
			getline(cin, input);
			if (!input.empty() && !isnumber(input)) {
				ichoice = stoi(input);
				Message::Out(reportList[ichoice - 1].Task, 2, true);
				getline(cin, reportList[ichoice - 1].Answers);

				mssqldb::SPAnswerReport(mssqldb::GetWString("{CALL PQ_UPDATE_REPORT_ANSWERS(?,?)}"), reportList[ichoice - 1]);
			}
			else {
				Message::Out("That is not a valid input!", 2, true);
			}
		}
		else {
			Message::Out("That is not a valid input!", 2, true);
		}
	}
	else {
		Message::Out("That is not a valid input!", 2, true);
	}
}

void StudentMgmt::DoTest() {
	vector<pair<int, string>> classes = mssqldb::SPGetCoursesBySID(mssqldb::GetWString("{CALL PQ_SELECT_COURSES_BY_SID(?)}"), curStudent.SID);
	string input;
	int ichoice;

	auto checkempty = [&](string input, string info) {
		if (input.empty()) {
			Message::Out(info + " cannot be empty!", 2, true);
			return true;
		}
		cout << endl;
		return false;
	};
	auto isnumber = [&](string input) {
		if (input.find_first_not_of("0123456789") == string::npos) {
			return false;
		}
		Message::Out("Please enter a number!", 2, true);
		return true;
	};

	Message::Out("Which class are you looking for?", 2, false);
	for (int i = 0; i < classes.size(); i++) {
		cout << "Course " << i + 1 << ": " << classes[i].second << endl;
	}
	cout << endl;

	getchar();
	Message::Out("Choice: ", 0, false);
	getline(cin, input);
	if (!input.empty() && !isnumber(input)) {
		ichoice = stoi(input);
		if (ichoice >= 1 && ichoice <= classes.size()) {
			vector<Test> testList = mssqldb::SPGetTestListWithSIDandCID(mssqldb::GetWString("{CALL PQ_FIND_TEST_IN_CID_WITH_SID(?,?)}"), curStudent.SID, classes[ichoice - 1].first);

			Message::Out("Which test will you do?", 2, false);
			for (int i = 0; i < testList.size(); i++) {
				cout << "Test " << i + 1 << ": " << testList[i].Subject << endl;
			}
			cout << endl;

			getchar();
			Message::Out("Choice: ", 0, false);
			getline(cin, input);
			if (!input.empty() && !isnumber(input)) {
				ichoice = stoi(input);
				Message::Out(testList[ichoice - 1].Task, 2, true);
				getline(cin, testList[ichoice - 1].Answers);

				mssqldb::SPAnswerTest(mssqldb::GetWString("{CALL PQ_UPDATE_TEST_ANSWERS(?,?)}"), testList[ichoice - 1]);
			}
			else {
				Message::Out("That is not a valid input!", 2, true);
			}
		}
		else {
			Message::Out("That is not a valid input!", 2, true);
		}
	}
	else {
		Message::Out("That is not a valid input!", 2, true);
	}
}

void StudentMgmt::ViewGrades() {
	vector<pair<string, float>> gradesList = mssqldb::SPViewGrades(mssqldb::GetWString("{CALL PQ_GET_GRADES(?)}"), curStudent.SID);

	float gpa = 0;
	for (int i = 0; i < gradesList.size(); i++) {
		cout << "\t" << gradesList[i].first << ": " << gradesList[i].second << endl;
		gpa += gradesList[i].second;
	}
	cout << endl;

	curStudent.GPA = (gpa / gradesList.size()) * 4;

	cout << "\tGPA: " << curStudent.GPA << endl;
}