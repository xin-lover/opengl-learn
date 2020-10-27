#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../public/texture.h"
#include "../public/shader.h"
#include "../public/threemath.h"
#include "../public/camera.h"
#include "../public/texture.h"

#include <cstdio>
#include <vector>

using namespace std;
using namespace luwu;

const int kWidth = 640;
const int kHeight= 480;

Camera cam(Vector3(0,0,6.0f));


void frame_size_callback(GLFWwindow* window,int width,int height);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    #ifdef /*define*/__APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,true);
    
    #endif ///*define*/__APPLE__

    GLFWwindow* window = glfwCreateWindow(kWidth,kHeight,"hdr",nullptr,nullptr);
    if(window == nullptr)
    {
        printf("glfw create window fail..\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,frame_size_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("glad load gl loader fail.\n");
        return -1;
    }

    vector<Vector3> lightPoses;
    lightPoses.push_back(Vector3(0.0f,0.0f,-30.5f));
    lightPoses.push_back(Vector3(-1.4f,-1.9f,-9.0f));
    lightPoses.push_back(Vector3(0.0f,-1.8f,4.0f));
    lightPoses.push_back(Vector3(0.8f,-1.7f,6.0f));

    vector<Vector3> lightColors;
    lightColors.push_back(Vector3(200.0f,200.0f,200.0f));
    lightColors.push_back(Vector3(200.1f,0.0f,0.0f));
    lightColors.push_back(Vector3(0.0f,0.0f,0.2f));
    lightColors.push_back(Vector3(0.0f,0.1f,0.0f));

    float vertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
        1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
        1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // right face
        1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
        1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
        1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
        1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
        1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
        1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
        // bottom face
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
        1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
        1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
        1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
        1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
        1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
    };

    GLuint cubeVao;
    glGenVertexArrays(1,&cubeVao);
    glBindVertexArray(cubeVao);
    GLuint cubeVbo;
    glGenBuffers(1,&cubeVbo);
    glBindBuffer(GL_ARRAY_BUFFER,cubeVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8 * sizeof(GL_FLOAT),(void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8 * sizeof(GL_FLOAT),(void*)(6 * sizeof(GL_FLOAT)));

    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    // setup plane VAO
    unsigned int quadVAO = 0;
    unsigned int quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));


    shared_ptr<Shader> sh = make_shared<Shader>("./hdr.vert","./hdr.frag");
    sh->Use();

    shared_ptr<Shader> lightsh = make_shared<Shader>("./lighting.vert","./lighting.frag");
    lightsh->Use();
    Matrix4x4 cmodel = Matrix4x4::Identity();
    cmodel.Translate(0.0,0.0,-25.0);
    cmodel.Scale(2.5f);
    lightsh->SetMatrix("model",cmodel.Get());
    lightsh->SetMatrix("view",cam.getMatrix().Get());
    Matrix4x4 projection = Perspective(RADIAN(60),(float)kWidth / kHeight,0.1f,100.0f);
    lightsh->SetMatrix("projection",projection.Get());
    for(int i=0;i<lightPoses.size();++i)
    {
        auto a = lightPoses[i];
        lightsh->SetVec3("lights[" + to_string(i) + "].position",a.x,a.y,a.z);
        a = lightColors[i];
        lightsh->SetVec3("lights[" + to_string(i) + "].color",a.x,a.y,a.z);
    }
    // lightsh->SetVec3("viewPos",cam.GetPosition().x,cam.GetPosition().y,cam.GetPosition().z);

    shared_ptr<Texture2D> wood = make_shared<Texture2D>("../resources/wood.png");

    //帧缓存
    GLuint hdrfbo;
    glGenFramebuffers(1,&hdrfbo);
    GLuint colorBuffer;
    glGenTextures(1,&colorBuffer);
    glBindTexture(GL_TEXTURE_2D,colorBuffer);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,kWidth,kHeight,0,GL_RGBA,GL_FLOAT,nullptr);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    GLuint rboDepth;
    glGenRenderbuffers(1,&rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER,rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,kWidth,kHeight);

    glBindFramebuffer(GL_FRAMEBUFFER,hdrfbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,colorBuffer,0);
    glFramebufferRenderbuffer(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,GL_RENDERBUFFER,rboDepth);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("frame buffer not complete\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER,0);

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window))
    {
        if(glfwGetKey(window,GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window,true);
        }

        glClearColor(0.1,0.1,0.1,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindFramebuffer(GL_FRAMEBUFFER,hdrfbo);
        glViewport(0,0,kWidth,kHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        lightsh->Use();
        glBindTexture(GL_TEXTURE_2D,wood->Get());
        glBindVertexArray(cubeVao);
        glDrawArrays(GL_TRIANGLES,0,36);
        glBindFramebuffer(GL_FRAMEBUFFER,0);

        glViewport(0,0,kWidth*2,kHeight*2);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        sh->Use();
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D,colorBuffer);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
    
}

void frame_size_callback(GLFWwindow* window,int width,int height)
{
    glViewport(0,0,width,height);
}