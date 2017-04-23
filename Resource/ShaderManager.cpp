#include "..\iostream.h"
map<string, Shader*> ShaderManager::m_ShaderList;

Shader * ShaderManager::GetLoad(string key, const char * vs, const char* fs)
{
	map<string, Shader*>::iterator pos = m_ShaderList.find(key);
	if (pos != m_ShaderList.end()) return pos->second;

	Shader* p = new Shader(vs, fs);
	m_ShaderList.insert({ key, p });
	return p;
}

Shader * ShaderManager::Get(string key)
{
	return m_ShaderList[key];
}

void ShaderManager::Release()
{
	map<string, Shader*>::iterator pos = m_ShaderList.begin();
	while (pos != m_ShaderList.end())
	{
		delete pos->second;
		pos->second = NULL;
		pos++;
	}
}