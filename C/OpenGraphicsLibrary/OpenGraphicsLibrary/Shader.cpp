#include "Shader.h"
#include <stddef.h>
#include <iostream>

#include <stdio.h>
#include <errno.h>
#include <string>


Shader::Shader(const char* vs_location, const char* fs_location)
{	
	//vertex shader
	FILE * vs_file = fopen(vs_location, "r");
	if (vs_file == NULL)
		std::cout << "Failed to open Vertex Shader file\n";

	fseek(vs_file, 0, SEEK_END);
	int size = ftell(vs_file);
	rewind(vs_file);

	char *buffer;

	buffer = (char *)calloc(sizeof(char)*size,1);
	fread((void *)buffer, 1, size, vs_file);
	printf("\nVertex Shader source: \n%s\n-------------\n", buffer);
	fclose(vs_file);
	
	vs = buffer;

	
	//fragment shader
	FILE * fs_file = fopen(fs_location, "r");
	if (fs_file == NULL)
		std::cout << "Failed to open Fragment Shader file\n";

	fseek(fs_file, 0, SEEK_END);
	size = ftell(fs_file);
	rewind(fs_file);


	buffer = (char *)calloc(sizeof(char)*size,1);
	fread((void *)buffer, 1, size, fs_file);
	printf("Fragment Shader source: \n%s\n-------------\n", buffer);
	fclose(fs_file);
	
	fs = buffer;
}

Shader::Shader()
{
	//temp shader......

	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	m_program = shaderProgram;
}
void Shader::CompileShader()
{
		
	//vertex shader
	m_VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_VertexShaderID, 1, &vs, NULL);
	glCompileShader(m_VertexShaderID);

	int success;
	char infoLog[512];
	glGetShaderiv(m_VertexShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_VertexShaderID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//fragment shader
	m_FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);	
	glShaderSource(m_FragmentShaderID, 1, &fs, NULL);	
	glCompileShader(m_FragmentShaderID);	
	
	glGetShaderiv(m_FragmentShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_FragmentShaderID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	
	//program, attach and link shaders
	m_program = glCreateProgram();
	glAttachShader(m_program, m_VertexShaderID);
	glAttachShader(m_program, m_FragmentShaderID);
	glLinkProgram(m_program);

	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	
	//after linking, regardless of success of fail, detach the shaders
	glDeleteShader(m_VertexShaderID);
	glDeleteShader(m_FragmentShaderID);

	//glDeleteProgram(m_program);
}


void Shader::useProgram()
{
	glUseProgram(m_program);
}


Shader::~Shader()
{
	glDeleteProgram(m_program);
	
}
