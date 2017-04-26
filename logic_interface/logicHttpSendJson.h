#pragma once
#ifndef __LOGIC_HTTP_SEND_JSON_H__
#define __LOGIC_HTTP_SEND_JSON_H__
#include "global.h"

/**
 @class	logicHttpSendJson

 @brief	人工智能生产的数据存储到sqlite库中以供用户调用

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
