#include "userPermissionsMgr.h"
#include <string>
#include <cstdio>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cassert>

std::list<std::string> userPermissionsMgr::ipWhiteList;
std::list<std::string> userPermissionsMgr::ipBlacklist;
userPermissionsMgr::userPermissionsMgr()
{
	isIpBlacklistOn = true;
	isIpWhiteListOn = true;
}


userPermissionsMgr::~userPermissionsMgr()
{
}


void userPermissionsMgr::register_ipList(std::list<std::string>& ipList, const char * ip)
{
	std::list<std::string>::iterator iter;
	iter = ipList.begin();
	bool CanReg = true;
	for (; iter != ipList.end(); ++iter)
		if ((*iter).compare(ip) == 0)
			CanReg = false;

	if (CanReg)
	{
		std::string str(ip);
		ipList.push_back(str);
	}
}

void userPermissionsMgr::register_IpWhiteList(const char * ip) 
{
	register_ipList(ipWhiteList, ip);
}

void userPermissionsMgr::register_ipBlacklist(const char * ip)
{
	register_ipList(ipBlacklist, ip);
}

void userPermissionsMgr::remove_IpList(std::list<std::string> &ipList, const char * ip)
{
	std::list<std::string>::iterator iter;
	iter = ipList.begin();
	for (; iter != ipList.end(); ++iter)
		if ((*iter).compare(ip) == 0)
			ipList.remove(*iter);
}

void userPermissionsMgr::remove_IpWhiteList(const char * ip)
{
	remove_IpList(ipWhiteList, ip);
}

void userPermissionsMgr::remove_ipBlacklist(const char * ip)
{
	remove_IpList(ipBlacklist, ip);
}

bool userPermissionsMgr::checkPermissions(const char * userIP)
{
	/*
		白名单优先级高于黑名单
	*/

	//白名单
	if (isIpWhiteListOn)
	{
		for (auto iter : ipWhiteList)
			if (iter.compare(userIP) == 0)
				return true;
	}

	//黑名单
	if (isIpBlacklistOn)
	{
		for (auto iter : ipBlacklist)
			if (iter.compare(userIP) == 0)
				return false;
	}
	
	//不在黑白名单默认拥有访问权限
	return true;
}

//去掉所有空格
void trim(std::string &s)
{
    int index = 0;
    if( !s.empty())
    {
        while( (index = s.find(' ',index)) != std::string::npos)
        {
            s.erase(index,1);
        }
    }
}

//从本地加载黑白名单
void userPermissionsMgr::loadFileRegIP()
{
	//这个函数只需要执行一次
	static bool one = false;
	if (one == true)
		return;
	one = true;

	std::string line;

	std::ifstream ifsBlack;
	ifsBlack.open("黑名单.config");
	if (!ifsBlack.is_open()) { return; }

	while (getline(ifsBlack, line))
	{
		trim(line);
		if (line.size() > 0) register_ipBlacklist(line.c_str());
	}
	ifsBlack.close();

	std::ifstream ifsWhite;
	ifsWhite.open("白名单.config");
	if (!ifsWhite.is_open()) { return; }

	while (getline(ifsWhite, line))
	{
		trim(line);
		if (line.size() > 0) register_ipBlacklist(line.c_str());
	}
	ifsWhite.close();

}
