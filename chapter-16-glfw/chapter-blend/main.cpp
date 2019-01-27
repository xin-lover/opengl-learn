#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <cstdio>
#include <iostream>

#include "../public/shader.h"
#include "../public/texture.h"
#include "../public/threemath.h"

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

const int k_width = 640;
const int k_height = 480;

void framebuffer_size_callback(GLFWwindow* window,int width,int height);
void  glfw_error_callback(int code,const char* msg);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,true);
#endif

    glfwSetErrorCallback(glfw_error_callback);
    GLFWwindow* window = glfwCreateWindow(k_width,k_height,"depth",nullptr,nullptr);
    if(window == nullptr)
    {
        Logger::Error("glfw create window fail.\n");
        char msg[256];
        const char *t = msg;
        glfwGetError(&t);
        printf("%s\n",msg);
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Logger::Error("glad load gl loader fail..\n");
        glfwTerminate();
        return -1;
    }
    


    float vertices[]={
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    float planeVertices[]={
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };

    float transparentVertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };
    
    //创建vao，vbo
    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    GLuint vbo;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(GL_FLAT),(void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    GLuint planeVAO;
    glGenVertexArrays(1,&planeVAO);
    glBindVertexArray(planeVAO);
    GLuint planeVBO;
    glGenBuffers(1,&planeVBO);
    glBindBuffer(GL_ARRAY_BUFFER,planeVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(planeVertices),planeVertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(GL_FLOAT),(void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    GLuint grassVAO,grassVBO;
    glGenVertexArrays(1,&grassVAO);
    glBindVertexArray(grassVAO);
    glGenBuffers(1,&grassVBO);
    glBindBuffer(GL_ARRAY_BUFFER,grassVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(transparentVertices),transparentVertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(GL_FLAT),(void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    shared_ptr<Shader> sh = make_shared<Shader>("./grass.vert","./grass.frag");
    sh->Use();

    Matrix4x4 model = Matrix4x4::Identity();
    model.Translate(Vector3(-1.0,0.0,-1.0));
    Matrix4x4 view = Matrix4x4::Identity();
    view.Translate(0,0,-3.0f);
    Matrix4x4 projection = Perspective(RADIAN(60.0f),(float)k_width/k_height,0.1f,100.0f);

    
    sh->SetMatrix("view",view.Get());
    sh->SetMatrix("projection",projection.Get());

    shared_ptr<Texture2D> tex = make_shared<Texture2D>("../resources/grass.png");
    shared_ptr<Texture2D> tex2 = make_shared<Texture2D>("../resources/marble.jpg");
    shared_ptr<Texture2D> tex3 = make_shared<Texture2D>("../resources/metal.png");
    shared_ptr<Texture2D> windowTex= make_shared<Texture2D>("../resources/window.png");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    while(!glfwWindowShouldClose(window))
    {
        if(glfwGetKey(window,GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window,true);
        }

        glClearColor(0.2,0.3,0.3,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sh->Use();

        Matrix4x4 model = Matrix4x4::Identity();
        model.Translate(Vector3(0.5,0.0,-0.0));
        sh->SetMatrix("model",model.Get());

        glBindTexture(GL_TEXTURE_2D,tex3->Get());
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0,36);

        model = Matrix4x4::Identity();
        model.Translate(Vector3(-1.0,0.0,-1.0));
        sh->SetMatrix("model",model.Get());

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0,36);

        glBindTexture(GL_TEXTURE_2D,tex2->Get());
        model = Matrix4x4::Identity();
        model.Translate(0.0f,0.0f,0.0f);
        sh->SetMatrix("model",model.Get());
        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES,0,6);

        model = Matrix4x4::Identity();
        model.Translate(Vector3(-1.0,0.0,0.5));
        sh->SetMatrix("model",model.Get());
        glBindTexture(GL_TEXTURE_2D,windowTex->Get());
        glBindVertexArray(grassVAO);
        glDrawArrays(GL_TRIANGLES,0,6);

        model = Matrix4x4::Identity();
        model.Translate(Vector3(0.3,0.0,0.8));
        sh->SetMatrix("model",model.Get());
        glBindTexture(GL_TEXTURE_2D,windowTex->Get());
        glBindVertexArray(grassVAO);
        glDrawArrays(GL_TRIANGLES,0,6);

        model = Matrix4x4::Identity();
        model.Translate(Vector3(-0.5,0.0,1.5));
        sh->SetMatrix("model",model.Get());
        glBindTexture(GL_TEXTURE_2D,windowTex->Get());
        glBindVertexArray(grassVAO);
        glDrawArrays(GL_TRIANGLES,0,6);

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

void glfw_error_callback(int code,const char* msg)
{
    printf("%d %s\n",code,msg);
}