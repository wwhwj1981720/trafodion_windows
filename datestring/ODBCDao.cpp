#include "StdAfx.h"
#include "ODBCDao.h"


ODBCDao::ODBCDao(void)
{
	szDSN = "oracleconn";
	szUID = "tongji";//log name
	szAuthStr = "tongji";//passward
	henv = SQL_NULL_HENV;//定义环境句柄
	hdbc1 = SQL_NULL_HDBC;//定义数据库连接句柄    
	hstmt1 = SQL_NULL_HSTMT;//定义语句句柄
}


ODBCDao::~ODBCDao(void)
{
}

/*
int ODBCDao::insertInto(RowBean *bean)
{
	

	RowObject *pnext=bean->p;
	// Allocate a statement handle.
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc1, &hstmt1);
	if(retcode < 0 ) //错误处理
	{
		cout<<"allocate ODBC statement handle errors."<<endl;
		return -1;
	}
	char  sqlstr[150]="insert into cdr values('";
	int i=0;
	//RowObject *pnext=beanobject.p;
	for (;i<M;i++)
	{

		strcat(sqlstr,pnext->timefirst);
		strcat(sqlstr,"','");
		strcat(sqlstr,pnext->type);
		strcat(sqlstr,"','");
		strcat(sqlstr,pnext->timenow);
		strcat(sqlstr,"','");
		strcat(sqlstr,pnext->timediff);
		//strcat(sqlstr,"','");
		strcat(sqlstr,"',");
		strcat(sqlstr,pnext->mobile);
		//strcat(sqlstr,"','");
		strcat(sqlstr,",");
		strcat(sqlstr,pnext->mobileout);
		//strcat(sqlstr,"')");
		strcat(sqlstr,")");
		//"insert into cdr values('150416','01','20150417061856','20150420072516','18611701625','18601106193')"

		retcode = SQLExecDirect(hstmt1,(SQLCHAR*)sqlstr,SQL_NTS);
		if(retcode<0)
		{
			cout<<"s1 insert errors."<<endl;
			return -1;
		}
		memset(sqlstr+strlen("insert into cdr values('"),0,150-strlen("insert into cdr values('"));
		pnext++;
	}

}
*/

int ODBCDao::getConn(void)
{
	//RETCODE retcode;//错误返回码

	// Allocate the ODBC Environment and save handle.
	retcode = SQLAllocHandle (SQL_HANDLE_ENV, NULL, &henv); //设置环境
	if(retcode < 0 )//错误处理
	{
		cout<<"allocate ODBC Environment handle errors."<<endl;
		return -1;
	}

	// Notify ODBC that this is an ODBC 3.0 application.
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION,
		(SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);
	if(retcode < 0 ) //错误处理
	{
		cout<<"the  ODBC is not version3.0 "<<endl;
		return -1;
	}

	// Allocate an ODBC connection and connect.
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc1);
	if(retcode < 0 ) //错误处理
	{
		cout<<"allocate ODBC connection handle errors."<<endl;
		return -1;
	}

	//Data Source Name must be of type User DNS or System DNS


	//connect to the Data Source
	retcode=SQLConnect(hdbc1,(SQLCHAR*)szDSN,(SWORD)strlen(szDSN),(SQLCHAR*)szUID, (SWORD)strlen(szUID),(SQLCHAR*)szAuthStr, (SWORD)strlen(szAuthStr));
	//return 0;
	if(retcode < 0 ) //错误处理
	{  
		cout<<"connect to  ODBC datasource errors."<<endl;
		return -1;
	}
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc1, &hstmt1);
}

  


