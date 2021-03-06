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

    GLFWwindow* window = glfwCreateWindow(kWidth,kHeight,"bloom",nullptr,nullptr);
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

    vector<Vector3> lightPoses;
    lightPoses.push_back(Vector3( 0.0f, 0.5f,  1.5f));
    lightPoses.push_back(Vector3(-4.0f, 0.5f, -3.0f));
    lightPoses.push_back(Vector3( 3.0f, 0.5f,  1.0f));
    lightPoses.push_back(Vector3(-.8f,  2.4f, -1.0f));

    vector<Vector3> lightColors;
    lightColors.push_back(Vector3(5.0f,   5.0f,  5.0f));
    lightColors.push_back(Vector3(10.0f,  0.0f,  0.0f));
    lightColors.push_back(Vector3(0.0f,   0.0f,  15.0f));
    lightColors.push_back(Vector3(0.0f,   5.0f,  0.0f));

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
    sh->SetInt("hdrBuffer",0);
    sh->SetInt("bloomBlur",1);

    shared_ptr<Shader> bloomsh = make_shared<Shader>("./bloom.vert","./bloom.frag");
    bloomsh->Use();
    Matrix4x4 cmodel = Matrix4x4::Identity();
    cmodel.Translate(0.0,0.0,-25.0);
    cmodel.Scale(2.5f);
    bloomsh->SetMatrix("model",cmodel.Get());
    bloomsh->SetMatrix("view",cam.getMatrix().Get());
    Matrix4x4 projection = Perspective(RADIAN(60.0f),(float)kWidth / kHeight,0.1f,100.0f);
    bloomsh->SetMatrix("projection",projection.Get());
    for(int i=0;i<lightPoses.size();++i)
    {
        auto a = lightPoses[i];
        bloomsh->SetVec3("lights[" + to_string(i) + "].position",a.x,a.y,a.z);
        a = lightColors[i];
        bloomsh->SetVec3("lights[" + to_string(i) + "].color",a.x,a.y,a.z);
    }
    // bloomsh->SetVec3("viewPos",cam.GetPosition().x,cam.GetPosition().y,cam.GetPosition().z);

    shared_ptr<Shader> blurSh = make_shared<Shader>("./blur.vert","./blur.frag");
    blurSh->Use();

    shared_ptr<Shader> lightBoxSh= make_shared<Shader>("./bloom.vert","./lightBox.frag");
    lightBoxSh->Use();
    lightBoxSh->SetMatrix("view",cam.getMatrix().Get());
    lightBoxSh->SetMatrix("projection",projection.Get());

    shared_ptr<Texture2D> wood = make_shared<Texture2D>("../resources/wood.png");
    shared_ptr<Texture2D> containerTex = make_shared<Texture2D>("../resources/container2.png");

    //帧缓存
    GLuint hdrfbo;
    glGenFramebuffers(1,&hdrfbo);
    glBindFramebuffer(GL_FRAMEBUFFER,hdrfbo);

    GLuint colorBuffers[2];
    glGenTextures(2,colorBuffers);
    for(int i=0;i<2;++i)
    {
        glBindTexture(GL_TEXTURE_2D,colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,kWidth,kHeight,0,GL_RGBA,GL_FLOAT,nullptr);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+i,GL_TEXTURE_2D,colorBuffers[i],0);
    }

    GLuint rboDepth;
    glGenRenderbuffers(1,&rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER,rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,kWidth,kHeight);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,rboDepth);

    GLuint attachments[2]={GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2,attachments);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("frame buffer not complete\n");
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);


    //pingpong framebuffer
    GLuint pingpongFBO[2];
    glGenFramebuffers(2,pingpongFBO);
    GLuint pingpongColorBuffers[2];
    glGenTextures(2,pingpongColorBuffers);
    for(int i=0;i<2;++i)
    {
        glBindFramebuffer(GL_FRAMEBUFFER,pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D,pingpongColorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,kWidth,kHeight,0,GL_RGB,GL_FLOAT,nullptr);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,pingpongColorBuffers[i],0);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("framebuffer not complete...\n");
        }
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

        glBindFramebuffer(GL_FRAMEBUFFER,hdrfbo);
        glViewport(0,0,kWidth,kHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        bloomsh->Use();
        bloomsh->SetMatrix("view",cam.getMatrix().Get());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,wood->Get());
        glBindVertexArray(cubeVao);

        Matrix4x4 model = Matrix4x4::Identity();
        model.Translate(0.0f,-1.0f,0.0f);
        model.Scale(12.5f,0.5f,12.5f);
        bloomsh->SetMatrix("model",model.Get());
        glDrawArrays(GL_TRIANGLES,0,36);

        glBindTexture(GL_TEXTURE_2D,containerTex->Get());
        model=Matrix4x4::Identity();
        model.Translate(2.0f,0.0f,1.0f);
        model.Scale(0.5f);
        bloomsh->SetMatrix("model",model.Get());
        glDrawArrays(GL_TRIANGLES,0,36);

        model=Matrix4x4::Identity();
        model.Rotate(Vector3(1.0,0.0,1.0),-60.0f);
        model.Translate(-1.0f,0.0f,2.0f);
        bloomsh->SetMatrix("model",model.Get());
        glDrawArrays(GL_TRIANGLES,0,36);

        model=Matrix4x4::Identity();
        model.Translate(0.0f,2.7f,4.0f);
        model.Rotate(Vector3(1.0,0.0,1.0),-23.0f);
        model.Scale(1.25f);
        bloomsh->SetMatrix("model",model.Get());
        glDrawArrays(GL_TRIANGLES,0,36);

        model=Matrix4x4::Identity();
        model.Translate(-2.0f,1.0f,-3.0f);
        model.Rotate(Vector3(1.0,0.0,1.0),-124.0f);
        bloomsh->SetMatrix("model",model.Get());
        glDrawArrays(GL_TRIANGLES,0,36);

        model=Matrix4x4::Identity();
        model.Translate(-3.0f,0.0f,0.0f);
        model.Scale(0.5f);
        bloomsh->SetMatrix("model",model.Get());
        glDrawArrays(GL_TRIANGLES,0,36);

        model=Matrix4x4::Identity();
        model.Translate(0.0f,1.5f,0.0f);
        model.Scale(0.5f);
        bloomsh->SetMatrix("model",model.Get());
        glDrawArrays(GL_TRIANGLES,0,36);

        lightBoxSh->Use();
        lightBoxSh->SetMatrix("view",cam.getMatrix().Get());
        for(int i=0;i<lightPoses.size();++i)
        {
            model = Matrix4x4::Identity();
            model.Translate(lightPoses[i]);
            model.Scale(0.25f);
            lightBoxSh->SetMatrix("model",model.Get());
            lightBoxSh->SetVec3("lightColor",lightColors[i].x,lightColors[i].y,lightColors[i].z);
            glDrawArrays(GL_TRIANGLES,0,36);
        }


        glBindFramebuffer(GL_FRAMEBUFFER,0);
        // glViewport(0,0,kWidth * 2,kHeight*2);

        blurSh->Use();
        int amount = 5;
        bool horizontal = false;
        bool first_iteration = true;
        for(int i=0;i<amount;++i)
        {
            glBindFramebuffer(GL_FRAMEBUFFER,pingpongFBO[horizontal]);
            blurSh->SetInt("horizontal",horizontal);
            glBindTexture(GL_TEXTURE_2D,first_iteration ? colorBuffers[1] : pingpongColorBuffers[!horizontal]);
            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            if(first_iteration)
            {
                first_iteration = false;
            }
            horizontal = !horizontal;
            glBindVertexArray(0);
        }

        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glViewport(0,0,kWidth*2,kHeight*2);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        sh->Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,colorBuffers[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,pingpongColorBuffers[!horizontal]);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);

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