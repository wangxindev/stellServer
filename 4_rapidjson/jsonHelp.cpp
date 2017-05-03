#include "jsonHelp.h"
#include <Exception>
#include <mutex>
using std::exception;
std::mutex mtx_Relate;

jsonHelp::jsonHelp(const char * json, const char *logic)
{
	_json = json;
	_logic = logic;
}

bool jsonHelp::parseJson()
{
	try
	{
		InterfaceHandler handler;
		Reader reader;
		StringStream ss(_json);
		reader.Parse(ss, handler);
		_JsonNode = handler.getJsonNode();
		createSql();
		_JsonNode = NULL;
		return true;
	}
	catch (exception* e)
	{
		e->what();
		return false;
	}
}

void jsonHelp::createSql()
{
	if (_JsonNode == NULL)
		return;

	jsonNodeVecPtr nodes = _JsonNode->getChildNodes();
	if (nodes.size() > 0)
	{
		createSqlCreate(nodes[0].get(), _logic);
		insertSqlCreate(nodes[0].get(), _logic);
	}

}

bool jsonHelp::createSqlCreate(jsonNode * node, const char * table, bool isRelate /*= false*/)
{
	

	string sqlCreate = "CREATE TABLE ";
	sqlCreate.append(table);
	sqlCreate.append(" (");

	if (isRelate)
	{//处理关联字段
		sqlCreate.append(table);
		sqlCreate.append("  ");
		sqlCreate.append(toString("INTEGER"));
		sqlCreate.append(",");
	}

	jsonNodeVecPtr nodes = node->getChildNodes();
	int size = nodes.size();
	for (int i = 0; i < size; ++i)
	{
		jsonNodePtr jNode = nodes[i];
		eJsonNodeType type = jNode->getNodeType();
		if (type == eArray || type == eObject)
		{
			{
				sqlCreate.append("  ");
				sqlCreate.append(jNode->getNodeDataType());
				sqlCreate.append(",");
			}
			string table_s = table;
			table_s.append(jNode->getNodeKey());
			createSqlCreate(jNode.get(), table_s.c_str(), true);
		}
		else if (type == eKey)
		{
			sqlCreate.append(jNode->getNodeData());
		}
		else if (type == eValue)
		{
			sqlCreate.append("  ");
			sqlCreate.append(jNode->getNodeDataType());
			sqlCreate.append(",");
		}
		else
			return false;

	}
	sqlCreate.erase(sqlCreate.end() - 1);
	sqlCreate.append(" );");

	_createTableSqlVec.push_back(sqlCreate);

	return true;;
}

bool jsonHelp::updateSqlCreate(jsonNode * node, const char * table)
{
	return true;
}

bool jsonHelp::insertSqlCreate(jsonNode * node, const char * table)
{
	string sqlKey ="INSERT INTO ";
	{
		sqlKey.append(table);
		sqlKey.append(" (");
	}
	string sqlValue = " VALUES (";

	jsonNodeVecPtr nodes = node->getChildNodes();
	int size = nodes.size();
	for (int i = 0; i < size; ++i)
	{
		jsonNodePtr jNode = nodes[i];
		eJsonNodeType type = jNode->getNodeType();
		int  idx = 0;
		if (type == eArray || type == eObject)
		{
			{
				idx++;
				char buf[256] = { 0 };
				sprintf(buf, "%s_%s_%d", table, jNode->getNodeKey().c_str(), idx);
				sqlKey.append(buf);
				{
					sqlValue.append("  ");
					sqlValue.append("1");
					sqlValue.append(",");
				}
			}
			string table_s = table;
			table_s.append(node->getNodeKey());
			insertSqlCreate(jNode.get(), table_s.c_str());
		}
		else if (type == eKey)
		{
			sqlKey.append("  ");
			sqlKey.append(jNode->getNodeData());
			sqlKey.append(",");
		}
		else if (type == eValue)
		{
			sqlValue.append("  ");
			sqlValue.append(jNode->getNodeData());
			sqlValue.append(",");
		}
		else
			return false;
	}

	sqlKey.erase(sqlKey.end() - 1);
	sqlKey.append(" )");
	sqlValue.erase(sqlValue.end() - 1);
	sqlValue.append(" );");

	_insertTableSqlVec.push_back(sqlKey + sqlValue);

	return true;
}

