#include <windows.h>
//��stdafx.h�ļ��н�sqlite3.hͷ�ļ�����
#ifndef SQLITE3
#define SQLITE3
#include "sqlite3.h"
#include <stdio.h>
#include <iostream>
//������stdafx.hͷ�ļ��м�����ʾ���ݿ��ѯ����Ļص�������
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
		std::cout << "�����ݿ�ɹ���" << std::endl;
		rc = sqlite3_exec(db, "create table if not exists user(ID integer,name varchar(32))", NULL, NULL, &errMsg);
		if (rc != SQLITE_OK)
		{
			printf("������ʧ�ܣ������룺%d,����ԭ��:%sn", rc, errMsg);
			std::cout << "������userʧ��!" << std::endl;
		}
		rc = sqlite3_exec(db, "insert into user values('123','����')", NULL, NULL, &errMsg);
		if (rc != SQLITE_OK)
		{
			std::cout << "��������ʧ��!" << std::endl;
		}
		rc = sqlite3_exec(db, "select * from user", showTableInfo, NULL, &errMsg);
		if (rc != SQLITE_OK)
		{
			std::cout << "��ѯʧ��!" << std::endl;
		}

	}
	return 0;
}

int showTableInfo(void *para, int n_column, char **column_value, char **column_name)
{
	int i;
	printf("��¼����%d���ֶ�\n", n_column);
	for (i = 0; i < n_column; i++)
	{
		printf("�ֶ�����%s  >> �ֶ�ֵ��%s\n", column_name[i], column_value[i]);
	}
	printf("--------------------------------------n");

	system("pause");
	return 0;

}