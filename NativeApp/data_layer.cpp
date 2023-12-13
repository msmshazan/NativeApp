#include "data_layer.h"

namespace DB {

	void* Data = NULL;
	bool IsConnectedToDatabase = false;

	void ConnectToDatabase() {
		SQLHENV env;
		SQLHDBC dbc;
		SQLRETURN ret;
		SQLWCHAR outstr[1024];
		SQLSMALLINT outstrlen;

		SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
		SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
		SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

		SQLWCHAR sqlConnectionString[] = L"DRIVER={ODBC Driver 17 for SQL Server};SERVER=localhost,1433;UID=SA;PWD=yourPassword;";
		ret = SQLDriverConnect(dbc, NULL, sqlConnectionString, SQL_NTS,
			outstr, sizeof(outstr), &outstrlen,
			SQL_DRIVER_NOPROMPT);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
		{
				
			SQLINTEGER   i = 0;
			SQLINTEGER   native;
			SQLWCHAR      state[7];
			SQLWCHAR      text[256];
			SQLSMALLINT  len;
			SQLRETURN    ret;
			SQLGetDiagRec(SQL_HANDLE_DBC, dbc, ++i, state, &native, text,
				sizeof(text), &len);
			WCHAR buffer[500];
			swprintf_s(buffer,500, L"%s:%ld:%ld:%s\n", state, i, native, text);
			OutputDebugString((WCHAR*)buffer);
			IsConnectedToDatabase = false;

		}
		else
		{
			OutputDebugString(outstr);
			IsConnectedToDatabase = true;
		}
		
	}

}