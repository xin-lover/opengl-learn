#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <functional>
#include <cassert>

#include "../public/shader.h"
#include "../public/texture.h"
#include "../public/threemath.h"
const int k_width = 800;
const int k_height = 600;

using namespace std;
using namespace luwu;

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

void framebuffer_size_callback(GLFWwindow* window,int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif

    GLFWwindow *window_ = glfwCreateWindow(k_width,k_height,"coordinate",nullptr,nullptr);
    if(window_ == nullptr)
    {
        Logger::Error("glfw fail create window...");
        glfwTerminate();
		return -1;
    }

	glfwMakeContextCurrent(window_);
	glfwSetFramebufferSizeCallback(window_,framebuffer_size_callback);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout<<"glfw load gl loader failed"<<std::endl;
		return -1;
	}


	glEnable(GL_DEPTH_TEST);

	GLuint vao;
	GLuint vbo;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);

    float vertices[] = {
         // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

	Vector3 cubePositions[]={
		Vector3(0.0f,0.0f,0.0f),
		Vector3(2.0f,5.0f,-15.0f),
		Vector3(-1.5f,-2.2f,-2.5f),
		Vector3(-3.8f,-2.0f,-12.3f),
	};

	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8 * sizeof(float),(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8 * sizeof(float),(void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glCheckError();

	// shared_ptr<Shader> sh = make_shared<Shader>("./phong.vert","./phong.frag");
	// sh->Use();
	// // sh->SetInt("image",0);
    // sh->SetVec3("lightColor",1.0f,1.0f,1.0f);
    // sh->SetVec3("lightPos",0.2f,0.0f,1.2f);
    // sh->SetVec3("viewPos",0,0,3);

	shared_ptr<Shader> sh = make_shared<Shader>("./spotlighting.vert","./spotlighting.frag");
	sh->Use();
    sh->SetInt("material.diffuse",0);
    sh->SetInt("material.specular",1);
    // sh->SetVec3("material.ambient",1.0f,0.5f,0.31f);
    // sh->SetVec3("material.diffuse",1.0,0.5f,0.31f);
    sh->SetVec3("material.specular",0.5f,0.5f,0.5f);
    sh->SetFloat("material.shininess",32.0f);
    
    sh->SetVec3("light.position",-0.0f,0.0f,1.6f);
    sh->SetVec3("light.direction",-0.0f,-0.0f,-1.2f);
    sh->SetFloat("light.cutoff",glm::cos(glm::radians(12.5f)));
    sh->SetFloat("light.outerCutoff",glm::cos(glm::radians(17.5f)));
    sh->SetFloat("light.constant",1.0f);
    sh->SetFloat("light.linear",0.09f);
    sh->SetFloat("light.quadratic",0.032f);
    sh->SetVec3("light.ambient",0.1f,0.1f,0.1f);
    sh->SetVec3("light.diffuse",0.5f,0.5f,0.5f);
    sh->SetVec3("light.specular",1.0f,1.0f,1.0f);

    sh->SetVec3("viewPos",0,0,3);

	//设置矩阵
	// glm::mat4 model = glm::mat4(1.0f);
	// model = glm::translate(model,glm::vec3(0.0,0.0,0.0));
	// float angle = 20.0f * 1;
	// glm的旋转使用的是右手法则
	// model = glm::rotate(model,glm::radians(angle),glm::vec3(1.0f,0.3f,0.5f));
	// sh->SetMatrix("model",&model[0][0]);

	// glm::mat4 view = glm::mat4(1.0f);
	// glm::mat4 projection = glm::mat4(1.0f);	

	// view = glm::translate(view,glm::vec3(0.0f,0.0f,-3.0f));
	// sh->SetMatrix("view",&view[0][0]);

	// projection = glm::perspective(glm::radians(45.0f),(float)k_width / (float)k_height,0.1f,100.0f);
	// sh->SetMatrix("projection",&projection[0][0]);


	Matrix4x4 view = Matrix4x4::Identity();
	view.Translate(Vector3(0.0f,0.0f,-3.0f));
	sh->SetMatrix("view",view.Get());

	float top = 0.1f * tan(22.5f / 180.0f * 3.1415926f);
	float right = top * k_width / k_height;
	// Matrix4x4 projection = Frustum(-right,right,-top,top,0.1f,100.0f);
	Matrix4x4 projection = Perspective(RADIAN(45.0f),(float)k_width/(float)k_height,0.1f,100.0f);
	sh->SetMatrix("projection",projection.Get());
	
	shared_ptr<Texture2D> tex = make_shared<Texture2D>("./container2.png");
	shared_ptr<Texture2D> spetex = make_shared<Texture2D>("./container2_specular.png");
	while(!glfwWindowShouldClose(window_))
    {
        if(glfwGetKey(window_,GLFW_KEY_ESCAPE))
        {
			glfwSetWindowShouldClose(window_,true);
        }


		glClearColor(0.2,0.3,0.3,1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//draw context
        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,tex->Get());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,spetex->Get());
		glBindVertexArray(vao);

		sh->Use();
        Vector3 lightColor;
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);
        Vector3 diffuseColor = lightColor * Vector3(0.5f);
        Vector3 ambientColor = diffuseColor * Vector3(0.2f);
        sh->SetVec3("light.ambient",ambientColor.x,ambientColor.y,ambientColor.z);
        sh->SetVec3("light.diffuse",diffuseColor.x,diffuseColor.y,diffuseColor.z);
		for(int i=0;i<4;++i)
		{
			Matrix4x4 model = Matrix4x4::Identity();
			model.Translate(cubePositions[i]);
			float angle = i * 20.0f;
			model.Rotate(Vector3(1.0f,0.3f,0.5f),angle);
			sh->SetMatrix("model",model.Get());

			glDrawArrays(GL_TRIANGLES,0,36);
		}


        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

void framebuffer_size_callback(GLFWwindow *window,int width, int height)
{
	glViewport(0,0,width,height);
}