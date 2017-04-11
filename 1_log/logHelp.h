#ifndef __LOGHELP_H_
#define __LOGHELP_H_

#include <string>
#include <vector>

enum ENUM_LOG_LEVEL2
{
	TRACE_E = 0,
	DEBUG_E,
	INFO_E,
	WARN_E,
	ERROR_E,
	ALARM_E,
	FATAL_E,
};
const std::vector<std::string> log_level_str_list = { "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "ALARM", "FATAL" };

typedef int LoggerId;

typedef struct log_config
{
	bool display = true;//是否显示在屏幕上
	ENUM_LOG_LEVEL2 lever = DEBUG_E;//log等级
	bool fileLine = true;//是否显示文件行数
	std::string path = "./";//文件输出路径
	bool outFile = true;//是否输出到文件
	bool monthDir = false;//是否开启月份文件夹
	int limitSize = 1;
	std::string name = "main";//log名

}log_config;

class logHelp
{
private:
	logHelp();
public:
	~logHelp();
	void createLog(log_config _config);
	void starLog(bool _on);
	void logOut(ENUM_LOG_LEVEL2 _level, const char* _name, const char*_logData, const char * _file = __FILE__, const int _line = __LINE__);

	static logHelp* getRef();
private:
	static logHelp * instance_;
	std::vector<std::pair<std::string, LoggerId> > logIdList_;
};

#define LOGOUT(level, name, logData) \
logHelp::getRef()->logOut(level,name,logData,__FILE__,__LINE__);


#define LOGT( _logName,_logData ) LOGOUT(TRACE_E,_logName,_logData)
#define LOGD( _logName,_logData ) LOGOUT(DEBUG_E,_logName,_logData)
#define LOGI( _logName,_logData ) LOGOUT(INFO_E,_logName,_logData)
#define LOGW( _logName,_logData ) LOGOUT(WARN_E,_logName,_logData)
#define LOGE( _logName,_logData ) LOGOUT(ERROR_E,_logName,_logData)
#define LOGA( _logName,_logData ) LOGOUT(ALARM_E,_logName,_logData)
#define LOGF( _logName,_logData ) LOGOUT(FATAL_E,_logName,_logData)

#endif