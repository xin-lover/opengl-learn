#include "glwindow.h"
#include <GLFW/glfw3.h>
#include "../logger.h"

using namespace luwu;

GLWindow::GLWindow(string title,int width,int height,int major,int minor)
    :width_(width),height_(height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif

    window_ = glfwCreateWindow(width,height,title.c_str(),nullptr,nullptr);
    if(window_ == nullptr)
    {
        Logger::Error("glfw fail create window...");
        glfwTerminate();
    }
    else
    {
        glfwMakeContextCurrent(window_);

        glfwSetFramebufferSizeCallback(window_,GLWindow::size_callback);
        glfwSetCursorPosCallback(window_,GLWindow::mouse_callback);
        glfwSetScrollCallback(window_,GLWindow::scroll_callback);
    }
}

GLWindow::~GLWindow()
{
    glfwTerminate();
}

void GLWindow::RunMainLoop()
{
    while(!glfwWindowShouldClose(window_))
    {
        assert(updateCallback_);
        
        if(glfwGetKey(window_,GLFW_KEY_ESCAPE))
        {
            Close();
        }

        updateCallback_();
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

void GLWindow::SetCursorEnabled(bool enabled)
{
    if(enabled)
    {
        glfwSetInputMode(window_,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
    }
    else
    {
        glfwSetInputMode(window_,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    }
}

void GLWindow::Close()
{
	glfwSetWindowShouldClose(window_,true);
}