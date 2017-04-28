#pragma once
#ifndef __JSONHELP_H_
#define __JSONHELP_H_
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <memory>

using namespace rapidjson;
using std::string;
using std::vector;
using std::pair;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::stringstream;

class jsonNode;
typedef shared_ptr<jsonNode> jsonNodePtr;

enum eJsonNodeType { eKey, eValue, eArray, eObject };
class jsonNode {
public:
	jsonNode() {}
	~jsonNode() {}
	inline bool isKey() { return _nodeType == eKey; }
	inline bool isValue() { return _nodeType == eValue; }
	inline bool isArray() { return _nodeType == eArray; }
	inline bool isObject() { return _nodeType == eObject; }

	inline void setNodeType(eJsonNodeType type) { _nodeType = type; }
	inline eJsonNodeType& getNodeType() { return _nodeType; }

	inline void setNodeData(string data) { _data = data; }
	inline string& getNodeData() { return _data; }

	inline void setNodeDataType(string dataType) { _dataType = dataType; }
	inline string& getNodeDataType() { return _dataType; }

	void setChildNode(jsonNodePtr node)
	{
		jsonNode * jsonN = this;
		jsonNodePtr jThis(jsonN);
		node->setFatherNode(jThis);
		_childNodes.push_back(node); 
	}
	inline vector<jsonNodePtr>& getChildNodes() { return _childNodes; }

	inline void setFatherNode(jsonNodePtr node) { _fatherNode = node; }
	inline jsonNodePtr getFatherNode() { return _fatherNode; }

private:

	eJsonNodeType _nodeType;
	string _data;
	string _dataType;

	jsonNodePtr _fatherNode;
	vector<jsonNodePtr> _childNodes;
};


class InterfaceHandler : public BaseReaderHandler<UTF8<>, InterfaceHandler> {
public:
	InterfaceHandler() 
	{
		_JsonNode = std::make_shared<jsonNode>();
		_currJsonNode = _JsonNode; 
	}

    bool Null();
    bool Bool(bool b);
    bool Int(int i);
    bool Uint(unsigned i);
    bool Int64(int64_t i);
    bool Uint64(uint64_t i);
    bool Double(double d);
    bool RawNumber(const Ch* str, SizeType length, bool copy);
    bool String(const Ch* str, SizeType length, bool copy);
    bool StartObject();
    bool Key(const Ch* str, SizeType length, bool copy);
    bool EndObject(SizeType memberCount);
    bool StartArray();
    bool EndArray(SizeType elementCount);

	inline jsonNodePtr getJsonNode() { return _JsonNode; }

	template <class T>
	string toString(T data)
	{
		stringstream sstr;
		string str;
		sstr << data;
		sstr >> str;
		return str;
	}
private:
	jsonNodePtr _JsonNode;
	jsonNodePtr _currJsonNode;
};

class jsonHelp
{
public:
	jsonHelp(const char * json) { _json = json; }
	~jsonHelp() {}

	bool parseJson();
	

private:
	const char * _json;
	jsonNodePtr _JsonNode;
	vector<string> _insertTableSqlVec;
	vector<string> _createTableSqlVec;
};
#endif

