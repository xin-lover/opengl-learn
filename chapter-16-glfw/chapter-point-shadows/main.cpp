#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <vector>

#include "../public/shader.h"
#include "../public/threemath.h"
#include "../public/camera.h"

using namespace std;
using namespace luwu;

const int kWidth=640;
const int kHeight=480;

void frame_size_callback(GLFWwindow* window,int width,int height);
void mouse_callback(GLFWwindow *window,double xPos, double yPos);

float lastPos_x =0.0f;
float lastPos_y = 0.0f;
bool firstMouse = true;
Camera cam(Vector3(0,0,3));

GLuint cubevao;
GLuint planevao;
void RenderScene(shared_ptr<Shader> sh);


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

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    #endif

    GLFWwindow *window = glfwCreateWindow(kWidth,kHeight,"point shadow",nullptr,nullptr);
    if(!window)
    {
        printf("glfw create window fail...\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,frame_size_callback);
    glfwSetCursorPosCallback(window,mouse_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("glad load gl loader fail..\n");
        glfwTerminate();
        return -1;
    }

    float vertices[]={
        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
        // Front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
        // Left face
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
        // Right face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        // Top face
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left      
    };

    float planeVertices[]=
    {
        0.5,-0.5,0.0, 0.0,1.0,0.0, 1.0,0.0,
        0.5,0.5,0.0,  0.0,1.0,0.0, 1.0,1.0,
        -0.5,0.5,0.0, 0.0,1.0,0.0, 0.0,1.0,

        -0.5,0.5,0.0, 0.0,1.0,0.0, 0.0,1.0,
        -0.5,-0.5,0.0,0.0,1.0,0.0, 0.0,0.0,
        0.5,-0.5,0.0, 0.0,1.0,0.0, 1.0,0.0,
    };

    glGenVertexArrays(1,&cubevao);
    glBindVertexArray(cubevao);
    GLuint cubevbo;
    glGenBuffers(1,&cubevbo);
    glBindBuffer(GL_ARRAY_BUFFER,cubevbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(GL_FLOAT),(void*)0);

    glGenVertexArrays(1,&planevao);
    glBindVertexArray(planevao);
    GLuint planevbo;
    glGenBuffers(1,&planevbo);
    glBindBuffer(GL_ARRAY_BUFFER,planevbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(planeVertices),planeVertices,GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(GL_FLOAT),(void*)0);

    Vector3 lightPos = Vector3(-12.0f,3.0f,0.3f);

    shared_ptr<Shader> sh = make_shared<Shader>("./pointshadow.vert","./pointshadow.frag");
    sh->Use();

    Matrix4x4 model = Matrix4x4::Identity();
    sh->SetMatrix("model",model.Get());
    sh->SetMatrix("view",cam.getMatrix().Get());
    Matrix4x4 projection = Perspective(RADIAN(60),(float)kWidth/kHeight,0.01f,100.0f);
    sh->SetMatrix("projection",projection.Get());
    sh->SetFloat("far_plane",25.0f);
    sh->SetVec3("lightPos",lightPos.x,lightPos.y,lightPos.z);

    GLuint depthCubemap;
    glGenTextures(1,&depthCubemap);
    const GLuint kShadowWidth = 1024,kShadowHeight = 1024;
    glBindTexture(GL_TEXTURE_CUBE_MAP,depthCubemap);
    for(GLuint i = 0;i<6;++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,GL_DEPTH_COMPONENT,kShadowWidth,kShadowHeight,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);

    GLuint depfbo;
    glGenFramebuffers(1,&depfbo);
    glBindFramebuffer(GL_FRAMEBUFFER,depfbo);
    glFramebufferTexture(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,depfbo,0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glCheckError();

    Matrix4x4 shadowProj = Perspective(RADIAN(90),(float)kShadowWidth/kShadowWidth,1.0f,25.0f);
    vector<Matrix4x4> shadowTransforms=
    {
        shadowProj * LookAt(lightPos,lightPos + Vector3(1.0f,0,0),Vector3(0.0,-1.0,0.0)),
        shadowProj * LookAt(lightPos,lightPos + Vector3(-1.0f,0,0),Vector3(0.0,-1.0,0.0)),
        shadowProj * LookAt(lightPos,lightPos + Vector3(0.0f,1.0f,0),Vector3(0.0,0.0,1.0)),
        shadowProj * LookAt(lightPos,lightPos + Vector3(0.0f,-1.0f,0),Vector3(0.0,0.0,-1.0)),
        shadowProj * LookAt(lightPos,lightPos + Vector3(0.0f,0,1.0),Vector3(0.0,-1.0,0.0)),
        shadowProj * LookAt(lightPos,lightPos + Vector3(0.0f,0,-1.0),Vector3(0.0,-1.0,0.0)),
    };

    shared_ptr<Shader> shadowSh = make_shared<Shader>("./pointshadowdepth.vert","./pointshadowdepth.frag","./pointshadowdepth.geom");
    shadowSh->Use();
    for(int i=0;i<6;++i)
    {
        shadowSh->SetMatrix("shadowMatrices[" + to_string(i) + "]",shadowTransforms[i].Get());
    }
    shadowSh->SetVec3("lightPos",lightPos.x,lightPos.y,lightPos.z);
    shadowSh->SetFloat("far_plane",25.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    float deltaTime = 0.0f;
    float lastTime = glfwGetTime();
    while(!glfwWindowShouldClose(window))
    {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        if(glfwGetKey(window,GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window,GL_TRUE);
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cam.ProcessKeyboard(Camera_Movement::kForward, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cam.ProcessKeyboard(Camera_Movement::kBackward, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cam.ProcessKeyboard(Camera_Movement::kLeft, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cam.ProcessKeyboard(Camera_Movement::kRight, deltaTime);

        glClearColor(0.1,0.2,0.2,1.0);
        glBindFramebuffer(GL_FRAMEBUFFER,depfbo);
        glViewport(0,0,kShadowWidth,kShadowWidth);
        glClear(GL_DEPTH_BUFFER_BIT);

        shadowSh->Use();
        RenderScene(shadowSh);
        glBindFramebuffer(GL_FRAMEBUFFER,0);

        glViewport(0,0,kWidth*2,kHeight*2);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        sh->Use();
        sh->SetMatrix("view",cam.getMatrix().Get());
        glBindTexture(GL_TEXTURE_CUBE_MAP,depthCubemap);
        RenderScene(sh);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void frame_size_callback(GLFWwindow *window, int width,int height)
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

void RenderScene(shared_ptr<Shader> sh)
{
    Matrix4x4 model = Matrix4x4::Identity();
    sh->SetMatrix("model",model.Get());
    glBindVertexArray(cubevao);
    glDrawArrays(GL_TRIANGLES,0,36);

    glBindVertexArray(planevao);
    model.Scale(4.0f);
    // model.Rotate(Vector3(0.0f,0.0f,1.0f),90.0f);
    model.Translate(0.0,0.0,-2.0);
    sh->SetMatrix("model",model.Get());
    glDrawArrays(GL_TRIANGLES,0,6);

    model = Matrix4x4::Identity();
    model.Scale(4.0f);
    model.Rotate(Vector3(0.0f,1.0f,0.0f),90);
    model.Translate(2,0.0f,0.0f);
    sh->SetMatrix("model",model.Get());
    glDrawArrays(GL_TRIANGLES,0,6);
}