int ODBCDao::insertBulkInto(RowBean *bean)
{
	
	RowObject *ordwth_array=bean->p;
	//retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc1, &hstmt1);
	if(retcode < 0 ) //错误处理
	{
		cout<<"allocate ODBC statement handle errors."<<endl;
		return -1;
	}
	//

	//retcode = SQLExecDirect(hstmt1, (SQLCHAR*)"select PARTION,CDR_TYPE,STARTTIME ,STOPTIME,CALLER,CALLEE  from cdr", SQL_NTS);
	
	//retcode = SQLFetchScroll(hstmt1, SQL_FETCH_NEXT, 0);

	//
	SQLSetStmtAttr(hstmt1, SQL_ATTR_ROW_ARRAY_SIZE, (SQLPOINTER)ROW_ARRAY_SIZE, 0);//ROW_ARRAY_SIZE 一个bean 里面有 1000个的 row
	SQLSetStmtAttr(hstmt1, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)sizeof(RowObject), 0); //一行数据的大小 rowobject 每一个行是一个  rowobject
	SQLUSMALLINT ParamStatusArray[ROW_ARRAY_SIZE] = { 0 }; 
	retcode = SQLSetStmtAttr(hstmt1, SQL_ATTR_ROW_STATUS_PTR, ParamStatusArray, 0);
	SQLINTEGER nBindOffset = 0;
	SQLSetStmtAttr(hstmt1, SQL_ATTR_ROW_BIND_OFFSET_PTR, (SQLPOINTER)&nBindOffset, 0);

	retcode = SQLSetStmtAttr(hstmt1, SQL_ATTR_CONCURRENCY, (SQLPOINTER)SQL_CONCUR_ROWVER, 0);

	retcode = SQLSetStmtAttr(hstmt1, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_DYNAMIC, 0);
	SQLINTEGER sCustID;
	
	retcode = SQLBindCol(hstmt1, 1, SQL_C_CHAR, &ordwth_array[0].timefirst, 7, NULL);  
	SQLBindCol(hstmt1, 2, SQL_C_CHAR, &ordwth_array[0].type, 3, NULL);  
	SQLBindCol(hstmt1, 3, SQL_C_CHAR, &ordwth_array[0].timenow, 15, NULL);  
	SQLBindCol(hstmt1, 4, SQL_C_CHAR, &ordwth_array[0].timediff, 15,NULL);  
	SQLBindCol(hstmt1, 5, SQL_C_CHAR, &ordwth_array[0].mobile, 12, NULL);  
	SQLBindCol(hstmt1, 6, SQL_C_CHAR, &ordwth_array[0].mobileout, 12,NULL);  
	SQLSetConnectAttr(hdbc1, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_IS_INTEGER);  
	retcode = SQLBulkOperations(hstmt1, SQL_ADD); 
	if (retcode == SQL_ERROR)  
	{  
		SQLCHAR msg[128];  
		SQLCHAR state[128];  
		SQLINTEGER error_id;  
		SQLSMALLINT text;  
		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt1, 1, state, &error_id, msg, 128, &text);  
		printf("db Insert fail, sqlstate=%s, errormsg=%s\n", state, msg);  
		SQLEndTran(SQL_HANDLE_DBC, hdbc1, SQL_ROLLBACK);
		SQLFreeHandle(SQL_HANDLE_STMT, &hstmt1);
		system("pause");  
		return retcode;  
	}  
	else if (retcode == SQL_SUCCESS_WITH_INFO)  
	{  
		SQLCHAR msg[128];  
		SQLCHAR state[128];  
		SQLINTEGER error_id;  
		SQLSMALLINT text;  
		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt1, 1, state, &error_id, msg, 128, &text);  
		printf("warning msg=%s\n", msg);  
	}  
	retcode = SQLEndTran(SQL_HANDLE_DBC, hdbc1, SQL_COMMIT); 
	SQLFreeHandle(SQL_HANDLE_STMT, &hstmt1);
}



