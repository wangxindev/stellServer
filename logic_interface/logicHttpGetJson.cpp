#include "logicHttpGetJson.h"

void logicHttpGetJson::logic_run(struct evhttp_request * req, void * userData)
{
	string logic;
	{//logic用来区分不同的业务，也用来建数据库存储对应的数据库名，表名
		logic = logicBase::getHttpHeadParam("logic", req);
		LOG_I_S("info", "logic:\n%s\n", logic.c_str());
	}

	string json;
	//jsonKey jsonKeys;//第一个json为key名，第二个参数为存储类型
	{ //获取http报文体中的json数据
		json = logicBase::getPostHttpBody(req);
		//const char json_cstr[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4], \"arr\":[{\"age\":1},{\"age\":2},{\"age2\":3}] ,\"hello\" : \"world\" } ";
		const char json_cstr[] = " {\"hello\" : \"'world'\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"arr\":[{\"age\":1},{\"age\":2},{\"age2\":3}] ,\"hello\" : \"world\"}";
		json = json_cstr;

		LOG_I_S("info", "httpBody:\n%s\n", json.c_str());
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
