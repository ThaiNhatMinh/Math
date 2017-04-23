#pragma once

#include "..\iostream.h"


class ShaderManager
{
private:
	static map<string, Shader*> m_ShaderList;

public:
	static Shader* GetLoad(string key, const char* vs, const char* fs);
	static Shader* Get(string key);
	static void Release();
};
