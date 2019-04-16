#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include "../public/shader.h"
#include "../public/threemath.h"
#include "../public/camera.h"

using namespace std;
using namespace luwu;

const int kWidth = 640;
const int kHeight = 480;

void frame_size_callback(GLFWwindow *window,int width,int height);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    #ifdef /*define*/__APPLE__

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    #endif ///*define*/__APPLE__

    GLFWwindow *window = glfwCreateWindow(kWidth,kHeight,"ubo",nullptr,nullptr);
    if(window == nullptr) {
        printf("glfw create window fail...\n");
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window,frame_size_callback);
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("glad load gl loader fail..\n");
    }

    float cubeVertices[] = {
        // positions         
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };

    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    GLuint vbo;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices),cubeVertices,GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(GL_FLAT),(void*)0);


    Matrix4x4 model = Matrix4x4::Identity();
    Camera cam(Vector3(1,0,3));
    Matrix4x4 projection = Perspective(RADIAN(60.0f),(float)kWidth/kHeight,0.1f,100.0f);

    shared_ptr<Shader> sh = make_shared<Shader>("./simple.vert","./simple.frag");
    sh->Use();
    sh->SetMatrix("model",model.Get());
    sh->SetVec3("cubeColor",1.0f,1.0f,0.0f);

    shared_ptr<Shader> shblue = make_shared<Shader>("./simple.vert","./blue.frag");
    shblue->Use();
    Matrix4x4 blueModel = Matrix4x4::Identity();
    blueModel.Translate(1,1,0.0f);
    shblue->SetMatrix("model",blueModel.Get());

    GLuint uniformBlockIndexRed = glGetUniformBlockIndex(sh->Get(),"Matrices");
    glUniformBlockBinding(sh->Get(),uniformBlockIndexRed,0);

    GLuint uboMatrices;
    glGenBuffers(1,&uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER,uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER,2 * sizeof(Matrix4x4),nullptr,GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER,0);

    glBindBufferRange(GL_UNIFORM_BUFFER,0,uboMatrices,0,2 * sizeof(Matrix4x4));
    glBindBuffer(GL_UNIFORM_BUFFER,uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(Matrix4x4),cam.getMatrix().Get());
    glBufferSubData(GL_UNIFORM_BUFFER,sizeof(Matrix4x4),sizeof(Matrix4x4),projection.Get());

    while(!glfwWindowShouldClose(window)){
        
        if(glfwGetKey(window,GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window,GL_TRUE);
        }

        glClearColor(0.1,0.2,0.2,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        sh->Use();
        glDrawArrays(GL_TRIANGLES,0,36);

        shblue->Use();
        glDrawArrays(GL_TRIANGLES,0,36);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    return 0;
}

void frame_size_callback(GLFWwindow *window,int width,int height)
{
    glViewport(0,0,width,height);
}