int ODBCDao::insertbulkParamInto(RowBean *bean)
{
	// Allocate a statement handle.

	
	RowObject *ordwth_array=bean->p;
	
	//printf("Inserting...\n");
	
	if(retcode < 0 ) //错误处理
	{
		cout<<"allocate ODBC statement handle errors."<<endl;
		return -1;
	}
	SQLSetStmtAttr(hstmt1, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)sizeof(RowObject), 0);

	retcode = SQLSetStmtAttr(hstmt1, SQL_ATTR_PARAM_BIND_TYPE, (SQLPOINTER)sizeof(RowObject), SQL_IS_INTEGER);

	//设定每次参数的数量
	retcode = SQLSetStmtAttr(hstmt1, SQL_ATTR_PARAMSET_SIZE, (SQLPOINTER)(long)ROW_ARRAY_SIZE, SQL_IS_INTEGER);

	SQLUSMALLINT ParamStatusArray[ROW_ARRAY_SIZE] = { 0 };
	//设定状态数组
	retcode = SQLSetStmtAttr(hstmt1, SQL_ATTR_ROW_STATUS_PTR, ParamStatusArray, 0);
	SQLINTEGER nBindOffset = 0;
	SQLSetStmtAttr(hstmt1, SQL_ATTR_ROW_BIND_OFFSET_PTR, (SQLPOINTER)&nBindOffset, 0);

	
	SQLCHAR *sql = (SQLCHAR*)"Insert into cdr_phonenumber_salt ( partion,cdr_type,starttime ,stoptime,caller,callee,lac,ci,emei,emsi ) Values(?, ?, ?, ?, ?,?, ?, ?, ?,?) ";
	
	retcode = SQLPrepare(hstmt1, sql, SQL_NTS);
	//SQLINTEGER sCustID;
	//参数说明  
	SQLBindParameter(hstmt1, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 7, 0, &ordwth_array[0].timefirst, sizeof(ordwth_array[0].timefirst), NULL);
    SQLBindParameter(hstmt1, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 3 , 0, &ordwth_array[0].type, sizeof(ordwth_array[0].type), NULL);
    SQLBindParameter(hstmt1, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 15 , 0, &ordwth_array[0].timenow, sizeof(ordwth_array[0].timenow), NULL);
	SQLBindParameter(hstmt1, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 15 , 0, &ordwth_array[0].timediff, sizeof(ordwth_array[0].timediff), NULL);
	SQLBindParameter(hstmt1, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 12 , 0, &ordwth_array[0].mobile, sizeof(ordwth_array[0].mobile), NULL);
	SQLBindParameter(hstmt1, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 12, 0, &ordwth_array[0].mobileout, sizeof(ordwth_array[0].mobileout), NULL);
	SQLBindParameter(hstmt1, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 6 , 0, &ordwth_array[0].lac, sizeof(ordwth_array[0].lac), NULL);
	SQLBindParameter(hstmt1, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 6, 0, &ordwth_array[0].ci, sizeof(ordwth_array[0].ci), NULL);
	SQLBindParameter(hstmt1, 9, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 16 , 0, &ordwth_array[0].emei, sizeof(ordwth_array[0].emei), NULL);
	SQLBindParameter(hstmt1, 10, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 16, 0, &ordwth_array[0].emsi, sizeof(ordwth_array[0].emsi), NULL);
	

	SQLSetConnectAttr(hdbc1, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, 0);
	
	//执行语句
	retcode = SQLExecute(hstmt1);

		if (retcode == 99||retcode==-1)
		{
			SQLCHAR msg[128];
			SQLCHAR state[128];
			SQLINTEGER error_id;
			SQLSMALLINT text;
			SQLGetDiagRec(SQL_HANDLE_STMT, hstmt1, 1, state, &error_id, msg, 128, &text);
			printf("db Insert fail, sqlstate=%s, errormsg=%s\n", state, msg);
			
			SQLEndTran(SQL_HANDLE_DBC, hdbc1, SQL_ROLLBACK);
			SQLFreeHandle(SQL_HANDLE_STMT, &hstmt1);
			
			
			
		

			//system("pause");
			return retcode;
		}
		else if (retcode == SQL_SUCCESS_WITH_INFO)
		{
			SQLCHAR msg[128];
			SQLCHAR state[128];
			SQLINTEGER error_id;
			SQLSMALLINT text;
			SQLGetDiagRec(SQL_HANDLE_STMT, hstmt1, 1, state, &error_id, msg, 128, &text);
			SQLFreeHandle(SQL_HANDLE_STMT, &hstmt1);
		
			printf("warning msg=%s\n", msg);
			return retcode;
		}
		
		SQLEndTran(SQL_HANDLE_DBC, hdbc1, SQL_COMMIT);
		SQLFreeHandle(SQL_HANDLE_STMT, &hstmt1);
		//SQLFreeStmt(hstmt1,SQL_CLOSE);
		//SQLCloseCursoSr();
		/*SQLFreeHandle(SQL_HANDLE_STMT, &hstmt1);
		SQLFreeHandle(SQL_HANDLE_DBC,hdbc1);
		SQLFreeHandle(SQL_HANDLE_ENV,henv);
		SQLDisconnect(hdbc1);*/
	return 0;
}
