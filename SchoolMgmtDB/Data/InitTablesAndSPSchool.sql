---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_LOG_STUDENT
-- SELECT FROM STUDENT
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_LOG_STUDENT' AND TYPE = 'P' )
  DROP PROC PQ_LOG_STUDENT
GO
CREATE PROC PQ_LOG_STUDENT
	@USERNAME		VARCHAR(30),
	@PASSWORD		VARCHAR(30)
AS
BEGIN
	SELECT * FROM STUDENTS WHERE USERNAME = @USERNAME AND PASSWORD = @PASSWORD
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_REG_STUDENT
-- INSERT INTO STUDENT
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_REG_STUDENT' AND TYPE = 'P' )
  DROP PROC PQ_REG_STUDENT
GO
CREATE PROC PQ_REG_STUDENT
	@SID		INT,
	@USERNAME	VARCHAR(30),
	@PASSWORD	VARCHAR(30),
	@FNAME		VARCHAR(30),
	@LNAME		VARCHAR(30),
	@DOB		VARCHAR(10),
	@ADDR1		VARCHAR(30),
	@ADDR2		VARCHAR(30),
	@CITY		VARCHAR(20),
	@ST			VARCHAR(2),
	@ZIP		VARCHAR(5),
	@EMAIL		VARCHAR(20),
	@PHONENO	VARCHAR(15),
	@GPA		DECIMAL(3,2),
	@FIXED		BIT
AS
BEGIN
DECLARE @NEWSID INT
	SELECT @NEWSID = COUNT(SID) FROM STUDENTS
	IF @NEWSID <= 0 SET @NEWSID = 1
	ELSE SELECT @NEWSID = MAX(SID) + 1 FROM STUDENTS
	--PRINT @NEWSID

	INSERT INTO STUDENTS VALUES(@NEWSID, @USERNAME, @PASSWORD, @FNAME, @LNAME, @DOB, @ADDR1, @ADDR2, @CITY, @ST, @ZIP, @EMAIL, @PHONENO, @GPA, @FIXED)
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_LOG_TEACHER
-- SELECT FROM TEACHER
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_LOG_TEACHER' AND TYPE = 'P' )
  DROP PROC PQ_LOG_TEACHER
GO
CREATE PROC PQ_LOG_TEACHER
	@USERNAME	VARCHAR(30),
	@PASSWORD		VARCHAR(30)
AS
BEGIN
	SELECT * FROM TEACHERS WHERE USERNAME = @USERNAME AND PASSWORD = @PASSWORD
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_REG_TEACHER
-- INSERT INTO TEACHER
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_REG_TEACHER' AND TYPE = 'P' )
  DROP PROC PQ_REG_TEACHER
GO
CREATE PROC PQ_REG_TEACHER
	@TID		INT,
	@USERNAME	VARCHAR(30),
	@PASSWORD	VARCHAR(30),
	@FNAME		VARCHAR(30),
	@LNAME		VARCHAR(30),
	@EMAIL		VARCHAR(30),
	@DEPARTMENT	VARCHAR(30),
	@COLLEGE	VARCHAR(30),
	@SUBJECTS	VARCHAR(30),
	@PHONENO	VARCHAR(15),
	@WEBSITE	VARCHAR(30)
AS
BEGIN
DECLARE @NEWTID INT
	SELECT @NEWTID = COUNT(TID) FROM TEACHERS
	IF @NEWTID <= 0 SET @NEWTID = 1
	ELSE SELECT @NEWTID = MAX(TID) + 1 FROM TEACHERS

	INSERT INTO TEACHERS VALUES(@NEWTID, @USERNAME, @PASSWORD, @FNAME, @LNAME, @EMAIL, @DEPARTMENT, @COLLEGE, @SUBJECTS, @PHONENO, @WEBSITE)
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_GET_COLLEGE_LIST
-- GET LIST OF COLLEGES
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_GET_COLLEGE_LIST' AND TYPE = 'P' )
  DROP PROC PQ_GET_COLLEGE_LIST
GO
CREATE PROC PQ_GET_COLLEGE_LIST
AS
BEGIN
	SELECT * FROM COLLEGES
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_GET_COURSES
-- GET LIST OF COURSES
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_GET_COURSE_LIST' AND TYPE = 'P' )
  DROP PROC PQ_GET_COURSE_LIST
GO
CREATE PROC PQ_GET_COURSE_LIST
AS
BEGIN
	SELECT * FROM COURSES
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_COLLEGE_NAME_WITH_ID
-- GET COLLEGE NAME WITH COLLEGE ID
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_COLLEGE_NAME_WITH_ID' AND TYPE = 'P' )
  DROP PROC PQ_COLLEGE_NAME_WITH_ID
