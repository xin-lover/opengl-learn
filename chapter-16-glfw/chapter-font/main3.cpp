#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window,int width, int height);
void processInput(GLFWwindow* window);

int main()
{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
		glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);

		GLFWwindow *window = glfwCreateWindow(640,480,"learn opengl",NULL,NULL);
		if(window == NULL)
		{
				std::cout<<"failed create window"<<std::endl;
				glfwTerminate();
				return -1;
		}

		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
				std::cout<<"glfw load gl loader failed."<<std::endl;
				return -1;
		}

		while(!glfwWindowShouldClose(window))
		{
				processInput(window);
				glClearColor(0.2,0.3,0.3,1.0);
				glClear(GL_COLOR_BUFFER_BIT);

				glfwSwapBuffers(window);
				glfwPollEvents();

		}

		glfwTerminate();
		return 0;
}

void framebuffer_size_callback(GLFWwindow* window,int width,int height)
{
		glViewport(0,0,width,height);
}

void processInput(GLFWwindow* window)
{
		if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
				glfwSetWindowShouldClose(window,GL_TRUE);
		}
}
