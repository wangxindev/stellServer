#include "logicHttpGetJson.h"

void logicHttpGetJson::logic_run(struct evhttp_request * req, void * userData)
{
	string logic;
	{//logic用来区分不同的业务，也用来建数据库存储对应的数据库名，表名
		logic = logicBase::getHttpHeadParam("logic", req);
		sprintf(getLogEmptyBuf(), "logic:\n%s", logic.c_str());
		LOGI("info", getLogCStr());
	}

	string json;
	jsonKey jsonKeys;//第一个json为key名，第二个参数为存储类型
	{ //获取http报文体中的json数据
		json = logicBase::getPostHttpBody(req);
		sprintf(getLogEmptyBuf(), "httpBody:\n%s", json.c_str());
		LOGI("info", getLogCStr());
		int err;
		jsonKeys = jsonHelp::getCjsonArray(json, &err);
		int a = 0;
		cout << a;
	}
}
