#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <memory>
#include <cstdlib>
#include <cstdio>
#include <map>

#include "../public/shader.h"
#include "../public/texture.h"
#include "ftfont.h"

const int k_width = 800;
const int k_height = 600;

using namespace std;
using namespace luwu;

void framebuffer_size_callback(GLFWwindow* window,int width, int height);
void processInput(GLFWwindow* window);

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

struct Character
{
	std::shared_ptr<Texture2D> tex;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};

map<char,Character> characters;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(k_width,k_height,"font",NULL,NULL);
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
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat) * 6 * 4,nullptr,GL_DYNAMIC_DRAW);

	// float vertices = {
	// -0.5, -0.5, 0.0, 0.0, 0.0,
	// -0.5, 0.5, 0.0, 0.0, 1.0,
	// 0.5, 0.5, 0.0, 1.0, 1.0,

	// 0.5, 0.5, 0.0, 1.0, 1.0,
	// 0.5, -0.5, 0.0, 1.0, 0.0,
	// -0.5, -0.5, 0.0, 0.0, 0.0};

	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,4 * sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER,0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glCheckError();

	Shader shader("./font.vert","./font.frag");
	shader.Use();

	glm::mat4 projection = glm::ortho(0.0f,static_cast<GLfloat>(k_width),0.0f,static_cast<GLfloat>(k_height));
	shader.SetMatrix("projection",glm::value_ptr(projection));
	shader.SetVec3("textColor",0.1,1.0,0.2);

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	FTFont ft("./Times New Roman.ttf");
	for(GLubyte c=0;c<128;++c)
	{
		GlyphRec b = ft.Load(c);
		std::shared_ptr<Texture2D> tex = Texture2D::LoadTexture(b.data,b.width,b.height,GL_RED);
		Character ct = {
			tex,
			glm::ivec2(b.width,b.height),
			glm::ivec2(b.bearingx,b.bearingy),
			b.advance,
		};

		characters.insert(std::make_pair(c,ct));
	}

	glCheckError();

	while(!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2,0.3,0.3,1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw context
		glBindVertexArray(vao);

		float startx = 25.0f;
		float starty = 25.0f;
		float scale = 0.5f;
		string text = "this is  sample text";
		for(auto c = text.begin();c != text.end();++c)
		{
			Character ch = characters[*c];

			GLfloat xpos = startx + ch.bearing.x * scale;
			GLfloat ypos = starty - (ch.size.y - ch.bearing.y) * scale;
			GLfloat w = ch.size.x * scale;
			GLfloat h = ch.size.y * scale;
			float vertices[6][4] = {
				{xpos,ypos + h, 0.0,0.0},
				{xpos,	ypos,	0.0,1.0},
				{xpos + w,	ypos,	1.0,1.0},
				{xpos, ypos + h, 0.0,0.0},
				{xpos + w, ypos, 1.0,1.0},
				{xpos + w, ypos + h, 1.0,0.0},
			};

			glBindBuffer(GL_ARRAY_BUFFER,vbo);
			glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertices),vertices);
			glBindBuffer(GL_ARRAY_BUFFER,0);

			glBindTexture(GL_TEXTURE_2D,ch.tex->Get());
			shader.Use();
			glDrawArrays(GL_TRIANGLES,0,6);

			startx += (ch.advance >> 6) * scale;
		}


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
