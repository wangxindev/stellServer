#pragma once
#ifndef __USER_PERMISSIONS_MGR_H_
#define __USER_PERMISSIONS_MGR_H_
#include <list>
#include <string>
using std::list;
using std::string;
class userPermissionsMgr
{
public:
	userPermissionsMgr();
	~userPermissionsMgr();
	void loadFileRegIP();

	void OpenIpWhiteList(bool b) { isIpWhiteListOn = b; }//开启或关闭白名单
	void OpenIpBlacklist(bool b) { isIpBlacklistOn = b; }//开启或关闭黑名单
	void register_IpWhiteList(const char * ip);//注册黑名单ip
	void register_ipBlacklist(const char * ip);//注册白名单ip
	void remove_IpWhiteList(const char * ip);//移除黑名单ip
	void remove_ipBlacklist(const char * ip);//移除白名单ip

	bool checkPermissions(const char* userIP);//检查用户是否拥有权限访问

private://权限管理
	bool isOpenIpWhiteList() { return isIpWhiteListOn; }//检查是否开启ip白名单
	bool isOpenIpBlacklist() { return isIpBlacklistOn; }//检查是否开启ip黑名单

	void register_ipList(list<string > ipList, const char *ip);
	void remove_IpList(list<string > ipList, const char * ip);

	list<string> ipWhiteList;//ip白名单
	list<string> ipBlacklist;//ip黑名单

	bool isIpWhiteListOn;//白名单是否开启
	bool isIpBlacklistOn;//黑名单是否开启
};

#endif

