#pragma once
#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "../1_log/logHelp.h"
#include "../2_libevent/httpServer.h"
#include "../3_libcurl/EasyCurl.h"
#include "../6_userPermissionsMgr/userPermissionsMgr.h"
#include "../7_util/http_util.h"
#include "../7_util/mutex.h"
#include "../7_util/str_util.h"
#include "../8_sqlite/sqlite_help.h"

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "jsonHelp.h"
#include <lua.hpp>
#include "loginit.h"

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <hash_map>
#include <set>
#include <hash_set>
#include <assert.h>
#include <mutex>
#include <thread>
#include <cstdio>
#include <cstdlib>

using std::cout;
using std::endl;
using std::string;
using std::thread;
using std::mutex;
using std::pair;

using namespace rapidjson;

#endif