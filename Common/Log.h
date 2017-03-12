#pragma once

#include "..\iostream.h"

class Log
{
private:
	static vector<string> m_Log;
public:
	enum LogType
	{
		LOG_ERROR,
		LOG_WARNING,
		LOG_DEBUG,
		LOG_NUM,
	};
	Log();
	~Log();

	static void Message(LogType type,string infomarion);
	static void OutputFile();
};

