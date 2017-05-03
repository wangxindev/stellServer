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
#include <mutex>

using namespace rapidjson;
using std::string;
using std::vector;
using std::pair;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::weak_ptr;
using std::stringstream;

template <class T>
string toString(T data)
{
	stringstream sstr;
	string str;
	sstr << data;
	sstr >> str;
	return str;
}

class jsonNode;
typedef shared_ptr<jsonNode> jsonNodePtr;
typedef weak_ptr<jsonNode> jsonNode_WeakPtr;

typedef vector<jsonNodePtr> jsonNodeVecPtr;

enum eJsonNodeType { eKey, eValue, eArray, eObject };
class jsonNode {
public:
	jsonNode() { _childNodes = vector<jsonNodePtr>(); }
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

	inline void setNodeKey(string nodeKey) { _key = nodeKey; }
	inline string& getNodeKey() { return _key; }

	void setChildNode(jsonNode_WeakPtr node)
	{
		if (jsonNodePtr ptr = node.lock())
		{
			ptr->setFatherNode(this);
			_childNodes.push_back(ptr);
		}
		else
			cout << "setChildNode error" << endl;
	}
	
	inline jsonNodeVecPtr& getChildNodes() { return _childNodes; }

	inline void setFatherNode(jsonNode * node) { _fatherNode = node; }
	inline jsonNode* getFatherNode() { return _fatherNode; }

private:

	eJsonNodeType _nodeType;
	string _data;
	string _dataType;

	string _key;//只有类型为object和array这个字段才生效

	jsonNode* _fatherNode;
	jsonNodeVecPtr _childNodes;
};


class InterfaceHandler : public BaseReaderHandler<UTF8<>, InterfaceHandler> {
public:
	InterfaceHandler() 
	{
		_currJsonNode = new jsonNode();
		_currJsonNode->setNodeData("root");
		_currJsonNode->setNodeType(eObject);
		_currJsonNode->setNodeDataType("null");
		_currJsonNode->setNodeKey("root");
		//_currJsonNode->setFatherNode(nullptr);
	}
	~InterfaceHandler()
	{
		delete _currJsonNode;
	}

	bool Null() { setChildNode(eValue, "NULL", "NULL"); return true; }
	bool Bool(bool b) { setChildNode(eValue, b, "INTEGER"); return true; }
	bool Int(int i) { setChildNode(eValue, i, "INTEGER"); return true; }
	bool Uint(unsigned i) { setChildNode(eValue, i, "INTEGER"); return true; }
	bool Int64(int64_t i) { setChildNode(eValue, i, "INTEGER"); return true; }
	bool Uint64(uint64_t i) { setChildNode(eValue, i, "INTEGER"); return true; }
	bool Double(double d) { setChildNode(eValue, d, "REAL"); return true; }
	bool RawNumber(const Ch* str, SizeType length, bool copy) { return true; }
	bool String(const Ch* str, SizeType length, bool copy) { setChildNode(eValue, str, "TEXT"); return true; }
	bool StartObject() { setAssembledNode(eObject); return true; }
	bool Key(const Ch* str, SizeType length, bool copy) { setChildNode(eKey, str, "TEXT"); return true; }
	bool EndObject(SizeType memberCount) { _currJsonNode = _currJsonNode->getFatherNode(); return true; }
	bool StartArray() { setAssembledNode(eArray); return true; }
	bool EndArray(SizeType elementCount) { _currJsonNode = _currJsonNode->getFatherNode(); return true; }

	jsonNode* getJsonNode() { return _currJsonNode; }

	

	template <class data>
	jsonNode* setChildNode(eJsonNodeType nodeType, data d, string dataType, string key = "null")
	{
		jsonNodePtr node = std::make_shared<jsonNode>();
		node->setNodeType(nodeType);
		node->setNodeData(toString(d));
		node->setNodeDataType(dataType);
		node->setNodeKey(key);
		_currJsonNode->setChildNode(node);
		return node.get();
	}

	void setAssembledNode(eJsonNodeType nodeType)
	{
		jsonNodeVecPtr nodes = _currJsonNode->getChildNodes();
		string key = "object";
		{
			if (nodes.size() > 0)
				key = nodes[nodes.size() - 1]->getNodeData();
		}
		
		jsonNode* node = setChildNode(nodeType, key, "INTEGER", key);
		_currJsonNode = node;
	}
private:
	jsonNode* _currJsonNode;
};

class jsonHelp
{
public:
	jsonHelp(const char * json, const char *logic);
	~jsonHelp() {}

	bool parseJson();
	void createSql();

	bool createSqlCreate(jsonNode * node, const char * table, bool isRelate = false);
	bool updateSqlCreate(jsonNode * node, const char * table);
	bool insertSqlCreate(jsonNode * node, const char * table);
	

private:
	const char * _json;
	const char * _logic;
	jsonNode* _JsonNode;
	vector<string> _insertTableSqlVec;
	vector<string> _createTableSqlVec;
};
#endif

