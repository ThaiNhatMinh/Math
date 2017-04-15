#pragma once
#include "..\iostream.h"

class Shader
{
public:
	GLuint m_iProgramID;
	GLuint m_iVertexShader;
	GLuint m_iFragmentShader;
public:

public:
	Shader(const char* vertexShaderFile, const char* fragmentShaderFile);
	void LinkShader();
	Shader() {};
	~Shader();

	bool Load(const char* vertexShaderFile, const char* fragmentShaderFile);
	inline void Use() { glUseProgram(m_iProgramID); };
	inline void UnUse() { glUseProgram(0); };
	inline void SetUniformMatrix(const char* name, float* data) { glUniformMatrix4fv(glGetUniformLocation(m_iProgramID, name),1,GL_FALSE,data); }
	inline void SetUniformMatrix(GLuint location, float* data) { glUniformMatrix4fv(location, 1, GL_FALSE, data); }
	inline void SetUniform(const char* name, int val){ glUniform1i(glGetUniformLocation(m_iProgramID, name),val); }
	inline void SetUniform(const char* name, Vec3 val) { glUniform3f(glGetUniformLocation(m_iProgramID, name),val.x,val.y,val.z); }
	inline void SetUniform(const char* name, float x,float y,float z) { glUniform3f(glGetUniformLocation(m_iProgramID, name), x, y, z); }
	inline void SetUniform(const char* name, float x, float y) { glUniform2f(glGetUniformLocation(m_iProgramID, name), x, y); }
	inline void SetUniform(const char* name, float val) { glUniform1f(glGetUniformLocation(m_iProgramID, name), val); }

	inline GLuint GetAttribLocation(const char* name) { return glGetAttribLocation(m_iProgramID, name); };
	inline GLuint GetUniformLocation(const char* name) { return glGetUniformLocation(m_iProgramID, name); };
};

