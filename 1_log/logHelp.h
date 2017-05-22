#ifndef __LOGHELP_H_
#define __LOGHELP_H_

#include <string>
#include <vector>
#include <mutex>
#include <string.h>
#include <stdio.h>

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

static char *logB = new char[10240];
static std::mutex mtx_logBuf;
class logBuf {
public:
	//这两个函数必须一起使用过，才能下次继续使用，加了锁保护
	static char * getLogInitBuf()
	{
		mtx_logBuf.lock();
		memset(logB, '0', 10240);
		return logB;
	}
	static std::string getLogBuf()
	{
		std::string logString = logB;
		mtx_logBuf.unlock();
		return logString;
	}

	static void logConfigInit()
	{
		log_config logErr;
		logErr.path = "./log/Err";
		logErr.name = "err";
		logErr.lever = ERROR_E;
		log_config logWarn;
		logWarn.path = "./log/Warn";
		logWarn.name = "warn";
		logWarn.lever = WARN_E;
		log_config logInfo;
		logInfo.path = "./log/Info";
		logInfo.name = "info";
		logInfo.lever = INFO_E;
		logHelp::getRef()->createLog(logErr);
		logHelp::getRef()->createLog(logWarn);
		logHelp::getRef()->createLog(logInfo);
		LOGI("main", "initd\n");
		LOGE("err", "initd\n");
		LOGW("warn", "initd\n");
		LOGI("info", "initd\n");
	}
};

#define getLogEmptyBuf() logBuf::getLogInitBuf()
#define getLogCStr() logBuf::getLogBuf().c_str() 
#define logInit() logBuf::logConfigInit()

#define LOGTS( _logName,_logData, iparam ) \
	sprintf(getLogEmptyBuf(),_logData, iparam); \
	 LOGOUT(TRACE_E,_logName,getLogCStr())
#define LOGDS( _logName,_logData, iparam ) \
	sprintf(getLogEmptyBuf(),_logData, iparam); \
	LOGOUT(DEBUG_E,_logName,getLogCStr())
#define LOGIS( _logName,_logData, iparam ) \
	sprintf(getLogEmptyBuf(),_logData, iparam); \
	LOGOUT(INFO_E,_logName,getLogCStr())
#define LOGWS( _logName,_logData, iparam ) \
	sprintf(getLogEmptyBuf(),_logData, iparam); \
	LOGOUT(WARN_E,_logName,getLogCStr())
#define LOGES( _logName,_logData, iparam ) \
	sprintf(getLogEmptyBuf(),_logData, iparam); \
	LOGOUT(ERROR_E,_logName,getLogCStr())
#define LOGAS( _logName,_logData, iparam ) \
	sprintf(getLogEmptyBuf(),_logData, iparam); \
	LOGOUT(ALARM_E,_logName,getLogCStr())
#define LOGFS( _logName,_logData, iparam ) \
	sprintf(getLogEmptyBuf(),_logData, iparam); \
	LOGOUT(FATAL_E,_logName,getLogCStr())

#endif