GO
CREATE PROC PQ_COLLEGE_NAME_WITH_ID
	@COLLEGEID	INT
AS
BEGIN
	SELECT NAME FROM COLLEGES WHERE COLLEGEID = @COLLEGEID
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_COURSEIDS_WITH_SID
-- GET COURSEIDS WITH SID
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_COURSEIDS_WITH_SID' AND TYPE = 'P' )
  DROP PROC PQ_COURSEIDS_WITH_SID
GO
CREATE PROC PQ_COURSEIDS_WITH_SID
	@SID	INT
AS
BEGIN
	SELECT CID FROM COURSEHIST WHERE SID = @SID
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_COURSE_NAMES_WITH_CID
-- GET NAMES OF COURSES FROM CID
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_COURSE_NAMES_WITH_CID' AND TYPE = 'P' )
  DROP PROC PQ_COURSE_NAMES_WITH_CID
GO
CREATE PROC PQ_COURSE_NAMES_WITH_CID
	@CID	INT
AS
BEGIN
	SELECT CLASSNAME FROM COURSES WHERE CID = @CID
END
GO

--------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------
--				STUDENT SP
--------------------------------------------------------------------------------------------------------------------------


---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_STUDENT_LIST
-- VIEW ALL STUDENTS
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_STUDENT_LIST' AND TYPE = 'P' )
  DROP PROC PQ_STUDENT_LIST
GO
CREATE PROC PQ_STUDENT_LIST
AS
BEGIN
	SELECT * FROM STUDENTS
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_JOIN_COURSE
-- TAKE A COURSE
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_JOIN_COURSE' AND TYPE = 'P' )
  DROP PROC PQ_JOIN_COURSE
GO
CREATE PROC PQ_JOIN_COURSE
	@CID		INT,
	@SID		INT,
	@POINTS		DECIMAL,
	@TIME		VARCHAR(20),
	@WGRADE		VARCHAR(2),
	@T_YEAR		INT
AS
BEGIN
	INSERT INTO COURSEHIST VALUES(@CID, @SID, @POINTS, @TIME, @WGRADE, @T_YEAR, 0)
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_LEAVE_COURSE
-- LEAVE A COURSE
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_LEAVE_COURSE' AND TYPE = 'P' )
  DROP PROC PQ_LEAVE_COURSE
GO
CREATE PROC PQ_LEAVE_COURSE
	@CID		INT,
	@SID		INT
AS
BEGIN
	DELETE FROM COURSEHIST WHERE CID = @CID AND SID = @SID
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_GET_CATEGORIES
-- TO INTRODUCE A COURSE
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_GET_CATEGORIES' AND TYPE = 'P' )
  DROP PROC PQ_GET_CATEGORIES
GO
CREATE PROC PQ_GET_CATEGORIES
AS
BEGIN
	SELECT DISTINCT CATEGORY FROM COURSES ORDER BY CATEGORY
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_COURSES_IN_CAT
-- VIEW COURSES BY CATEGORY
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_COURSES_IN_CAT' AND TYPE = 'P' )
  DROP PROC PQ_COURSES_IN_CAT
GO
CREATE PROC PQ_COURSES_IN_CAT
	@CATEGORY	VARCHAR(20)
AS
BEGIN
	SELECT CLASSNAME FROM COURSES WHERE CATEGORY = @CATEGORY ORDER BY CLASSNAME
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_COURSES_BY_CAT
-- VIEW COURSES BY CATEGORY
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_COURSES_BY_CAT' AND TYPE = 'P' )
  DROP PROC PQ_COURSES_BY_CAT
GO
CREATE PROC PQ_COURSES_BY_CAT
	@CATEGORY	VARCHAR(20)
AS
BEGIN
	SELECT * FROM COURSES WHERE CATEGORY = @CATEGORY ORDER BY CLASSNAME
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_GET_CID_FROM_CLASSNAME
-- VIEW COURSES BY CATEGORY
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_GET_CID_FROM_CLASSNAME' AND TYPE = 'P' )
  DROP PROC PQ_GET_CID_FROM_CLASSNAME
GO
CREATE PROC PQ_GET_CID_FROM_CLASSNAME
	@CLASSNAME	VARCHAR(30)
