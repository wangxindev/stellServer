#include "logicHttpGetJson.h"

void logicHttpGetJson::logic_run(struct evhttp_request * req, void * userData)
{
	string logic;
	{//logic�������ֲ�ͬ��ҵ��Ҳ���������ݿ�洢��Ӧ�����ݿ���������
		logic = logicBase::getHttpHeadParam("logic", req);
		sprintf(getLogEmptyBuf(), "logic:\n%s", logic.c_str());
		LOGI("info", getLogCStr());
	}

	string json;
	jsonKey jsonKeys;//��һ��jsonΪkey�����ڶ�������Ϊ�洢����
	{ //��ȡhttp�������е�json����
		json = logicBase::getPostHttpBody(req);
		sprintf(getLogEmptyBuf(), "httpBody:\n%s", json.c_str());
		LOGI("info", getLogCStr());
		int err;
		jsonKeys = jsonHelp::getCjsonArray(json, &err);
		int a = 0;
		cout << a;
	}
}
