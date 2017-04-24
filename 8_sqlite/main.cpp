#include <windows.h>
//在stdafx.h文件中将sqlite3.h头文件引入
#ifndef SQLITE3
#define SQLITE3
#include "sqlite3.h"
#include <stdio.h>
#include <iostream>
//首先在stdafx.h头文件中加入显示数据库查询结果的回调函数：
int showTableInfo(void *para, int n_column, char **column_value, char **column_name);
#endif


int main(int argc, char* argv[])
{
	char *errMsg;
	int rc;
	sqlite3 *db;
	rc = sqlite3_open("manage1.db", &db);
	if (rc == SQLITE_OK)
	{
		std::cout << "打开数据库成功！" << std::endl;
		rc = sqlite3_exec(db, "create table if not exists user(ID integer,name varchar(32))", NULL, NULL, &errMsg);
		if (rc != SQLITE_OK)
		{
			printf("创建表失败，错误码：%d,错误原因:%sn", rc, errMsg);
			std::cout << "创建表user失败!" << std::endl;
		}
		rc = sqlite3_exec(db, "insert into user values('123','测试')", NULL, NULL, &errMsg);
		if (rc != SQLITE_OK)
		{
			std::cout << "插入数据失败!" << std::endl;
		}
		rc = sqlite3_exec(db, "select * from user", showTableInfo, NULL, &errMsg);
		if (rc != SQLITE_OK)
		{
			std::cout << "查询失败!" << std::endl;
		}

	}
	return 0;
}

int showTableInfo(void *para, int n_column, char **column_value, char **column_name)
{
	int i;
	printf("记录包含%d个字段\n", n_column);
	for (i = 0; i < n_column; i++)
	{
		printf("字段名：%s  >> 字段值：%s\n", column_name[i], column_value[i]);
	}
	printf("--------------------------------------n");

	system("pause");
	return 0;

}