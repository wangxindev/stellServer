#pragma once
#ifndef __LOGIC_HTTP_GET_JSON_H__
#define __LOGIC_HTTP_GET_JSON_H__

#include "global.h"

/**
 @class	logicHttpGetJson

 @brief	获取人工智能所需的数据存储到sqlite库中以供调用

 @author	wangxin
 @date	2017/4/26
 */

class logicHttpGetJson :
	public logicBase
{
public:
	logicHttpGetJson(char *logicInfo) :logicBase::logicBase(logicInfo) {}
	~logicHttpGetJson() {}
	void logic_run(struct evhttp_request * req, void * userData);
};

#endif

