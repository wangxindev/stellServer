#pragma once
#ifndef __LOGIC_HTTP_SEND_JSON_H__
#define __LOGIC_HTTP_SEND_JSON_H__
#include "global.h"

/**
 @class	logicHttpSendJson

 @brief	�˹��������������ݴ洢��sqlite�����Թ��û�����

 @author	wangxin
 @date	2017/4/26
 */

class logicHttpSendJson :
	public logicBase
{
public:
	logicHttpSendJson(char *logicInfo) :logicBase::logicBase(logicInfo) {}
	~logicHttpSendJson() {}
	void logic_run(struct evhttp_request * req, void * userData);
};


#endif
