
#include "logHelp.h"
#include "log4z.h"
static std::mutex mtx_log_new_instance_1;
using namespace zsummer::log4z;

logHelp * logHelp::instance_  = NULL;

logHelp::logHelp()
{
	ILog4zManager::getRef().setLoggerDisplay(LOG4Z_MAIN_LOGGER_ID, true);
    ILog4zManager::getRef().setLoggerLevel(LOG4Z_MAIN_LOGGER_ID, LOG_LEVEL_DEBUG);
    ILog4zManager::getRef().setLoggerFileLine(LOG4Z_MAIN_LOGGER_ID, false);
    ILog4zManager::getRef().setLoggerName(LOG4Z_MAIN_LOGGER_ID, "main");
    ILog4zManager::getRef().setLoggerPath(LOG4Z_MAIN_LOGGER_ID, "./log/Main");
    ILog4zManager::getRef().setLoggerOutFile(LOG4Z_MAIN_LOGGER_ID, true);
	ILog4zManager::getRef().start();
}

logHelp::~logHelp()
{
}

void logHelp::createLog(log_config _config)
{
	LoggerId id = ILog4zManager::getRef().createLogger(_config.name.c_str());
	ILog4zManager::getRef().setLoggerDisplay(id, _config.display);
    ILog4zManager::getRef().setLoggerLevel(id, _config.lever);
    ILog4zManager::getRef().setLoggerFileLine(id, _config.fileLine);
    ILog4zManager::getRef().setLoggerName(id, _config.name.c_str());
    ILog4zManager::getRef().setLoggerPath(id, _config.path.c_str());
	ILog4zManager::getRef().setLoggerOutFile(id, _config.outFile);
	ILog4zManager::getRef().setLoggerMonthdir(id, _config.monthDir);
	ILog4zManager::getRef().setLoggerLimitsize(id, _config.limitSize);
	std::pair<std::string, LoggerId> pair(_config.name.c_str(), id);
	logIdList_.push_back(pair);
}

void logHelp::starLog(bool _on)
{
	if (_on)
		ILog4zManager::getRef().start();
	else
		ILog4zManager::getRef().stop();
}

void logHelp::logOut(ENUM_LOG_LEVEL2 _level, const char * _name, const char * _logData, const char * _file, const int _line)
{
	LoggerId id = -1;
	std::vector < std::pair < std::string,LoggerId> >::iterator iter = logIdList_.begin();
	for (;iter != logIdList_.end(); ++iter)
	{
		std::pair<std::string, LoggerId> pair = (std::pair<std::string, LoggerId>)(*iter);
		if (pair.first.compare(_name) == 0)
			id = pair.second;
	}
	if (id == -1)
	{
		log_config con;
		con.name = _name;
		con.path = "./log/Main";
		createLog(con);
	}
	
	switch (_level)
	{
	case TRACE_E:
		LOG_STREAM(id, LOG_LEVEL_TRACE, _file, _line, _logData);
		break;
	case DEBUG_E:
		LOG_STREAM(id, LOG_LEVEL_DEBUG, _file, _line, _logData);
		break;
	case INFO_E:
		LOG_STREAM(id, LOG_LEVEL_INFO, _file, _line, _logData);
		break;
	case WARN_E:
		LOG_STREAM(id, LOG_LEVEL_WARN, _file, _line, _logData);
		break;
	case ERROR_E:
		LOG_STREAM(id, LOG_LEVEL_ERROR, _file, _line, _logData);
		break;
	case ALARM_E:
		LOG_STREAM(id, LOG_LEVEL_ALARM, _file, _line, _logData);
		break;
	case FATAL_E:
		LOG_STREAM(id, LOG_LEVEL_FATAL, _file, _line, _logData);
		break;
	default:
		break;
	}
}

logHelp * logHelp::getRef()
{
	if (!instance_)
	{
		mtx_log_new_instance_1.lock();
		if(!instance_)
			instance_ = new logHelp;
		mtx_log_new_instance_1.unlock();
	}
	return instance_;
}
