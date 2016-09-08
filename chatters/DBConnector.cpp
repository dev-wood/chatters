#include "DBConnector.h"



/************************************************************************************
 * etc. Function definition.

************************************************************************************/
void HandleDiagnosticRecord(SQLHANDLE      hHandle,
	SQLSMALLINT    hType,
	RETCODE        RetCode)
{
	SQLSMALLINT iRec = 0;
	SQLINTEGER  iError;
	WCHAR       wszMessage[1000];
	WCHAR       wszState[SQL_SQLSTATE_SIZE + 1];

	if (RetCode == SQL_INVALID_HANDLE)
	{
		fwprintf(stderr, L"Invalid handle!\n");
		return;
	}

	while (SQLGetDiagRec(hType,
		hHandle,
		++iRec,
		wszState,
		&iError,
		wszMessage,
		(SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)),
		(SQLSMALLINT *)NULL) == SQL_SUCCESS)
	{
		// Hide data truncated..
		if (wcsncmp(wszState, L"01004", 5))
		{
			fwprintf(stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
		}
	}
}

std::string convCharT(const std::wstring & wstr)
{
	return std::string(wstr.cbegin(), wstr.cend());
}
std::wstring convCharT(const std::string & str)
{
	return std::wstring(str.cbegin(), str.cend());
}



/************************************************************************************
 * DBConnector class definition.

************************************************************************************/
DBConnector::DBConnector() : _connectionFlag(false)
{
	_init();
}
DBConnector::~DBConnector()
{
	if (_connectionFlag == true)
	{
		close();
		std::cout << "DBConnector connection closed by destructor." << std::endl;
	}
	std::cout << "~DBConnector()" << std::endl;
	//rev _closeFlag==true 일 때 처리..?
}
RETCODE DBConnector::connect()
{
	std::wstring id, pw;
	RETCODE rtnmsg;

	if (_connectionFlag == true)
	{

		std::cout << "Connection already established." << std::endl;
		return SQL_SUCCESS;
	}
	std::cout << "Select data source >>";
	std::getline(std::wcin, _dsnName);
	std::cout << "User ID >>";
	std::getline(std::wcin, _userID);
	std::cout << "Password >>";
	std::getline(std::wcin, _pswd);

	if ((rtnmsg = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_hEnv)) != SQL_SUCCESS) {
		std::cout << "Alloc handle error: SQL_HANDLE_ENV" << std::endl;
		return rtnmsg;
	}
	if ((rtnmsg = SQLSetEnvAttr(_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0)) != SQL_SUCCESS) {
		std::cout << "Set Attribute error: SQL_HANDLE_ENV" << std::endl;
		return rtnmsg;
	}

	if ((rtnmsg = SQLAllocHandle(SQL_HANDLE_DBC, _hEnv, &_hDbc)) != SQL_SUCCESS) {
		std::cout << "Alloc handle error: SQL_HANDLE_DBC" << std::endl;
		return rtnmsg;
	}
	SQLSetConnectAttr(_hDbc, 5, (SQLPOINTER)SQL_LOGIN_TIMEOUT, 0);

	if (rtnmsg = SQLConnect(_hDbc, const_cast<wchar_t *>(_dsnName.c_str()), SQL_NTS, const_cast<wchar_t *>(_userID.c_str()), SQL_NTS, const_cast<wchar_t*>(_pswd.c_str()), SQL_NTS) == SQL_ERROR)	
		// connection이 성공 시 SQL_SUCCESS, 혹은 SQL_SUCCESS_WITH_INFO 반환. 
	{
		std::cout << "DB connection error." << std::endl;
		switch (rtnmsg) {
		case SQL_ERROR:
			std::cout << "SQL_ERROR" << std::endl;
			break;
		case SQL_SUCCESS_WITH_INFO:
			std::cout << "SQL_SUCCESS_WITH_INFO" << std::endl;
			break;
		case SQL_INVALID_HANDLE:
			std::cout << "SQL_INVALID_HANDLE" << std::endl;
			break;
		default:
			std::cout << "etc error" << std::endl;
			break;
		}
		HandleDiagnosticRecord(_hDbc, SQL_HANDLE_DBC, rtnmsg);
		return rtnmsg;
	}

	_connectionFlag = true;

	return rtnmsg;
}
void DBConnector::close()
{
	if (_connectionFlag == true) {
		try {
			SQLFreeHandle(SQL_HANDLE_STMT, _hStmt);
			SQLFreeHandle(SQL_HANDLE_DBC, _hDbc);
			SQLFreeHandle(SQL_HANDLE_ENV, _hEnv);  //End the connection
		}
		catch (...)
		{
			throw;
		}
	}
	_connectionFlag = false;
}
RETCODE DBConnector::excute(const std::string & stmt)
{
	std::wstring wstr(stmt.cbegin(), stmt.cend());

	SQLAllocHandle(SQL_HANDLE_STMT, _hDbc, &_hStmt);

	RETCODE RetCode = SQLExecDirect(_hStmt, const_cast<wchar_t *>(wstr.c_str()), SQL_NTS);	// 성공 시 SQL_SUCCESS 반환
	return RetCode;
}
RETCODE DBConnector::getResultNum(SQLSMALLINT & number)
{
	return SQLNumResultCols(_hStmt, &number);
}
SQLHENV DBConnector::hEnv() const
{
	return _hEnv;
}
SQLHDBC DBConnector::hDbc() const
{
	return _hDbc;
}
SQLHSTMT DBConnector::hStmt() const
{
	return _hStmt;
}
void DBConnector::_init()
{
	_connectionFlag = false;
}
