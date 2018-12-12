#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdio>

#include "../public/shader.h"
#include "../public/texture.h"
#include "ftfont.h"


using namespace std;
using namespace luwu;

void framebuffer_size_callback(GLFWwindow* window,int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(640,480,"font",NULL,NULL);
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
		std::cout<<"glfw load gl loader failed"<<std::endl;
		return -1;
	}


	GLuint vao;
	GLuint vbo;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);

	float vertices[]={
		-0.5,-0.5,0.0, 0.0,0.0,
		-0.5,0.5,0.0,  0.0,1.0,
		0.5,0.5,0.0,   1.0,1.0,
		
		0.5,0.5,0.0,	1.0,1.0,
		0.5,-0.5,0.0,	1.0,0.0,
		-0.5,-0.5,0.0,	0.0,0.0
	};

	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER,0);

	Shader shader("./font.vert","./font.frag");
	//Texture2D tex("./Lenna.png");
	FTFont ft("./Times New Roman.ttf");
	Bitmap b = ft.Load(0x41);
	printf("%d %d %d\n",b.width,b.height,b.data);
	FILE *f = fopen("./a.txt","w");
    int  i, j;
	unsigned char* data = b.data;
    for ( i = 0; i < b.height; i++ )
    {
        for ( j = 0; j < b.width; j++ )
		{
			char c = data[i * b.width + j];
			fputc( c == 0 ? ' '
                                    : c < 128 ? '+'
                                                        : '*' ,f);	
		}
            
        fputc( '\n',f );
    }

    fclose(f);
	Texture2D tex = Texture2D::LoadTexture(b.data,b.width,b.height,GL_RED);

	while(!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2,0.3,0.3,1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw context
		glBindVertexArray(vao);
		glBindTexture(GL_TEXTURE_2D,tex.Get());
		shader.Use();
		glDrawArrays(GL_TRIANGLES,0,6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if(glfwGetKey(window,GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window,true);
	}
}

void framebuffer_size_callback(GLFWwindow *window,int width, int height)
{
	glViewport(0,0,width,height);
}
