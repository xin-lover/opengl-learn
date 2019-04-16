#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>

#include "../public/shader.h"

using namespace std;
using namespace luwu;

const int kWidth=640;
const int kHeight=480;

void frame_size_callback(GLFWwindow *window,int width,int height);
void glfw_error_handle(int type,const char *msg);

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
        // std::cout << error << " | " << file << " (" << line << ")" << std::endl;
        printf("%s file %s line %d\n",error.c_str(),file,line);
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    #ifdef /*define*/__APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    #endif ///*define*/__APPLE__

    glfwSetErrorCallback(glfw_error_handle);
    GLFWwindow *window = glfwCreateWindow(kWidth,kHeight,"geometry",nullptr,nullptr);
    if(window == nullptr)
    {
        printf("glfw create window fail.\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window,frame_size_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("glad load gl loader fail..\n");
        glfwTerminate();
        return -1;
    }

    float vertices[]={
        -0.5f,0.5f,0.0f,
        0.5f,0.5f,0.0f,
        -0.5f,-0.5f,0.0f,
        0.5f,-0.5f,0.0f,
    };

    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    GLuint vbo;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(GL_FLOAT),(void*)0);

    shared_ptr<Shader> sh = make_shared<Shader>("./simple.vert","./simple.frag","./simple.gs");
    sh->Use();

    // glPointSize(10);
    glCheckError();
    while(!glfwWindowShouldClose(window))
    {
        if(glfwGetKey(window,GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window,true);
        }

        glClearColor(0.1,0.2,0.2,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        sh->Use();
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS,0,4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void frame_size_callback(GLFWwindow *window,int width,int height)
{
    glViewport(0,0,width,height);
}

void glfw_error_handle(int type,const char *msg)
{
    printf("type:%d error:%s\n",type,msg);
}