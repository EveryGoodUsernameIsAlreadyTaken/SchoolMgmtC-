#include "TeacherMgmt.h"

TeacherMgmt::TeacherMgmt()
{

}

TeacherMgmt::~TeacherMgmt()
{

}

int TeacherMgmt::Login()
{
	string username, password;
	Message::Out("Username: ", 0, false);
	getline(cin, username);
	Message::Out("Password: ", 0, false);
	getline(cin, password);

	if (mssqldb::CheckUser(username, "TEACHERS") && mssqldb::CheckPass(password, "TEACHERS")) {

		curTeacher = mssqldb::SPTeacherInfo(mssqldb::GetWString("{CALL PQ_LOG_TEACHER(?,?)}"), username, password);
		Message::Out("Login successful!", 1, true);

		return 2;
	}
	else
	{
		Message::Out("That username and password does not exist.", 1, true);
		return 0;
	}
}

bool TeacherMgmt::LogOut() {
	curTeacher = Teacher();
	return true;
}

int TeacherMgmt::AppendTeacherInfo()
{
	auto checkuser = [&](string user) {
		if (mssqldb::CheckUser(user, "TEACHERS"))
		{
			Message::Out("Username is already in use! Please use a different username.", 2, true);
			return true;
		}
		curTeacher.User = user;
		return false;
	};
	auto checkpass = [&](string pass) {
		if (mssqldb::CheckPass(pass, "curTeacher"))
		{
			Message::Out("Password is already in use! Please use a different password.", 2, true);
			Message::Out("Username: " + curTeacher.User, 1, false);
			return true;
		}
		curTeacher.Pass = pass;
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
	auto getdepartment = [&]() {
		string input;
		int iInput;

		vector<string> Colleges = mssqldb::SPCollegeList(mssqldb::GetWString("{CALL PQ_GET_COLLEGE_LIST}"));
		for (int i = 0; i < Colleges.size(); i++)
		{
			cout << "\t(" << (i + 1) << ") " << Colleges[i] << endl;
		}
		cout << endl;
		getline(cin, input);
		iInput = stoi(input);

		if (iInput >= 1 && iInput <= Colleges.size())
		{
			curTeacher.Dptmt = Colleges[iInput - 1];
			return false;
		}
		else {
			Message::Out("Please input a valid number!", 2, true);
			return true;
		}
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
		getline(cin, curTeacher.FName);
	} while (checkempty(curTeacher.FName, "First Name"));

	do
	{
		Message::Out("Last name: ", 0, false);
		getline(cin, curTeacher.LName);
	} while (checkempty(curTeacher.LName, "Last Name"));

	do
	{
		Message::Out("Email: ", 0, false);
		getline(cin, curTeacher.Email);
	} while (checkempty(curTeacher.Email, "Email"));

	do
	{
		Message::Out("Please select your department: ", 1, false);
	} while (getdepartment());

	do {
		Message::Out("Please input the college you graduated from: ", 0, false);
		getline(cin, curTeacher.College);
	} while (checkempty(curTeacher.College, "College"));

	do {
		Message::Out("Please type the subject you are able to teach: ", 0, false);
		getline(cin, curTeacher.Subj);
	} while (checkempty(curTeacher.Subj, "Subject"));

	do
	{
		Message::Out("Phone Number: ", 0, false);
		getline(cin, curTeacher.PhoneNo);
	} while (checkempty(curTeacher.PhoneNo, "Phone Number"));

	do
	{
		Message::Out("Website: ", 0, false);
		getline(cin, curTeacher.Website);
	} while (checkempty(curTeacher.Website, "Website"));

	curTeacher.TID = mssqldb::SPInputTeacher(mssqldb::GetWString("{CALL PQ_REG_TEACHER(?,?,?,?,?,?,?,?,?,?,?)}"), curTeacher);

	Message::Out("Registration Successful!", 1, true);
	return 2;
}

void TeacherMgmt::ViewInfo() {
	char buffer[1000];
	sprintf_s(buffer, "%-5s %-10s %-10s %-10s %-10s %-15s %-15s %-20s %-30s %-15s %-30s", "TID", "USERNAME", "PASSWORD", "FIRSTNAME", "LASTNAME", "EMAIL", "DEPARTMENT", "COLLEGE", "SUBJECTS", "PHONENO", "WEBSITE");
	std::cout << buffer << endl;
	sprintf_s(buffer, "%-5s %-10s %-10s %-10s %-10s %-15s %-15s %-20s %-30s %-15s %-30s", "---", "--------", "--------", "---------", "--------", "-----", "----------", "-------", "--------", "-------", "-------");
	std::cout << buffer << endl;
	sprintf_s(buffer, "%-5s %-10s %-10s %-10s %-10s %-15s %-15s %-20s %-30s %-15s %-30s", "XXX", curTeacher.User.c_str(), curTeacher.Pass.c_str(), curTeacher.FName.c_str(), curTeacher.LName.c_str(), curTeacher.Email.c_str(), curTeacher.Dptmt.c_str(), curTeacher.College.c_str(), curTeacher.Subj.c_str(), curTeacher.PhoneNo.c_str(), curTeacher.Website.c_str());
	std::cout << buffer << endl;
}

void TeacherMgmt::ViewTeachersList() {
	vector<Teacher> tList = mssqldb::SPAllTeacherList(mssqldb::GetWString("{CALL PQ_TEACHERS_LIST}"));

	char buffer[1000];
	sprintf_s(buffer, "%-5s %-9s %-9s %-9s %-9s %-17s %-15s %-22s %-24s %-15s %-13s", "TID", "USERNAME", "PASSWORD", "FIRSTNAME", "LASTNAME", "EMAIL", "DEPARTMENT", "COLLEGE", "SUBJECTS", "PHONENO", "WEBSITE");
	std::cout << buffer << endl;
	sprintf_s(buffer, "%-5s %-9s %-9s %-9s %-9s %-17s %-15s %-22s %-24s %-15s %-13s", "---", "--------", "--------", "---------", "--------", "-----", "----------", "-------", "--------", "-------", "-------");
	std::cout << buffer << endl;
	for (int i = 0; i < tList.size(); i++) {
		sprintf_s(buffer, "%-5d %-9s %-9s %-9s %-9s %-17s %-15s %-22s %-24s %-15s %-13s", tList[i].TID, tList[i].User.c_str(), tList[i].Pass.c_str(), tList[i].FName.c_str(), tList[i].LName.c_str(), tList[i].Email.c_str(), tList[i].Dptmt.c_str(), tList[i].College.c_str(), tList[i].Subj.c_str(), tList[i].PhoneNo.c_str(), tList[i].Website.c_str());
		std::cout << buffer << endl;
	}
}

void TeacherMgmt::AppendCourse() {
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
	auto usedroom = [&](string roomno) {
		if (mssqldb::CheckRoom(roomno, "COURSES")) {
			Message::Out("That room is already in use!", 2, true);
			return true;
		}
		return false;
	};
	auto isvalidnumber = [&](int from, int to, string compare) {
		int iNum = stoi(compare);
		if (iNum >= from && iNum <= to) {
			return false;
		}
		Message::Out("That number is not valid!", 2, true);
		return true;
	};
	auto checktime = [&](string ampm) {
		if (ampm == "AM" || ampm == "PM") return false;
		Message::Out("Invalid input!", 2, true);
		return true;
	};

	string input;
	Course newCourse;
	getchar();
	do
	{
		Message::Out("Class name: ", 0, false);
		getline(cin, newCourse.ClassName);
	} while (checkempty(newCourse.ClassName, "Class name"));

	do
	{
		Message::Out("Textbook: ", 0, false);
		getline(cin, newCourse.TextBook);
	} while (checkempty(newCourse.TextBook, "Textbook"));

	do
	{
		Message::Out("Max size: ", 0, false);
		getline(cin, input);
	} while (checkempty(input, "Max size") || isnumber(input));
	newCourse.MaxSize = stoi(input);

	do
	{
		Message::Out("Room Number: ", 0, false);
		getline(cin, input);
	} while (checkempty(input, "Room Number") || isnumber(input) || usedroom(input));
	newCourse.RoomNo = stoi(input);

	do {
		Message::Out("Please type which category your course goes under: ", 0, false);
		getline(cin, newCourse.Category);
	} while (checkempty(newCourse.Category, "Category"));

	string time = "";
	do {
		Message::Out("Please type the time this course starts.", 2, false);
		Message::Out("Hours: ", 0, false);
		getline(cin, input);
	} while (checkempty(input, "Hours") || isnumber(input) || isvalidnumber(1, 12, input));
	time += input + ":";
	do {
		Message::Out("Please type the time this course starts.", 2, false);
		Message::Out("Minutes: " + time, 0, false);
		getline(cin, input);
	} while (checkempty(input, "Minutes") || isnumber(input) || isvalidnumber(0, 60, input));
	time += input + " ";
	do {
		Message::Out("Please type AM or PM.", 2, false);
		Message::Out("AM/PM: " + time, 0, false);
		getline(cin, input);
	} while (checktime(input));
	time += input;

	mssqldb::SPInsertCourse(mssqldb::GetWString("{CALL PQ_INSERT_COURSE(?,?,?,?,?)}"), newCourse);

	int cid = mssqldb::SPGetCIDFromName(mssqldb::GetWString("{CALL PQ_GET_CID_FROM_CLASSNAME(?)}"), newCourse.ClassName);

	mssqldb::SPInsertLecture(mssqldb::GetWString("{CALL PQ_INSERT_INTO_LECTURES(?,?,?)}"), cid, curTeacher.TID, time);
}

void TeacherMgmt::DeleteCourse() {
	auto isnumber = [&](string input) {
		if (input.find_first_not_of("0123456789") == string::npos) {
			return false;
		}
		Message::Out("Please enter a number!", 2, true);
		return true;
	};
	string input;
	int ichoice;

	vector<pair<int, string>> courseInfo = mssqldb::SPGetCoursesByTID(mssqldb::GetWString("{CALL PQ_SELECT_COURSES_BY_TID(?)}"), curTeacher.TID);

	Message::Out("Choose which of the following courses to delete.", 2, false);
	cout << "\t(" << 0 << ") " << "Cancel" << endl;
	for (int i = 0; i < courseInfo.size(); i++) {
		cout << "\t(" << (i + 1) << ") " << courseInfo[i].second << endl;
	}
	cout << endl;

	getchar();
	Message::Out("Choice: ", 0, false);
	getline(cin, input);
	if (!input.empty() && !isnumber(input)) {
		ichoice = stoi(input);

		if (ichoice == 0) {
			Message::Out("Canceled Deletion", 2, true);
		}
		else if (ichoice >= 1 && ichoice <= courseInfo.size()) {
			Message::Out("Are you sure you want to delete this course?\nPress (Y/y) to delete or anything else to cancel.", 2, true);

			getline(cin, input);
			if(input == "y" || input == "Y")
				mssqldb::SPDeleteCourse(mssqldb::GetWString("{CALL PQ_DELETE_COURSE(?)}"), courseInfo[ichoice - 1].first);
			else
				Message::Out("Cancelled Deletion", 2, true);
		}
		else {
			Message::Out("That is not a valid input!", 2, true);
		}
	}
	else {
		Message::Out("That is not a valid input!", 2, true);
	}
}

void TeacherMgmt::ViewCourses() {
	vector<pair<int, string>> courseInfo = mssqldb::SPGetCoursesByTID(mssqldb::GetWString("{CALL PQ_SELECT_COURSES_BY_TID(?)}"), curTeacher.TID);

	Message::Out("The courses you are currently teaching right now are", 2, false);
	for (int i = 0; i < courseInfo.size(); i++) {
		cout << "\t(" << (i + 1) << ") " << courseInfo[i].second << endl;
	}
	cout << endl;
}

void TeacherMgmt::MakeReport() {
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

	string input;
	int ichoice;

	vector<pair<int, string>> courseInfo = mssqldb::SPGetCoursesByTID(mssqldb::GetWString("{CALL PQ_SELECT_COURSES_BY_TID(?)}"), curTeacher.TID);

	Message::Out("Choose which of the following courses to give a report.", 2, false);
	cout << "\t(" << 0 << ") " << "Cancel" << endl;
	for (int i = 0; i < courseInfo.size(); i++) {
		cout << "\t(" << (i + 1) << ") " << courseInfo[i].second << endl;
	}
	cout << endl;

	getchar();
	Message::Out("Choice: ", 0, false);
	getline(cin, input);
	if (!input.empty() && !isnumber(input)) {
		ichoice = stoi(input);

		if (ichoice == 0) {
			Message::Out("Canceled Report", 2, true);
		}
		else if (ichoice >= 1 && ichoice <= courseInfo.size()) {
			vector<int> sidList = mssqldb::SPGetSIDListFromCID(mssqldb::GetWString("{CALL PQ_GET_STUDENTS_WITH_CID(?)}"), courseInfo[ichoice - 1].first);
			
			Report newReport = Report();
			newReport.TID = curTeacher.TID;
			newReport.CID = courseInfo[ichoice - 1].first;
			Message::Out("Please type in Report information.", 2, true);
			
			do {
				Message::Out("Title: ", 0, false);
				getline(cin, newReport.Title);
			} while (checkempty(newReport.Title, "Title"));

			do {
				Message::Out("Task: ", 0, false);
				getline(cin, newReport.Task);
			} while (checkempty(newReport.Task, "Task"));

			string dueDate = "";
			do {
				Message::Out("Please enter the due date.", 2, false);
				Message::Out("Month: ", 0, false);
				getline(cin, input);
			} while (checkempty(input, "Month") || isnumber(input));
			dueDate += input + "/";
			do {
				Message::Out("Please enter the due date.", 2, false);
				Message::Out("Day: " + dueDate, 0, false);
				getline(cin, input);
			} while (checkempty(input, "Day") || isnumber(input));
			dueDate += input + "/";
			do {
				Message::Out("Please enter the due date.", 2, false);
				Message::Out("Year: " + dueDate, 0, false);
				getline(cin, input);
			} while (checkempty(input, "Year") || isnumber(input));
			newReport.DueDate = dueDate + input;
			newReport.Year = stoi(input);

			for (int sid : sidList) {
				mssqldb::SPMakeReport(mssqldb::GetWString("{CALL PQ_MAKE_REPORT(?,?,?,?,?,?,?)}"), newReport, sid);
			}
			Message::Out("Successfully created a new report", 2, true);
		}
		else {
			Message::Out("That is not a valid input!", 2, true);
		}
	}
	else {
		Message::Out("That is not a valid input!", 2, true);
	}
}

void TeacherMgmt::JudgeReport() {
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

	string input;
	int ichoice;

	vector<pair<int, string>> courseInfo = mssqldb::SPGetCoursesByTID(mssqldb::GetWString("{CALL PQ_SELECT_COURSES_BY_TID(?)}"), curTeacher.TID);

	Message::Out("Choose one of the following courses to grade.", 2, false);
	cout << "\t(" << 0 << ") " << "Cancel" << endl;
	for (int i = 0; i < courseInfo.size(); i++) {
		cout << "\t(" << (i + 1) << ") " << courseInfo[i].second << endl;
	}
	cout << endl;

	getchar();
	Message::Out("Choice: ", 0, false);
	getline(cin, input);
	if (!input.empty() && !isnumber(input)) {
		ichoice = stoi(input);

		if (ichoice == 0) {
			Message::Out("Canceled Judgement", 2, true);
		}
		else if (ichoice >= 1 && ichoice <= courseInfo.size()) {
			vector<Report> reportList = mssqldb::SPGetReportListWithCID(mssqldb::GetWString("{CALL PQ_FIND_REPORT_IN_CID(?, ?)}"), courseInfo[ichoice - 1].first, true);

			for (int i = 0; i < reportList.size(); i++) {
				cout << "\t(" << mssqldb::SPGetNameWithSID(mssqldb::GetWString("{CALL PQ_GET_NAME_OF_SID(?)}"), reportList[ichoice-1].SID) << ") " << endl;
				cout << "Question: " << reportList[ichoice - 1].Task << endl;
				cout << "Answer: " << reportList[ichoice - 1].Answers << endl << endl;

				cout << "Grade (0-100): " << endl;
				getline(cin, input);
				if (!input.empty() && !isnumber(input) && stoi(input) >= 0 && stoi(input) <= 100)
					mssqldb::SPUpdateGrade(mssqldb::GetWString("{CALL PQ_UPDATE_STUDENT_GRADE(?,?,?)}"), reportList[ichoice - 1].SID, reportList[ichoice - 1].CID, stoi(input));
			}
		}
		else {
			Message::Out("That is not a valid input!", 2, true);
		}
	}
}

void TeacherMgmt::MakeTest() {
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

	string input;
	int ichoice;

	vector<pair<int, string>> courseInfo = mssqldb::SPGetCoursesByTID(mssqldb::GetWString("{CALL PQ_SELECT_COURSES_BY_TID(?)}"), curTeacher.TID);

	Message::Out("Choose which of the following courses to give a test.", 2, false);
	cout << "\t(" << 0 << ") " << "Cancel" << endl;
	for (int i = 0; i < courseInfo.size(); i++) {
		cout << "\t(" << (i + 1) << ") " << courseInfo[i].second << endl;
	}
	cout << endl;

	getchar();
	Message::Out("Choice: ", 0, false);
	getline(cin, input);
	if (!input.empty() && !isnumber(input)) {
		ichoice = stoi(input);

		if (ichoice == 0) {
			Message::Out("Canceled Test", 2, true);
		}
		else if (ichoice >= 1 && ichoice <= courseInfo.size()) {
			vector<int> sidList = mssqldb::SPGetSIDListFromCID(mssqldb::GetWString("{CALL PQ_GET_STUDENTS_WITH_CID(?)}"), courseInfo[ichoice - 1].first);

			Test newTest = Test();
			newTest.TID = curTeacher.TID;
			newTest.CID = courseInfo[ichoice - 1].first;
			Message::Out("Please type in Test information.", 2, true);

			do {
				Message::Out("Subject: ", 0, false);
				getline(cin, newTest.Subject);
			} while (checkempty(newTest.Subject, "Subject"));

			do {
				Message::Out("Task: ", 0, false);
				getline(cin, newTest.Task);
			} while (checkempty(newTest.Task, "Task"));

			string takeDate = "";
			do {
				Message::Out("Please enter the test date.", 2, false);
				Message::Out("Month: ", 0, false);
				getline(cin, input);
			} while (checkempty(input, "Month") || isnumber(input));
			takeDate += input + "/";
			do {
				Message::Out("Please enter the test date.", 2, false);
				Message::Out("Day: " + takeDate, 0, false);
				getline(cin, input);
			} while (checkempty(input, "Day") || isnumber(input));
			takeDate += input + "/";
			do {
				Message::Out("Please enter the test date.", 2, false);
				Message::Out("Year: " + takeDate, 0, false);
				getline(cin, input);
			} while (checkempty(input, "Year") || isnumber(input));
			newTest.TakeDate = takeDate + input;
			newTest.Year = stoi(input);

			for (int sid : sidList) {
				cout << "get here?" << endl;
				mssqldb::SPMakeTest(mssqldb::GetWString("{CALL PQ_MAKE_TEST(?,?,?,?,?,?,?)}"), newTest, sid);
			}
			Message::Out("Successfully created a new report", 2, true);
		}
		else {
			Message::Out("That is not a valid input!", 2, true);
		}
	}
	else {
		Message::Out("That is not a valid input!", 2, true);
	}
}

void TeacherMgmt::JudgeTest() {
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

	string input;
	int ichoice;

	vector<pair<int, string>> courseInfo = mssqldb::SPGetCoursesByTID(mssqldb::GetWString("{CALL PQ_SELECT_COURSES_BY_TID(?)}"), curTeacher.TID);

	Message::Out("Choose one of the following courses to grade.", 2, false);
	cout << "\t(" << 0 << ") " << "Cancel" << endl;
	for (int i = 0; i < courseInfo.size(); i++) {
		cout << "\t(" << (i + 1) << ") " << courseInfo[i].second << endl;
	}
	cout << endl;

	getchar();
	Message::Out("Choice: ", 0, false);
	getline(cin, input);
	if (!input.empty() && !isnumber(input)) {
		ichoice = stoi(input);

		if (ichoice == 0) {
			Message::Out("Canceled Deletion", 2, true);
		}
		else if (ichoice >= 1 && ichoice <= courseInfo.size()) {
			vector<Test> testList = mssqldb::SPGetTestListWithCID(mssqldb::GetWString("{CALL PQ_FIND_TEST_IN_CID(?)}"), courseInfo[ichoice - 1].first, true);

			for (int i = 0; i < testList.size(); i++) {
				if (testList[ichoice - 1].Graded == false) {
					cout << "\t(" << mssqldb::SPGetNameWithSID(mssqldb::GetWString("{CALL PQ_GET_NAME_OF_SID(?)}"), testList[ichoice - 1].SID) << ") " << endl;
					cout << "Question: " << testList[ichoice - 1].Task << endl;
					cout << "Answer: " << testList[ichoice - 1].Answers << endl << endl;

					cout << "Grade (0-100): " << endl;
					getline(cin, input);
					if (!input.empty() && !isnumber(input) && stoi(input) >= 0 && stoi(input) <= 100)
						mssqldb::SPUpdateGrade(mssqldb::GetWString("{CALL PQ_UPDATE_STUDENT_GRADE(?,?,?)}"), testList[ichoice - 1].SID, testList[ichoice - 1].CID, stoi(input));
				}
			}
		}
		else {
			Message::Out("That is not a valid input!", 2, true);
		}
	}
}