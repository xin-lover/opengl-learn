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

float lastPos_x =0.0f;
float lastPos_y = 0.0f;
bool firstMouse = true;
Camera cam(Vector3(0,0,6.0f));

void frame_size_callback(GLFWwindow* window,int width,int height);
void mouse_callback(GLFWwindow *window,double xPos, double yPos);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    #ifdef /*define*/__APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,true);
    
    #endif ///*define*/__APPLE__

    GLFWwindow* window = glfwCreateWindow(kWidth,kHeight,"deffered shading",nullptr,nullptr);
    if(window == nullptr)
    {
        printf("glfw create window fail..\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window,mouse_callback);
    glfwSetFramebufferSizeCallback(window,frame_size_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("glad load gl loader fail.\n");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // vector<Vector3> lightPoses;
    // lightPoses.push_back(Vector3( 0.0f, 0.5f,  1.5f));
    // lightPoses.push_back(Vector3(-4.0f, 0.5f, -3.0f));
    // lightPoses.push_back(Vector3( 3.0f, 0.5f,  1.0f));
    // lightPoses.push_back(Vector3(-.8f,  2.4f, -1.0f));

    // vector<Vector3> lightColors;
    // lightColors.push_back(Vector3(5.0f,   5.0f,  5.0f));
    // lightColors.push_back(Vector3(10.0f,  0.0f,  0.0f));
    // lightColors.push_back(Vector3(0.0f,   0.0f,  15.0f));
    // lightColors.push_back(Vector3(0.0f,   5.0f,  0.0f));

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


    shared_ptr<Shader> sh = make_shared<Shader>("./gbuffer.vert","./gbuffer.frag");
    sh->Use();
    Matrix4x4 cmodel = Matrix4x4::Identity();
    cmodel.Translate(0.0,0.0,-25.0);
    cmodel.Scale(2.5f);
    sh->SetMatrix("model",cmodel.Get());
    sh->SetMatrix("view",cam.getMatrix().Get());
    Matrix4x4 projection = Perspective(RADIAN(60.0f),(float)kWidth / kHeight,0.1f,100.0f);
    sh->SetMatrix("projection",projection.Get());

    shared_ptr<Shader> defferedSh = make_shared<Shader>("./defferedshading.vert","./defferedshading.frag");
    defferedSh->Use();
    defferedSh->SetInt("gPosition",0);
    defferedSh->SetInt("gNormal",1);
    defferedSh->SetInt("gAlbedoSpec",2);
    const int kLightNum = 32;
    vector<Vector3> lightPoses;
    vector<Vector3> lightColors;
    for(int i=0;i<kLightNum;++i)
    {
        float xPos = ((rand()%100) / 100.0) * 6.0 - 3.0;
        float yPos = ((rand()%100) / 100.0) * 6.0 - 4.0;
        float zPos = ((rand()%100) / 100.0) * 6.0 - 3.0;
        defferedSh->SetVec3("lights[" + to_string(i) + "].position",xPos,yPos,zPos);
        float r = ((rand()%100)/200.0) + 0.5;
        float g = ((rand()%100)/200.0) + 0.5;
        float b = ((rand()%100)/200.0) + 0.5;
        defferedSh->SetVec3("lights[" + to_string(i) + "].color",r,g,b);

        defferedSh->SetFloat("lights[" + to_string(i) +"].linear",0.7);
        defferedSh->SetFloat("lights[" + to_string(i) +"].quadratic",1.8);

        lightPoses.push_back(Vector3(xPos,yPos,zPos));
        lightColors.push_back(Vector3(r,g,b));
    }

    // shared_ptr<Shader> lightBoxSh= make_shared<Shader>("./bloom.vert","./lightBox.frag");
    // lightBoxSh->Use();
    // lightBoxSh->SetMatrix("view",cam.getMatrix().Get());
    // lightBoxSh->SetMatrix("projection",projection.Get());

    shared_ptr<Shader> debugsh = make_shared<Shader>("./fbo_debug.vert","./fbo_debug.frag");
    debugsh->Use();

    shared_ptr<Shader> lightBoxSh = make_shared<Shader>("./lightBox.vert","./lightBox.frag");
    lightBoxSh->Use();
    lightBoxSh->SetMatrix("view",cam.getMatrix().Get());
    lightBoxSh->SetMatrix("projection",projection.Get());

    shared_ptr<Texture2D> wood = make_shared<Texture2D>("../resources/wood.png");
    shared_ptr<Texture2D> containerTex = make_shared<Texture2D>("../resources/container2.png");

    //帧缓存
    GLuint gBuffer;
    glGenFramebuffers(1,&gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,gBuffer);

    GLuint gPostion,gNormal,gAlbedoSpec;
    glGenTextures(1,&gPostion);
    glBindTexture(GL_TEXTURE_2D,gPostion);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,kWidth,kHeight,0,GL_RGB,GL_FLOAT,nullptr);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,gPostion,0);

    glGenTextures(1,&gNormal);
    glBindTexture(GL_TEXTURE_2D,gNormal);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,kWidth,kHeight,0,GL_RGB,GL_FLOAT,nullptr);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,gNormal,0);

    glGenTextures(1,&gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D,gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,kWidth,kHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,nullptr);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT2,GL_TEXTURE_2D,gAlbedoSpec,0);

    GLuint rboDepth;
    glGenRenderbuffers(1,&rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER,rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,kWidth,kHeight);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,rboDepth);

    GLuint attachments[3]={GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3,attachments);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("frame buffer not complete\n");
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    float deltaTime = 0.0f;
    float lastTime = glfwGetTime();
    while(!glfwWindowShouldClose(window))
    {
        if(glfwGetKey(window,GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window,true);
        }

        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cam.ProcessKeyboard(Camera_Movement::kForward, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cam.ProcessKeyboard(Camera_Movement::kBackward, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cam.ProcessKeyboard(Camera_Movement::kLeft, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cam.ProcessKeyboard(Camera_Movement::kRight, deltaTime);

        glClearColor(0.1,0.1,0.1,1.0);

        glBindFramebuffer(GL_FRAMEBUFFER,gBuffer);
        glViewport(0,0,kWidth,kHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sh->Use();
        sh->SetMatrix("view",cam.getMatrix().Get());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,wood->Get());
        glBindVertexArray(cubeVao);

        Matrix4x4 model = Matrix4x4::Identity();
        model.Translate(0.0f,-1.0f,0.0f);
        model.Scale(12.5f,0.5f,12.5f);
        sh->SetMatrix("model",model.Get());
        glDrawArrays(GL_TRIANGLES,0,36);

        glBindTexture(GL_TEXTURE_2D,containerTex->Get());
        model=Matrix4x4::Identity();
        model.Translate(2.0f,0.0f,1.0f);
        model.Scale(0.5f);
        sh->SetMatrix("model",model.Get());
        glDrawArrays(GL_TRIANGLES,0,36);

        model=Matrix4x4::Identity();
        model.Rotate(Vector3(1.0,0.0,1.0),-60.0f);
        model.Translate(-1.0f,0.0f,2.0f);
        sh->SetMatrix("model",model.Get());
        glDrawArrays(GL_TRIANGLES,0,36);

        model=Matrix4x4::Identity();
        model.Translate(0.0f,2.7f,4.0f);
        model.Rotate(Vector3(1.0,0.0,1.0),-23.0f);
        model.Scale(1.25f);
        sh->SetMatrix("model",model.Get());
        glDrawArrays(GL_TRIANGLES,0,36);

        model=Matrix4x4::Identity();
        model.Translate(-2.0f,1.0f,-3.0f);
        model.Rotate(Vector3(1.0,0.0,1.0),-124.0f);
        sh->SetMatrix("model",model.Get());
        glDrawArrays(GL_TRIANGLES,0,36);

        model=Matrix4x4::Identity();
        model.Translate(-3.0f,0.0f,0.0f);
        model.Scale(0.5f);
        sh->SetMatrix("model",model.Get());
        glDrawArrays(GL_TRIANGLES,0,36);

        model=Matrix4x4::Identity();
        model.Translate(0.0f,1.5f,0.0f);
        model.Scale(0.5f);
        sh->SetMatrix("model",model.Get());
        glDrawArrays(GL_TRIANGLES,0,36);

        // lightBoxSh->Use();
        // lightBoxSh->SetMatrix("view",cam.getMatrix().Get());
        // for(int i=0;i<lightPoses.size();++i)
        // {
        //     model = Matrix4x4::Identity();
        //     model.Translate(lightPoses[i]);
        //     model.Scale(0.25f);
        //     lightBoxSh->SetMatrix("model",model.Get());
        //     lightBoxSh->SetVec3("lightColor",lightColors[i].x,lightColors[i].y,lightColors[i].z);
        //     glDrawArrays(GL_TRIANGLES,0,36);
        // }

        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0,0,kWidth*2,kHeight*2);
        defferedSh->Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,gPostion);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,gAlbedoSpec);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);

        glBindFramebuffer(GL_READ_FRAMEBUFFER,gBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
        glBlitFramebuffer(0,0,kWidth,kHeight,0,0,kWidth * 2,kHeight * 2,GL_DEPTH_BUFFER_BIT,GL_NEAREST);

        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glViewport(0,0,kWidth*2,kHeight*2);

        lightBoxSh->Use();
        lightBoxSh->SetMatrix("view",cam.getMatrix().Get());
        glBindVertexArray(cubeVao);
        for(int i=0;i<kLightNum;++i)
        {
            Matrix4x4 model = Matrix4x4::Identity();
            model.Translate(lightPoses[i]);
            model.Scale(0.125);
            lightBoxSh->SetMatrix("model",model.Get());
            lightBoxSh->SetVec3("lightColor",lightColors[i].x,lightColors[i].y,lightColors[i].z);
            glDrawArrays(GL_TRIANGLES,0,36);
        }

        // debugsh->Use();
        // glBindVertexArray(quadVAO);
        // glViewport(0,0,kWidth,kHeight);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D,gPostion);
        // glDrawArrays(GL_TRIANGLE_STRIP,0,4);

        // glViewport(kWidth,0,kWidth,kHeight);
        // glBindTexture(GL_TEXTURE_2D,gNormal);
        // glDrawArrays(GL_TRIANGLE_STRIP,0,4);

        // glViewport(0,kHeight,kWidth,kHeight);
        // glBindTexture(GL_TEXTURE_2D,gAlbedoSpec);
        // glDrawArrays(GL_TRIANGLE_STRIP,0,4);

        // glViewport(0,0,kWidth,kHeight);
        // glBindTexture(GL_TEXTURE_2D,gPostion);
        // glDrawArrays(GL_TRIANGLE_STRIP,0,4);

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

void mouse_callback(GLFWwindow *window,double xPos, double yPos)
{
    if(firstMouse)
    {
        firstMouse = false;
        lastPos_x = xPos;
        lastPos_y = yPos;
    }

    float xOffset = xPos - lastPos_x;
    float yOffset = lastPos_y-yPos;
    lastPos_x = xPos;
    lastPos_y = yPos;
    cam.ProcessMouseMovement(xOffset,yOffset);
}