AS
BEGIN
	SELECT CID FROM COURSES WHERE CLASSNAME = @CLASSNAME
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_VIEW_GRADES
-- VIEW GRADES IN COURSES
---------------------------------------------------------------------------------------
/*
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_VIEW_GRADES' AND TYPE = 'P' )
  DROP PROC PQ_VIEW_GRADES
GO
CREATE PROC PQ_VIEW_GRADES
	@SID	INT
AS
BEGIN
	SELECT COURSES.CLASSNAME, COURSEHIST.POINTS FROM COURSES INNER JOIN COURSEHIST ON COURSEHIST.CID = COURSES.CID WHERE COURSEHIST.SID = @SID
END
GO
*/


---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_SEL_REPORTS
-- VIEW REPORTS
---------------------------------------------------------------------------------------
/*
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_SEL_REPORTS' AND TYPE = 'P' )
  DROP PROC PQ_SEL_REPORTS
GO
CREATE PROC PQ_SEL_REPORTS
	@SID	 INT,
	@TITLE	VARCHAR(30)
AS
BEGIN
	SELECT * FROM REPORTS WHERE SID = @SID AND TITLE = @TITLE
END
GO
*/

--------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------
--				TEACHER SP
--------------------------------------------------------------------------------------------------------------------------

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_TEACHERS_LIST
-- VIEW ALL TEACHERS
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_TEACHERS_LIST' AND TYPE = 'P' )
  DROP PROC PQ_TEACHERS_LIST
GO
CREATE PROC PQ_TEACHERS_LIST
AS
BEGIN
	SELECT * FROM TEACHERS
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_INSERT_COURSE
-- MAKE NEW COURSE
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_INSERT_COURSE' AND TYPE = 'P' )
  DROP PROC PQ_INSERT_COURSE
GO
CREATE PROC PQ_INSERT_COURSE
	@CLASSNAME	VARCHAR(30),
	@TEXTBOOK	VARCHAR(30),
	@MAXSIZE	INT,
	@ROOMNO		INT,
	@CATEGORY	VARCHAR(20)
AS
BEGIN
DECLARE @NEWCID INT
	SELECT @NEWCID = COUNT(CID) FROM COURSES
	IF @NEWCID <= 0 SET @NEWCID = 1
	ELSE SELECT @NEWCID = MAX(CID) + 1 FROM COURSES
	
DECLARE @NEWCLGID INT
	SELECT @NEWCLGID = COUNT(COLLEGEID) FROM COURSES
	IF @NEWCLGID <= 0 SET @NEWCLGID = 1
	ELSE SELECT @NEWCLGID = MAX(COLLEGEID) + 1 FROM COURSES

	INSERT INTO COURSES VALUES(@NEWCID, @NEWCLGID, @CLASSNAME, @TEXTBOOK, @MAXSIZE, @ROOMNO, @CATEGORY)
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_DELETE_COURSE
-- DELETE A COURSE WITH CLASS ID
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_DELETE_COURSE' AND TYPE = 'P' )
  DROP PROC PQ_DELETE_COURSE
GO
CREATE PROC PQ_DELETE_COURSE
	@CID		INT
AS

	DELETE FROM COURSES WHERE CID = @CID
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_SELECT_COURSES_BY_TID
-- GET A COURSE THAT IS BEING TAUGHT
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_SELECT_COURSES_BY_TID' AND TYPE = 'P' )
  DROP PROC PQ_SELECT_COURSES_BY_TID
GO
CREATE PROC PQ_SELECT_COURSES_BY_TID
	@TID		INT
AS
BEGIN
	SELECT COURSES.CID, COURSES.CLASSNAME FROM COURSES INNER JOIN LECTURES ON LECTURES.CID = COURSES.CID WHERE LECTURES.TID = @TID
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_SELECT_COURSES_BY_SID
-- GET A COURSE THAT IS BEING LEARNED
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_SELECT_COURSES_BY_SID' AND TYPE = 'P' )
  DROP PROC PQ_SELECT_COURSES_BY_SID
GO
CREATE PROC PQ_SELECT_COURSES_BY_SID
	@SID		INT
AS
BEGIN
	SELECT COURSES.CID, COURSES.CLASSNAME FROM COURSES INNER JOIN COURSEHIST ON COURSEHIST.CID = COURSES.CID WHERE COURSEHIST.SID = @SID
END
GO
---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_GET_STUDENTS_IN_CID
-- GET A LIST OF ALL STUDENT IDS IN COURSE WITH CID
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_GET_STUDENTS_WITH_CID' AND TYPE = 'P' )
  DROP PROC PQ_GET_STUDENTS_WITH_CID
