#include "jsonHelp.h"


bool InterfaceHandler::Null()
{
	jsonNodePtr node(new jsonNode);
	node->setNodeType(eValue);
	node->setNodeData("null");
	node->setNodeDataType("null");
	_currJsonNode->setChildNode(node);
	return true;
}

bool InterfaceHandler::Bool(bool b)
{
	jsonNodePtr node(new jsonNode);
	node->setNodeType(eValue);
	node->setNodeData(toString(b));
	node->setNodeDataType("bool");
	_currJsonNode->setChildNode(node);
	return true;
}

bool InterfaceHandler::Int(int i)
{
	jsonNodePtr node(new jsonNode);
	node->setNodeType(eValue);
	node->setNodeData(toString(i));
	node->setNodeDataType("int");
	_currJsonNode->setChildNode(node);
	return true;
}

bool InterfaceHandler::Uint(unsigned i)
{
	jsonNodePtr node(new jsonNode);
	node->setNodeType(eValue);
	node->setNodeData(toString(i));
	node->setNodeDataType("uint");
	_currJsonNode->setChildNode(node);
	return true;
}

bool InterfaceHandler::Int64(int64_t i)
{
	jsonNodePtr node(new jsonNode);
	node->setNodeType(eValue);
	node->setNodeData(toString(i));
	node->setNodeDataType("int64");
	_currJsonNode->setChildNode(node);
	return true;
}

bool InterfaceHandler::Uint64(uint64_t i)
{
	jsonNodePtr node(new jsonNode);
	node->setNodeType(eValue);
	node->setNodeData(toString(i));
	node->setNodeDataType("uint64");
	_currJsonNode->setChildNode(node);
	return true;
}

bool InterfaceHandler::Double(double d)
{
	jsonNodePtr node(new jsonNode);
	node->setNodeType(eValue);
	node->setNodeData(toString(d));
	node->setNodeDataType("double");
	_currJsonNode->setChildNode(node);
	return true;
}

bool InterfaceHandler::RawNumber(const Ch* str, SizeType length, bool copy)
{
	return true;
}

bool InterfaceHandler::String(const Ch* str, SizeType length, bool copy)
{
	jsonNodePtr node(new jsonNode);
	node->setNodeType(eValue);
	node->setNodeData(str);
	node->setNodeDataType("string");
	_currJsonNode->setChildNode(node);
	return true;
}

bool InterfaceHandler::StartObject()
{
	jsonNodePtr node(new jsonNode);
	node->setNodeType(eObject);
	_currJsonNode->setChildNode(node);
	vector<jsonNodePtr> nodes = _currJsonNode->getChildNodes();
	_currJsonNode = node;

	return true;
}

bool InterfaceHandler::Key(const Ch* str, SizeType length, bool copy)
{
	jsonNodePtr node(new jsonNode);
	node->setNodeType(eKey);
	node->setNodeData(str);
	node->setNodeDataType("string");
	_currJsonNode->setChildNode(node);
	return true;
}

bool InterfaceHandler::EndObject(SizeType memberCount)
{
	_currJsonNode = _currJsonNode->getFatherNode();
	return true;
}

bool InterfaceHandler::StartArray()
{
	jsonNodePtr node(new jsonNode);
	node->setNodeType(eArray);
	_currJsonNode->setChildNode(node);
	vector<jsonNodePtr> nodes = _currJsonNode->getChildNodes();
	_currJsonNode = node;
	return true;
}

bool InterfaceHandler::EndArray(SizeType elementCount)
{
	_currJsonNode = _currJsonNode->getFatherNode();
	return true;
}

bool jsonHelp::parseJson()
{
	InterfaceHandler handler;
	Reader reader;
	StringStream ss(_json);
	reader.Parse(ss, handler);
	_JsonNode = handler.getJsonNode();
	return true;
}

