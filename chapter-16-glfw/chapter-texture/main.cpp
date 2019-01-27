#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "../public/shader.h"
#include "../public/texture.h"

#include <iostream>
#include <memory>

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

		GLuint vao;
		GLuint vbo;
		luwu::Shader sd("./texture.vert","./texture.frag");
		sd.Use();
		sd.SetInt("img1",0);
		sd.SetInt("img2",1);

		glGenVertexArrays(1,&vao);
		glBindVertexArray(vao);

		glGenBuffers(1,&vbo);
		glBindBuffer(GL_ARRAY_BUFFER,vbo);

		// float vertices[]=
		// {
		// 	-0.5,0.5,-1,0,1,
		// 	0.5,0.5,-1,1,1,
		// 	0.5,-0.5,-1,1,0,

		// 	0.5,-0.5,-1,1,0,
		// 	-0.5,-0.5,-1,0,0,
		// 	-0.5,0.5,-1,0,1
		// };
		float vertices[]={
			// positions          // colors           // texture coords
			0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
		};

		glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

		unsigned int indices[]={
			0,1,3,
			1,2,3
		};

		GLuint ebo;
		glGenBuffers(1,&ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(GL_FLOAT) * 8,(GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(GL_FLOAT) * 8,(GLvoid*)(sizeof(GL_FLOAT)*3));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(GL_FLOAT) * 8,(GLvoid*)(sizeof(GL_FLOAT)*6));
		glEnableVertexAttribArray(2);

		std::unique_ptr<luwu::Texture2D> tex = std::make_unique<luwu::Texture2D>("../resources/container.jpg");
		std::unique_ptr<luwu::Texture2D> tex2 = std::make_unique<luwu::Texture2D>("../resources/awesomeface.png");
		while(!glfwWindowShouldClose(window))
		{
				processInput(window);
				glClearColor(0.2,0.3,0.3,1.0);
				glClear(GL_COLOR_BUFFER_BIT);

				//triangle
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D,tex->Get());
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D,tex2->Get());
				sd.Use();
				glBindVertexArray(vao);
				glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT, 0);

				glfwSwapBuffers(window);
				glfwPollEvents();

		}

		glDeleteVertexArrays(1,&vao);
		glDeleteBuffers(1,&vbo);

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
