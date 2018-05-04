#pragma once


#include <GL\glew.h>
class Shader
{
public:
	Shader(const char* vs, const char* fs);
	Shader();
	~Shader();

	void CompileShader();
	void useProgram();

	GLuint m_VertexShaderID;
	GLuint m_FragmentShaderID;
	GLuint m_program;
	const char* vs;
	const char* fs;
};

