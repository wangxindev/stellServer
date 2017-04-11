#if 0
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
#endif

#include <iostream>  
#include <string.h>  
#include <lua.hpp>

using namespace std;  
void main()  
{  
    //1.创建一个state  
    lua_State *L = luaL_newstate();  
       
    //2.入栈操作  
    lua_pushstring(L, "I am so cool~");   
    lua_pushnumber(L,20);  
   
    //3.取值操作  
    if( lua_isstring(L,1)){             //判断是否可以转为string  
		cout << lua_tostring(L, 1) << endl;
    }  
    if( lua_isnumber(L,2)){  
        cout<< lua_tonumber(L,2)<<endl;  
    }  
   
    //4.关闭state  
    lua_close(L);  
	getchar();
    return ;  
}

