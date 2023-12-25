#include "data_layer.h"

namespace DB {

	void* Data = NULL;
	bool IsConnectedToDatabase = false;

	void ConnectToDatabase() {

		void* env;
		void* dbc;
		short ret;
		char outstr[1024] = {};
		short outstrlen;
		SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
		SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
		SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
		char sqlConnectionString[1024] = {};
		size_t bufferSize = 0;
		char buffer[1024] = {};
		getenv_s(&bufferSize, buffer, sizeof(buffer), "DB_CONNSTR");
		memcpy(sqlConnectionString, buffer, bufferSize);
		ret = SQLDriverConnect(dbc, NULL, (SQLCHAR*)sqlConnectionString, SQL_NTS,
			(SQLCHAR*)outstr, sizeof(outstr), &outstrlen,
			SQL_DRIVER_NOPROMPT);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
		{
			int i = 0;
			int native;
			char state[7];
			char text[256];
			short len;
			ret = SQLGetDiagRec(SQL_HANDLE_DBC, dbc, ++i, (SQLCHAR*)state, (SQLINTEGER*)&native, (SQLCHAR*)text,
				sizeof(text), &len);
			char buffer[500] = {};
			sprintf_s(buffer, 500, "state:%s record_id:%ld native_error:%ld error_msg:%s\n", state, i, native, text);
			OutputDebugString(buffer);
			IsConnectedToDatabase = false;
		}
		else
		{
			OutputDebugString(outstr);
			IsConnectedToDatabase = true;
		}
	}

	void FetchDataForTable() {
	}
}