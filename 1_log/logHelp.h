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
	bool display = true;//�Ƿ���ʾ����Ļ��
	ENUM_LOG_LEVEL2 lever = DEBUG_E;//log�ȼ�
	bool fileLine = true;//�Ƿ���ʾ�ļ�����
	std::string path = "./";//�ļ����·��
	bool outFile = true;//�Ƿ�������ļ�
	bool monthDir = false;//�Ƿ����·��ļ���
	int limitSize = 1;
	std::string name = "main";//log��

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
	logHelp::getRef()->logOut(level,name,logData,__FILE__,__LINE__)


#define LOG_T( _logName,_logData ) LOGOUT(TRACE_E,_logName,_logData)
#define LOG_D( _logName,_logData ) LOGOUT(DEBUG_E,_logName,_logData)
#define LOG_I( _logName,_logData ) LOGOUT(INFO_E,_logName,_logData)
#define LOG_W( _logName,_logData ) LOGOUT(WARN_E,_logName,_logData)
#define LOG_E( _logName,_logData ) LOGOUT(ERROR_E,_logName,_logData)
#define LOG_A( _logName,_logData ) LOGOUT(ALARM_E,_logName,_logData)
#define LOG_F( _logName,_logData ) LOGOUT(FATAL_E,_logName,_logData)

static char *logB = new char[10240];
static std::mutex mtx_logBuf;
class logBuf {
public:
	//��������������һ��ʹ�ù��������´μ���ʹ�ã�����������
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
		LOG_I("main", "initd\n");
		LOG_E("err", "initd\n");
		LOG_W("warn", "initd\n");
		LOG_I("info", "initd\n");
	}
};

#define getLogEmptyBuf() logBuf::getLogInitBuf()
#define getLogCStr() logBuf::getLogBuf().c_str() 
#define logInit() logBuf::logConfigInit()


#define LOG_S( _logName,_logData, iparam ,log_lerver) \
	sprintf(getLogEmptyBuf(),_logData, iparam), \
	 LOGOUT(log_lerver,_logName,getLogCStr())\

#define LOG_T_S( _logName,_logData, iparam ) LOG_S(_logName, _logData, iparam, TRACE_E)
#define LOG_D_S( _logName,_logData, iparam ) LOG_S(_logName, _logData, iparam, DEBUG_E)
#define LOG_I_S( _logName,_logData, iparam ) LOG_S(_logName, _logData, iparam, INFO_E)
#define LOG_W_S( _logName,_logData, iparam ) LOG_S(_logName, _logData, iparam, WARN_E)
#define LOG_E_S( _logName,_logData, iparam ) LOG_S(_logName, _logData, iparam, ERROR_E)
#define LOG_A_S( _logName,_logData, iparam ) LOG_S(_logName, _logData, iparam, ALARM_E)
#define LOG_F_S( _logName,_logData, iparam ) LOG_S(_logName, _logData, iparam, FATAL_E)

#endif