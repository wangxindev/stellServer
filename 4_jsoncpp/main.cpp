#include <fstream>
#include <cassert>
#include "json/json.h"
#include <iostream>
 
int main()
{
#if 0
    std::ifstream ifs;
    ifs.open("testjson.json");
    assert(ifs.is_open());
 
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(ifs, root, false))
    {
        return -1;
    }
 
    std::string name;
    int age;
    int size = root.size();
    for (int i=0; i<size; ++i)
    {
        name = root[i]["name"].asString();
        age = root[i]["age"].asInt();
 
        std::cout<<name<<" "<<age<<std::endl;
    }
#else
	Json::Value arrayObj;   // ��������    
	Json::Value root;
	Json::Value new_item, new_item1;
	new_item["date"] = "2011-11-11";
	new_item1["date"] = "11:11:11";
	arrayObj.append(new_item);  // ���������Ա    
	arrayObj.append(new_item1); // ���������Ա    
	root["name"] = arrayObj;
	std::string out = root.toStyledString();
	// ����޸�ʽjson�ַ���    
	Json::FastWriter writer;
	std::string strWrite = writer.write(root);
	std::cout << strWrite << std::endl;
#endif
 
	getchar();
    return 0;
}