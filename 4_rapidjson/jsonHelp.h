#pragma once
#ifndef __JSONHELP_H_
#define __JSONHELP_H_
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include <string>
#include <vector>
#include <iostream>

using namespace rapidjson;
using std::string;
using std::vector;
using std::pair;
using std::cout;
using std::endl;

class nameNode;
typedef vector<pair<nameNode, string > > jsonKey;

class nameNode {
public:
	nameNode(string _key, string _value) { key = _key; value = _value; }
	string key;
	string value;
	jsonKey jsonKeys;
};

class jsonHelp
{
public:
	jsonHelp() {}
	~jsonHelp() {}

	//Value::MemberIterator
	static nameNode* getCdom(nameNode * keys, Value::MemberIterator iter);
	static jsonKey getCjsonArray(string json, int*errn);

	static string getType(Value::MemberIterator iter);
	static bool isArray(Value::MemberIterator iter);
	static string getStringValue(Value::MemberIterator iter);
};

#endif

