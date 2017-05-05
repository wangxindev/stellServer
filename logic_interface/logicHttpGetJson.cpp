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
	//jsonKey jsonKeys;//��һ��jsonΪkey�����ڶ�������Ϊ�洢����
	{ //��ȡhttp�������е�json����
		json = logicBase::getPostHttpBody(req);
		//const char json_cstr[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4], \"arr\":[{\"age\":1},{\"age\":2},{\"age2\":3}] ,\"hello\" : \"world\" } ";
		const char json_cstr[] = " {\"hello\" : \"'world'\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"arr\":[{\"age\":1},{\"age\":2},{\"age2\":3}] ,\"hello\" : \"world\"}";
		json = json_cstr;

		sprintf(getLogEmptyBuf(), "httpBody:\n%s", json.c_str());
		LOGI("info", getLogCStr());
		try
		{
			jsonHelp jH(json.c_str(), logic.c_str());
			jH.parseJson();
		}
		catch (const std::exception& e)
		{
			cout << e.what() << endl;
		}

	//	jsonKeys = jsonHelp::getCjsonArray(json, &err);
	//	int a = 0;
	//	cout << a;
	}
}
