#pragma once
#ifndef __USER_PERMISSIONS_MGR_H_
#define __USER_PERMISSIONS_MGR_H_
#include <list>
class userPermissionsMgr
{
public:
	userPermissionsMgr();
	~userPermissionsMgr();
	void loadFileRegIP();

	void OpenIpWhiteList(bool b) { isIpWhiteListOn = b; }//������رհ�����
	void OpenIpBlacklist(bool b) { isIpBlacklistOn = b; }//������رպ�����
	void register_IpWhiteList(const char * ip);//ע�������ip
	void register_ipBlacklist(const char * ip);//ע�������ip
	void remove_IpWhiteList(const char * ip);//�Ƴ�������ip
	void remove_ipBlacklist(const char * ip);//�Ƴ�������ip

	bool checkPermissions(const char* userIP);//����û��Ƿ�ӵ��Ȩ�޷���

private://Ȩ�޹���
	bool isOpenIpWhiteList() { return isIpWhiteListOn; }//����Ƿ���ip������
	bool isOpenIpBlacklist() { return isIpBlacklistOn; }//����Ƿ���ip������

	void register_ipList(std::list<std::string> &ipList, const char *ip);
	void remove_IpList(std::list<std::string> &ipList, const char * ip);

	static std::list<std::string> ipWhiteList;//ip������
	static std::list<std::string> ipBlacklist;//ip������

	bool isIpWhiteListOn;//�������Ƿ���
	bool isIpBlacklistOn;//�������Ƿ���
};

#endif

