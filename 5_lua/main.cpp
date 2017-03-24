#include <iostream>  
#include <string.h>  
#include <lua.hpp>


#ifdef DEBUG
#pragma comment(lib, "lua_mtd.lib")
#else
#pragma comment(lib, "lua_mt.lib")
#endif

using namespace std;  
void main()  
{  
    //1.����һ��state  
    lua_State *L = luaL_newstate();  
       
    //2.��ջ����  
    lua_pushstring(L, "I am so cool~");   
    lua_pushnumber(L,20);  
   
    //3.ȡֵ����  
    if( lua_isstring(L,1)){             //�ж��Ƿ����תΪstring  
		cout << lua_tostring(L, 1) << endl;
    }  
    if( lua_isnumber(L,2)){  
        cout<< lua_tonumber(L,2)<<endl;  
    }  
   
    //4.�ر�state  
    lua_close(L);  
	getchar();
    return ;  
}

