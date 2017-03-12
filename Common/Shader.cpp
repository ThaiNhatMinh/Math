#include "..\iostream.h"


Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile)
{
	
	Load(vertexShaderFile, fragmentShaderFile);

}


Shader::~Shader()
{
	glDeleteProgram(m_iProgramID);

}

bool Shader::Load(const char * vertexShaderFile, const char * fragmentShaderFile)
{
	char sourceVertex[10000];
	char sourceFragment[10000];
	GLint check;
	char infoLog[512];
	GLuint vertexShader, fragmentShader;


	FILE* pvFile = fopen(vertexShaderFile, "rt");
	if (!pvFile)
	{
		Log::Message(Log::LOG_ERROR, "Can't open File: " + string(vertexShaderFile));
		exit(0);
	}
	if (fseek(pvFile, 0L, SEEK_END) == 0)
	{
		long bufsize = ftell(pvFile);
		fseek(pvFile, 0L, SEEK_SET);

		size_t newLen = fread(sourceVertex, sizeof(char), bufsize, pvFile);
		sourceVertex[newLen] = '\0'; /* Just to be safe. */
	}
	fclose(pvFile);
	FILE* pfFile = fopen(fragmentShaderFile, "rt");
	if (!pfFile)
	{
		Log::Message(Log::LOG_ERROR, "Can't open File: " + string(fragmentShaderFile));
		exit(0);
	}
	if (fseek(pfFile, 0L, SEEK_END) == 0)
	{
		long bufsize = ftell(pfFile);
		fseek(pfFile, 0L, SEEK_SET);

		size_t newLen = fread(sourceFragment, sizeof(char), bufsize, pfFile);
		sourceFragment[newLen] = '\0'; /* Just to be safe. */
	}
	fclose(pfFile);
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* pvStr = sourceVertex;
	glShaderSource(vertexShader, 1, &pvStr, NULL);
	glCompileShader(vertexShader);


	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &check);
	if (check == GL_FALSE)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		Log::Message(Log::LOG_ERROR, "Vertex shader compile error -> " + string(infoLog));

	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* pfStr = sourceFragment;
	glShaderSource(fragmentShader, 1, &pfStr, NULL);
	glCompileShader(fragmentShader);


	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &check);
	if (check == GL_FALSE)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		Log::Message(Log::LOG_ERROR, "Fragment shader compile error -> " + string(infoLog));

	}


	m_iProgramID = glCreateProgram();
	glAttachShader(m_iProgramID, vertexShader);
	glAttachShader(m_iProgramID, fragmentShader);
	glLinkProgram(m_iProgramID);

	glGetProgramiv(m_iProgramID, GL_LINK_STATUS, &check);
	if (check == GL_FALSE)
	{
		glGetProgramInfoLog(m_iProgramID, 512, NULL, infoLog);
		Log::Message(Log::LOG_ERROR, "Program Shader link error -> " + string(infoLog));
	}


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return true;
}
