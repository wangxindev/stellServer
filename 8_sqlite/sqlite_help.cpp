#include "sqlite_help.h"
#include <sqlite3.h>
#include <iostream>
#include <cstdio>
#include <mutex>

class mutexHelp
{
public:
	mutexHelp() { mtx.lock(); }
	~mutexHelp() { mtx.unlock(); }
	std::mutex mtx;
};

int sqlite_help_one::createTable(char * dataBase, char * sql, char **errmsg)
{
	mutexHelp mtx;
	sqlite3 *db;
	int rc = runSql(dataBase, sql, NULL, NULL, errmsg);
	printErrInfo(rc, errmsg);
	return rc;
}

int sqlite_help_one::insertTable(char * dataBase, char * sql, char **errmsg)
{
	mutexHelp mtx;
	sqlite3 *db;
	int rc = runSql(dataBase, sql, NULL, NULL, errmsg);
	printErrInfo(rc, errmsg);
	return rc;
}

int sqlite_help_one::selectTable(char * dataBase, char * sql, int(*callback)(void*, int, char**, char**), char **errmsg, void * param)
{
	mutexHelp mtx;
	sqlite3 *db;
	int rc = runSql(dataBase, sql, callback, param, errmsg);
	printErrInfo(rc, errmsg);
	return rc;
}

int sqlite_help_one::runSql(const char *database, const char *sql, int(*callback)(void*, int, char**, char**), void * v, char **errmsg )
{
	mutexHelp mtx;
	sqlite3 *db;
	int rc = sqlite3_open(database, &db);
	if (rc == SQLITE_OK)
	{
		std::cout << "open db ok!" << std::endl;
		rc = sqlite3_exec(db, sql, callback, v, errmsg);
	}
	else
		rc = -1;
	sqlite3_close(db);
	return rc;
}

int sqlite_help_one::printErrInfo(int rc, char **errinfo)
{
	mutexHelp mtx;
	if (rc > -1)
	{
		if (rc == SQLITE_OK)
		{
			if(errinfo == NULL)
				printf("db error number:%d\n", rc);
			else
				printf("db error number:%d,error string:%s\n", rc, *errinfo);
		}
	}
	return rc;
}