GO
CREATE PROC PQ_GET_STUDENTS_WITH_CID
	@CID		INT
AS
BEGIN
	SELECT SID FROM COURSEHIST WHERE CID = @CID
END
GO

-- SELECT * FROM COURSES

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_MAKE_REPORT
-- MAKE A NEW REPORT
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_MAKE_REPORT' AND TYPE = 'P' )
  DROP PROC PQ_MAKE_REPORT
GO
CREATE PROC PQ_MAKE_REPORT
	@SID		INT,
	@TID		INT,
	@CID		INT,
	@TITLE		VARCHAR(30),
	@TASK		VARCHAR(200),
	@DUEDATE	VARCHAR(10),
	@YEAR		INT
AS
BEGIN
DECLARE @NEWID INT
	SELECT @NEWID = COUNT(REPORTID) FROM REPORTS
	IF @NEWID <= 0 SET @NEWID = 1
	ELSE SELECT @NEWID = MAX(REPORTID) + 1 FROM REPORTS

	INSERT INTO REPORTS VALUES(@NEWID, @SID, @TID, @CID, @TITLE, @TASK, '', @DUEDATE, 0, @YEAR, 'FALSE', 'FALSE')
END
GO

--SELECT * FROM COURSES
--SELECT * FROM COURSEHIST
--SELECT * FROM LECTURES

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_MAKE_TEST
-- MAKE A NEW TEST
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_MAKE_TEST' AND TYPE = 'P' )
  DROP PROC PQ_MAKE_TEST
GO
CREATE PROC PQ_MAKE_TEST
	@SID		INT,
	@TID		INT,
	@CID		INT,
	@SUBJECT	VARCHAR(30),
	@TASK		VARCHAR(200),
	@TAKEDATE	VARCHAR(10),
	@YEAR		INT
AS
BEGIN
DECLARE @NEWID INT
	SELECT @NEWID = COUNT(TESTID) FROM TESTS
	IF @NEWID <= 0 SET @NEWID = 1
	ELSE SELECT @NEWID = MAX(TESTID) + 1 FROM TESTS

	INSERT INTO TESTS VALUES(@NEWID, @SID, @TID, @CID, @SUBJECT, @TASK, '', @TAKEDATE, 0, @YEAR, 'FALSE', 'FALSE')
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_FIND_REPORT_IN_CID
-- GET ALL REPORTS WITH CID
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_FIND_REPORT_IN_CID' AND TYPE = 'P' )
  DROP PROC PQ_FIND_REPORT_IN_CID
GO
CREATE PROC PQ_FIND_REPORT_IN_CID
	@CID		INT,
	@FINISHED	BIT
AS
BEGIN
	SELECT * FROM REPORTS WHERE CID = @CID AND FINISHED = @FINISHED
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_FIND_REPORT_IN_CID_WITH_SID
-- GET ALL REPORTS WITH CID FOR STUDENT
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_FIND_REPORT_IN_CID_WITH_SID' AND TYPE = 'P' )
  DROP PROC PQ_FIND_REPORT_IN_CID_WITH_SID
GO
CREATE PROC PQ_FIND_REPORT_IN_CID_WITH_SID
	@SID		INT,
	@CID		INT
AS
BEGIN
	SELECT * FROM REPORTS WHERE SID = @SID AND CID = @CID AND FINISHED = 'FALSE'
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_UPDATE_REPORT_ANSWERS
-- UPDATE ANSWERS IN REPORT
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_UPDATE_REPORT_ANSWERS' AND TYPE = 'P' )
  DROP PROC PQ_UPDATE_REPORT_ANSWERS
GO
CREATE PROC PQ_UPDATE_REPORT_ANSWERS
	@REPORTID		INT,
	@ANSWERS		VARCHAR(30)
AS
BEGIN
	UPDATE REPORTS SET ANSWERS = @ANSWERS, FINISHED = 'TRUE' WHERE REPORTID = @REPORTID
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_FIND_TEST_IN_CID
-- GET ALL TESTS WITH CID
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_FIND_TEST_IN_CID' AND TYPE = 'P' )
  DROP PROC PQ_FIND_TEST_IN_CID
GO
CREATE PROC PQ_FIND_TEST_IN_CID
	@CID		INT,
	@FINISHED	BIT
AS
BEGIN
	SELECT * FROM TESTS WHERE CID = @CID AND FINISHED = @FINISHED
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_FIND_TEST_IN_CID_WITH_SID
-- GET ALL TESTS WITH CID FOR STUDENT
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_FIND_TEST_IN_CID_WITH_SID' AND TYPE = 'P' )
  DROP PROC PQ_FIND_TEST_IN_CID_WITH_SID
