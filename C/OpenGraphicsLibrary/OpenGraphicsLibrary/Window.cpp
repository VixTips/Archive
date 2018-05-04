
#include "Window.h"

bool Window::processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		return true;
	}
	return false;
}

void Window::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}


Window::Window(int width, int height,const char * title)
{
	//init the window: create it, make it current context
	m_width = width;
	m_height = height;
	glfwInit();
	m_window = glfwCreateWindow(width, height, title, NULL, NULL);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwMakeContextCurrent(m_window);
	
	if (!m_window)
		std::cout << "Failed to Initialize the window\n";

	//initialize glew
	GLenum err = glewInit();
	if (err != GLEW_OK)
		std::cout << "Failed to Initialize GLEW\n";
	
	std::cout << glewGetErrorString(err) << std::endl;
	int major, minor, revision;
	glfwGetVersion(&major, &minor,&revision);
	std::cout << "OpenGL Version " << major << "." << minor << "." << revision << std::endl;
	
	//specify the opengl window
	glViewport(0, 0, width, height);

	//monitor framerate
	glfwSwapInterval(1);

	//clear color to, flush the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//init the resize callback
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
}

//terminate the glfw instance
Window::~Window()
{
	glfwTerminate();
}

//return if esc is pressed, poll events, flush buffer and do some other things...
bool Window::updateWindow()
{
	if (processInput(m_window)) { return true; }
	glfwSwapBuffers(m_window);
	glfwPollEvents();

	return false;;

}