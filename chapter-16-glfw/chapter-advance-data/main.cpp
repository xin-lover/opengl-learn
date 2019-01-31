#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <cstdio>
#include <vector>

#include "../public/shader.h"
#include "../public/camera.h"
#include "../public/threemath.h"

using namespace std;
using namespace luwu;

const int kWidth=600;
const int kHeight=480;
void frame_size_callback(GLFWwindow *window,int width,int height);

Camera camera(Vector3(0,0,3));

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(kWidth,kHeight,"advance data",nullptr,nullptr);
    if(window == nullptr)
    {
        printf("glfw create window fail..\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window,frame_size_callback);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("load gl loader fail.\n");
        glfwTerminate();
        return -1;
    }

    //读取数据
    ifstream f("./box.txt");
    vector<float> vertices;
    float t;
    while(!f.eof())
    {
        f >> t;
        vertices.push_back(t);
    }

    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    GLuint vbo;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(float),nullptr,GL_STATIC_DRAW);
    void* ptr =glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
    memcpy(ptr,&vertices[0],vertices.size() * sizeof(float));
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(GL_FLOAT),(void*)0);
    
    shared_ptr<Shader> sh = make_shared<Shader>("./simple.vert","./simple.frag");
    sh->Use();
    Matrix4x4 model = Matrix4x4::Identity();
    sh->SetMatrix("model",model.Get());
    Matrix4x4 projection = Perspective(RADIAN(60),(float)kWidth/(float)kHeight,0.1f,100.0f);
    sh->SetMatrix("projection",projection.Get());

    float deltaTime = 0.0f;
    float lastTime = glfwGetTime();
    while(!glfwWindowShouldClose(window))
    {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        if(glfwGetKey(window,GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window,true);
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(Camera_Movement::kForward, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(Camera_Movement::kBackward, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(Camera_Movement::kLeft, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(Camera_Movement::kRight, deltaTime);

        glClearColor(0.1,0.2,0.2,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        sh->Use();
        sh->SetMatrix("view",camera.getMatrix().Get());

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0,36);

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