#include "iostream.h"

vector<string> Log::m_Log;

string logString[] = {
	"Error: ",
	"Warning: ",
	"Debug: " };

Log::Log()
{
}


Log::~Log()
{
}

void Log::Message(LogType type, string infomarion)
{
	string t = logString[type] + infomarion;
	m_Log.push_back(t);

	cout << t << endl;
}
