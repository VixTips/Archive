#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>

class Window
{
public:
	Window(int width, int height, const char * title);
	void setContext(GLFWwindow* window) { glfwMakeContextCurrent(window); }

	bool processInput(GLFWwindow *window);

	static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
	bool updateWindow();

	~Window();


	GLFWwindow *m_window;
	int m_width;
	int m_height;

};