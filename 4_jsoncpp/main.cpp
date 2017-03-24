
#ifdef DEBUG
#pragma comment(lib, "json_mtd.lib")
#else
#pragma comment(lib, "json_mt.lib")
#endif
 
#include <fstream>
#include <cassert>
#include "json/json.h"
#include <iostream>
 
int main()
{
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
 
	getchar();
    return 0;
}