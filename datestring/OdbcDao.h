#pragma once
#define ROW_ARRAY_SIZE 1000
class ODBCDao
{
public:
	const char* szDSN ;
	const char* szUID ;//log name
	const char* szAuthStr ;//passward
	SQLHENV  henv ;//定义环境句柄
	SQLHDBC  hdbc1 ;//定义数据库连接句柄    
	SQLHSTMT  hstmt1 ;//定义语句句柄
	RETCODE retcode;//错误返回码
	ODBCDao(void);
	~ODBCDao(void);
	int insertInto(RowBean *bean);
	int getConn(void);
	int insertBulkInto(RowBean *bean);
	int insertbulkParamInto(RowBean *bean);
};

