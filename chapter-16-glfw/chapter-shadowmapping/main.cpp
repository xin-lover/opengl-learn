#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>

#include "../public/shader.h"
#include "../public/camera.h"
#include "../public/threemath.h"
#include "../public/texture.h"

using namespace std;
using namespace luwu;

const int kWidth=640;
const int kHeight=480;

float lastPos_x =0.0f;
float lastPos_y = 0.0f;
bool firstMouse = true;
Camera cam(Vector3(0,2.0f,8.0f));

void mouse_callback(GLFWwindow *window,double xPos, double yPos);
void frame_size_callback(GLFWwindow *window,int width,int height);

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

    GLFWwindow *window = glfwCreateWindow(kWidth,kHeight,"shadowmapping",nullptr,nullptr);
    if(!window)
    {
        printf("glfw create window fail..");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, frame_size_callback);
    glfwSetCursorPosCallback(window,mouse_callback);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("glad load gl loader fail..\n");
        return -1;
    }


    glEnable(GL_DEPTH_TEST);

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

    float quadVertices[]=
    {
        0.5,-0.5,0.0, 0.0,1.0,0.0, 1.0,0.0,
        0.5,0.5,0.0,  0.0,1.0,0.0, 1.0,1.0,
        -0.5,0.5,0.0, 0.0,1.0,0.0, 0.0,1.0,

        -0.5,0.5,0.0, 0.0,1.0,0.0, 0.0,1.0,
        -0.5,-0.5,0.0,0.0,1.0,0.0, 0.0,0.0,
        0.5,-0.5,0.0, 0.0,1.0,0.0, 1.0,0.0,
    };

    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    GLuint vbo;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8 * sizeof(GL_FLOAT),(void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8 * sizeof(GL_FLOAT),(void*)(6 * sizeof(GL_FLOAT)));

    GLuint quadvao;
    glGenVertexArrays(1,&quadvao);
    glBindVertexArray(quadvao);
    GLuint quadVbo;
    glGenBuffers(1,&quadVbo);
    glBindBuffer(GL_ARRAY_BUFFER,quadVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices),quadVertices,GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8 * sizeof(GL_FLOAT),(void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8 * sizeof(GL_FLOAT),(void*)(6 * sizeof(GL_FLOAT)));

    shared_ptr<Shader> sh = make_shared<Shader>("./shadow_mapping_depth.vert","./shadow_mapping_depth.frag");
    sh->Use();

    Matrix4x4 model = Matrix4x4::Identity();
    sh->SetMatrix("model",model.Get());
    sh->SetMatrix("view",cam.getMatrix().Get());
    Matrix4x4 projection = Perspective(RADIAN(60.f),(float)kWidth/(float)kHeight,0.01f,100.0f);
    sh->SetMatrix("projection",projection.Get());
    // sh->SetFloat("near_plane",0.01f);
    // sh->SetFloat("far_plane",100.0f);
    glCheckError();

    //创建阴影贴图
    GLuint depthMapFBO;
    glGenFramebuffers(1,&depthMapFBO);

    const int kMapWidth = 1024,kMapHeight=1024;
    GLuint depthMap;
    glGenTextures(1,&depthMap);
    glBindTexture(GL_TEXTURE_2D,depthMap);
    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,kMapWidth,kMapHeight,0,GL_DEPTH_COMPONENT,GL_FLOAT,nullptr);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER,depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depthMap,0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    Matrix4x4 lightProjection = Ortho(-10.0f,10.0f,-10.0f,10.0f,1.0f,7.5f);
    Matrix4x4 lightView = LookAt(Vector3(-2.0f,4.0f,-1.0f),Vector3(0.0f),Vector3(0.0f,1.0f,0.0f));
    Matrix4x4 lightSpaceMatrix = lightProjection * lightView;

    shared_ptr<Shader> dpsh = make_shared<Shader>("./simpledepth.vert","./simpledepth.frag");
    dpsh->Use();
    dpsh->SetMatrix("lightSpaceMatrix",lightSpaceMatrix.Get());
    dpsh->SetMatrix("model",model.Get());

    sh->Use();
    sh->SetMatrix("lightSpaceMatrix",lightSpaceMatrix.Get());

    shared_ptr<Texture2D> wood = make_shared<Texture2D>("../resources/marble.jpg");

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


        glBindFramebuffer(GL_FRAMEBUFFER,depthMapFBO);
        glViewport(0,0,kMapWidth,kMapHeight);
        glClearColor(0.1,0.2,0.2,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        dpsh->Use();
        Matrix4x4 model = Matrix4x4::Identity();
        dpsh->SetMatrix("model",model.Get());
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0,36);

        // model.Translate(-0.5f,0.5f,0.0f);
        model.Scale(5.0);
        model.Rotate(Vector3(1.0,0.0,0.0),-90.0);
        model.Translate(-2.0,-1.0,0.0);
        dpsh->SetMatrix("model",model.Get());
        glBindVertexArray(quadvao);
        glDrawArrays(GL_TRIANGLES,0,6);

        glBindFramebuffer(GL_FRAMEBUFFER,0);

        glViewport(0,0,kWidth * 2,kHeight*2);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // model = Matrix4x4::Identity();
        // sh->Use();
        // glBindTexture(GL_TEXTURE_2D,depthMap);
        // sh->SetMatrix("model",model.Get());
        // glBindVertexArray(quadvao);
        // glDrawArrays(GL_TRIANGLES,0,6);
        // glCheckError();

        sh->Use();
        sh->SetMatrix("view",cam.getMatrix().Get());
        model = Matrix4x4::Identity();
        sh->SetMatrix("model",model.Get());
        glBindTexture(GL_TEXTURE_2D,depthMap);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0,36);

        model.Scale(5.0);
        model.Rotate(Vector3(1.0,0.0,0.0),-90.0);
        model.Translate(-2.0,-1.0,0.0);
        sh->SetMatrix("model",model.Get());
        glBindVertexArray(quadvao);
        glBindTexture(GL_TEXTURE_2D,depthMap);
        glDrawArrays(GL_TRIANGLES,0,6);

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