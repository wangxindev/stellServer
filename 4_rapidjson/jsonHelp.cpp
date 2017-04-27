#include "jsonHelp.h"

nameNode* jsonHelp::getCdom(nameNode * node, Value::MemberIterator iter)
{
	if (/*isArray(iter) || */iter->value.IsObject())
	{
		for (auto begin = iter->value.MemberBegin(); begin != iter->value.MemberEnd(); ++begin)
		{
			if (begin->name.IsString())
			{
#ifdef DEBUG
				cout << "key:" << begin->name.GetString() << endl;
#endif
				nameNode name(begin->name.GetString(),getStringValue(begin));
				string value(getType(begin));
				if (/*isArray(begin) || */iter->value.IsObject()) //这个元素是数组,迭代处理
					getCdom(&name,begin);
				pair<nameNode, string> key(name, value);
				node->jsonKeys.push_back(key);
			}
		}
	}
	return  node;
}

jsonKey jsonHelp::getCjsonArray(string json, int* errn)
{
	Document document;
	jsonKey jsonKeys;

	const char *jsonCstr = json.c_str();
	const int jsonSize = json.size() + 1;
	char *buffer = new char[jsonSize];
	memcpy(buffer, jsonCstr, jsonSize);
	if (document.ParseInsitu(buffer).HasParseError())
	{
		*errn = -1;
		delete buffer;
		return jsonKeys;
	}

	for (auto begin = document.MemberBegin(); begin != document.MemberEnd(); ++begin)
	{
		if (begin->name.IsString())
		{
			nameNode name(begin->name.GetString(), getStringValue(begin));
			string valueType(getType(begin));
			if (/*valueType.compare("Array") == 0 || */valueType.compare("Object") == 0)
			{//这个元素是数组
				getCdom(&name, begin);
			}
			pair<nameNode, string> key(name, valueType);
			jsonKeys.push_back(key);
		}
	}
	*errn = 0;
	delete buffer;
	return jsonKeys;
}

string jsonHelp::getType(Value::MemberIterator iter)
{
	static const char* kTypeNames[] =
	{ "Null", "False", "True", "Object", "Array", "String", "Number" };
	string value(kTypeNames[iter->value.GetType()]);
	return value;
}

bool jsonHelp::isArray(Value::MemberIterator iter)
{
	return getType(iter).compare("Array") == 0;
}

string jsonHelp::getStringValue(Value::MemberIterator iter)
{
	string type(getType(iter));
	char buf[1024] = { 0 };
	if (type.compare("String") == 0)
	{
		return iter->value.GetString();
	}
	else if (type.compare("Number") == 0)
	{
		if (iter->value.IsInt())
		{
			sprintf(buf, "%d", iter->value.GetInt());
			return buf;
		}
		else if (iter->value.IsDouble())
		{
			sprintf(buf, "%lf", iter->value.GetInt());
			return buf;
		}
	}
	else if (type.compare("False") == 0)
		return "false";
	else if (type.compare("True") == 0)
		return "true";
	else if (type.compare("Null") == 0)
		return "null";
	else if (type.compare("Object") == 0)
		return "object";
	else if (type.compare("Array") == 0)
		return "array";
	else
		return "";
}
