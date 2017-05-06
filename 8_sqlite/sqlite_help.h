#pragma once
#ifndef _SQLITE_HELP_H_
#define _SQLITE_HELP_H_
//#define		             -1   /* open database err */
//#define SQLITE_OK           0   /* Successful result */
//#define SQLITE_ERROR        1   /* SQL error or missing database */
//#define SQLITE_INTERNAL     2   /* Internal logic error in SQLite */
//#define SQLITE_PERM         3   /* Access permission denied */
//#define SQLITE_ABORT        4   /* Callback routine requested an abort */
//#define SQLITE_BUSY         5   /* The database file is locked */
//#define SQLITE_LOCKED       6   /* A table in the database is locked */
//#define SQLITE_NOMEM        7   /* A malloc() failed */
//#define SQLITE_READONLY     8   /* Attempt to write a readonly database */
//#define SQLITE_INTERRUPT    9   /* Operation terminated by sqlite3_interrupt()*/
//#define SQLITE_IOERR       10   /* Some kind of disk I/O error occurred */
//#define SQLITE_CORRUPT     11   /* The database disk image is malformed */
//#define SQLITE_NOTFOUND    12   /* Unknown opcode in sqlite3_file_control() */
//#define SQLITE_FULL        13   /* Insertion failed because database is full */
//#define SQLITE_CANTOPEN    14   /* Unable to open the database file */
//#define SQLITE_PROTOCOL    15   /* Database lock protocol error */
//#define SQLITE_EMPTY       16   /* Database is empty */
//#define SQLITE_SCHEMA      17   /* The database schema changed */
//#define SQLITE_TOOBIG      18   /* String or BLOB exceeds size limit */
//#define SQLITE_CONSTRAINT  19   /* Abort due to constraint violation */
//#define SQLITE_MISMATCH    20   /* Data type mismatch */
//#define SQLITE_MISUSE      21   /* Library used incorrectly */
//#define SQLITE_NOLFS       22   /* Uses OS features not supported on host */
//#define SQLITE_AUTH        23   /* Authorization denied */
//#define SQLITE_FORMAT      24   /* Auxiliary database format error */
//#define SQLITE_RANGE       25   /* 2nd parameter to sqlite3_bind out of range */
//#define SQLITE_NOTADB      26   /* File opened that is not a database file */
//#define SQLITE_NOTICE      27   /* Notifications from sqlite3_log() */
//#define SQLITE_WARNING     28   /* Warnings from sqlite3_log() */
//#define SQLITE_ROW         100  /* sqlite3_step() has another row ready */
//#define SQLITE_DONE        101  /* sqlite3_step() has finished executing *

//这个类适用于执行一次性语句,因为每次执行会打开和关闭数据库
class sqlite_help_one
{
public:
	sqlite_help_one() {};
	~sqlite_help_one() {};

	/**
	 @fn	static int createTable(char * dataBase, char * sql, char **errmsg);
	
	 @brief	Creates a table.
	
	 @author	wangxin
	 @date	2017/4/24
	
	 @param [in,out]	dataBase	数据库名称
	 @param [in,out]	sql			SQL语句.
	 @param [in,out]	errmsg  	错误信息.
	
	 @return	错误码，0为成功-1为打开数据库失败，其他看头文件.
	 */

	static int createTable(char * dataBase, char * sql, char **errmsg);

	/**
	 @fn	static int insertTable(char * dataBase, char * sql, char **errmsg);
	
	 @brief	Inserts a table.
	
	 @author	wangxin
	 @date	2017/4/24
	
	@param [in,out]	dataBase	数据库名称
	@param [in,out]	sql			SQL语句.
	@param [in,out]	errmsg  	错误信息.

	@return	错误码，0为成功-1为打开数据库失败，其他看头文件..
	 */

	static int insertTable(char * dataBase, char * sql, char **errmsg);

	/**
	 @fn	static int selectTable(char * dataBase, char * sql, int(*callback)(void*, int, char**, char**), char **errmsg);
	
	 @brief	Select table.
	
	 @author	wangxin
	 @date	2017/4/24
	
	 @param [in,out]	dataBase	数据库名称
	 @param [in,out]	sql			SQL语句.
	 @param [in,out]	callback	查询的回调函数,通过这个函数拿到表的信息
	 @param [in,out]	errmsg  	错误信息.
	
	 @return	错误码，0为成功-1为打开数据库失败，其他看头文件..
	 */

	static int selectTable(char * dataBase, char * sql, int(*callback)(void*, int, char**, char**), char **errmsg , void * param);
	static int runSql(
		const char * db,                                  /* An open database */
		const char *sql,                           /* SQL to be evaluated */
		int(*callback)(void*, int, char**, char**),  /* Callback function */
		void *,                                    /* 1st argument to callback */
		char **errmsg                              /* Error msg written here */
	);
	static int printErrInfo(int rc, char **errinfo);
};

#endif

