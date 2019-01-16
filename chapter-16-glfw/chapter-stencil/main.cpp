#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "threemath.h"
#include "texture.h"
#include "logger.h"
#include "shader.h"

#include <cstdio>

using namespace std;
using namespace luwu;

const int k_width = 640;
const int k_height = 480;

void frame_size_callback(GLFWwindow* window,int width,int height);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(k_width,k_height,"stencil",nullptr,nullptr);
    if(window == nullptr)
    {
        Logger::Error("glfw create window fail..\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,frame_size_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    { 
        Logger::Error("glad load gl loader fail..");
        return 1;
    }

    float cubeVertices[]={
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

    GLuint cubeVAO,cubeVBO;
    glGenVertexArrays(1,&cubeVAO);
    glBindVertexArray(cubeVAO);
    glGenBuffers(1,&cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER,cubeVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices),cubeVertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 *sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(GL_FLOAT),(void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    GLuint planeVAO,planeVBO;
    glGenVertexArrays(1,&planeVAO);
    glBindVertexArray(planeVAO);
    glGenBuffers(1,&planeVBO);
    glBindBuffer(GL_ARRAY_BUFFER,planeVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(planeVertices),planeVertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(GL_FLOAT),(void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);


    shared_ptr<Shader> sh = make_shared<Shader>("./stencil.vert","./stencil.frag");
    sh->Use();

    Matrix4x4 model = Matrix4x4::Identity();
    Matrix4x4 view = Matrix4x4::Identity();
    view.Translate(0,0,-3);
    Matrix4x4 projection = Perspective(RADIAN(60.0f),(float)k_width / k_height,0.1f,100.0f);

    sh->SetMatrix("model",model.Get());
    sh->SetMatrix("view",view.Get());
    sh->SetMatrix("projection",projection.Get());

    shared_ptr<Shader> sh2 = make_shared<Shader>("./frame.vert","./frame.frag");
    sh2->Use();
    sh2->SetMatrix("model",model.Get());
    sh2->SetMatrix("view",view.Get());
    sh2->SetMatrix("projection",projection.Get());

    shared_ptr<Texture2D> tex1 = make_shared<Texture2D>("../resources/marble.jpg");
    shared_ptr<Texture2D> tex2 = make_shared<Texture2D>("../resources/metal.png");



    while(!glfwWindowShouldClose(window))
    {
        if(glfwGetKey(window,GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window,true);
        }

        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);

        glClearColor(0.2,0.3,0.3,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glStencilMask(0x00);
        sh->Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex1->Get());
        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES,0,6);

        glStencilFunc(GL_ALWAYS,1,0xFF);
        glStencilMask(0xFF);
        sh->Use();
        Matrix4x4 model = Matrix4x4::Identity();
        sh->SetMatrix("model",model.Get());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex2->Get());
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES,0,36);

        glStencilFunc(GL_NOTEQUAL,1,0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        sh2->Use();
        model = Matrix4x4::Identity();
        model.Scale(1.1f);
        sh2->SetMatrix("model",model.Get());
        glDrawArrays(GL_TRIANGLES,0,36);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}

void frame_size_callback(GLFWwindow* window,int width,int height)
{
    glViewport(0,0,width,height);
}