GO
CREATE PROC PQ_FIND_TEST_IN_CID_WITH_SID
	@SID		INT,
	@CID		INT
AS
BEGIN
	SELECT * FROM TESTS WHERE SID = @SID AND CID = @CID AND FINISHED = 'FALSE'
END
GO


---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_GET_NAME_OF_SID
-- GET NAME OF A STUDENT
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_GET_NAME_OF_SID' AND TYPE = 'P' )
  DROP PROC PQ_GET_NAME_OF_SID
GO
CREATE PROC PQ_GET_NAME_OF_SID
	@SID		INT
AS
BEGIN
	SELECT FIRSTNAME, LASTNAME FROM STUDENTS WHERE SID = @SID
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_UPDATE_TEST_ANSWERS
-- UPDATE ANSWERS TO A TEST
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_UPDATE_TEST_ANSWERS' AND TYPE = 'P' )
  DROP PROC PQ_UPDATE_TEST_ANSWERS
GO
CREATE PROC PQ_UPDATE_TEST_ANSWERS
	@TESTID		INT,
	@ANSWERS	VARCHAR(100)
AS
BEGIN
	UPDATE TESTS SET ANSWERS = @ANSWERS, FINISHED = 'TRUE' WHERE TESTID = @TESTID
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_UPDATE_STUDENT_GRADE
-- UPDATE GRADE TO A STUDENT
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_UPDATE_STUDENT_GRADE' AND TYPE = 'P' )
  DROP PROC PQ_UPDATE_STUDENT_GRADE
GO
CREATE PROC PQ_UPDATE_STUDENT_GRADE
	@SID		INT,
	@CID		INT,
	@GRADE		DECIMAL
AS
BEGIN
DECLARE @NEWGRADE DECIMAL
	SELECT @NEWGRADE = POINTS * 100 * WORKDONE + @GRADE FROM COURSEHIST WHERE SID = @SID AND CID = @CID

	UPDATE COURSEHIST SET WORKDONE = WORKDONE + 1 WHERE SID = @SID AND CID = @CID
	
	UPDATE COURSEHIST SET POINTS = (@NEWGRADE/100)/WORKDONE WHERE SID = @SID AND CID = @CID;
END
GO

--SELECT * FROM REPORTS
--SELECT * FROM TESTS

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_GET_TIME
-- GET TIME OF CLASS FROM CLASS ID
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_GET_TIME' AND TYPE = 'P' )
  DROP PROC PQ_GET_TIME
GO
CREATE PROC PQ_GET_TIME
	@CID		INT
AS
BEGIN
	SELECT TIME FROM LECTURES WHERE CID = @CID
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_GET_GRADES
-- GET GRADES OF COURSES
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_GET_GRADES' AND TYPE = 'P' )
  DROP PROC PQ_GET_GRADES
GO
CREATE PROC PQ_GET_GRADES
	@SID		INT
AS
BEGIN
	SELECT COURSES.CLASSNAME, COURSEHIST.POINTS FROM COURSES INNER JOIN COURSEHIST ON COURSES.CID = COURSEHIST.CID WHERE COURSEHIST.SID = @SID
END
GO

---------------------------------------------------------------------------------------
-- [PROCEDURE]: PQ_INSERT_INTO_LECTURES
-- INSERT NEW LECTURE
---------------------------------------------------------------------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_INSERT_INTO_LECTURES' AND TYPE = 'P' )
  DROP PROC PQ_INSERT_INTO_LECTURES
GO
CREATE PROC PQ_INSERT_INTO_LECTURES
	@CID		INT,
	@TID		INT,
	@TIME		VARCHAR(20)
AS
BEGIN
	INSERT INTO LECTURES VALUES(@CID, @TID, @TIME)
END
GO

-- SELECT * FROM COURSES DELETE FROM COURSES
-- SELECT * FROM LECTURES 
-----------------------------
IF EXISTS(SELECT * FROM SYS.PROCEDURES WHERE NAME = 'PQ_TT' AND TYPE = 'P' )
  DROP PROC PQ_TT
GO
CREATE PROC PQ_TT
	@USERNAME		VARCHAR(30),
	@PASSWORD		VARCHAR(30)
AS
BEGIN
	SELECT DOB, ADDR2, CITY, ST FROM STUDENTS WHERE USERNAME = @USERNAME AND PASSWORD = @PASSWORD
END
GO
