#pragma once
#ifndef __LOGINIT_H__
#define __LOGINIT_H__
#include <string>
#include <mutex>
#include "../1_log/logHelp.h"

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

#endif