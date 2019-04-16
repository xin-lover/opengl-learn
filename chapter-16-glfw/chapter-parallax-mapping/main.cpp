#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <memory>

#include "../public/threemath.h"
#include "../public/shader.h"
#include "../public/camera.h"
#include "../public/texture.h"

using namespace std;
using namespace luwu;

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

void frame_size_callback(GLFWwindow* window,int width,int height);
void mouse_callback(GLFWwindow *window,double xPos, double yPos);

const int kWidth = 640;
const int kHeight = 480;

float lastPos_x =0.0f;
float lastPos_y = 0.0f;
bool firstMouse = true;
Camera cam(Vector3(0,0,2.0));

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,true);
    #endif

    GLFWwindow* window = glfwCreateWindow(kWidth,kHeight,"parallax mapping",nullptr,nullptr);
    if(window == nullptr)
    {
        printf("glfw create window fail\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frame_size_callback);
    glfwSetCursorPosCallback(window,mouse_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("glad load gl loader fail.\n");
        glfwTerminate();
        return 1;
    }

    // float planeVertices[]=
    // {
    //     0.5,-0.5,0.0, 0.0,0.0,1.0, 1.0,0.0,
    //     0.5,0.5,0.0,  0.0,0.0,1.0, 1.0,1.0,
    //     -0.5,0.5,0.0, 0.0,0.0,1.0, 0.0,1.0,

    //     -0.5,0.5,0.0, 0.0,0.0,1.0, 0.0,1.0,
    //     -0.5,-0.5,0.0,0.0,0.0,1.0, 0.0,0.0,
    //     0.5,-0.5,0.0, 0.0,0.0,1.0, 1.0,0.0,
    // };

    Vector3 pos1(-1.0f,1.0f,0.0f);
    Vector3 pos2(-1.0f,-1.0f,0.0f);
    Vector3 pos3(1.0f,-1.0f,0.0f);
    Vector3 pos4(1.0f,1.0f,0.0f);

    Vector2 uv1(0.0f,1.0f);
    Vector2 uv2(0.0f,0.0f);
    Vector2 uv3(1.0f,0.0f);
    Vector2 uv4(1.0f,1.0f);

    Vector3 nm(0.0f,0.0f,1.0f);

    //calculate tangent/bitangent vector3 of both tirangles
    Vector3 tangent1,bitangent1;
    Vector3 tangent2,bitangent2;

    Vector3 edge1 = pos2-pos1;
    Vector3 edge2 = pos3-pos1;
    Vector2 deltaUV1= uv2-uv1;
    Vector2 deltaUV2= uv3-uv1;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent1.Normalized();

    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent1.Normalized();
    
    //triangles 2
    edge1 = pos3-pos1;
    edge2 = pos4-pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent2.Normalized();

    bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent2.Normalized();

    float planeVertices[]={
        pos1.x,pos1.y,pos1.z,nm.x,nm.y,nm.z,uv1.x,uv1.y,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
        pos2.x,pos2.y,pos2.z,nm.x,nm.y,nm.z,uv2.x,uv2.y,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,
        pos3.x,pos3.y,pos3.z,nm.x,nm.y,nm.z,uv3.x,uv3.y,tangent1.x,tangent1.y,tangent1.z,bitangent1.x,bitangent1.y,bitangent1.z,

        pos1.x,pos1.y,pos1.z,nm.x,nm.y,nm.z,uv1.x,uv1.y,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
        pos3.x,pos3.y,pos3.z,nm.x,nm.y,nm.z,uv3.x,uv3.y,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
        pos4.x,pos4.y,pos4.z,nm.x,nm.y,nm.z,uv4.x,uv4.y,tangent2.x,tangent2.y,tangent2.z,bitangent2.x,bitangent2.y,bitangent2.z,
    };


    GLuint planevao;
    glGenVertexArrays(1,&planevao);
    glBindVertexArray(planevao);
    GLuint planevbo;
    glGenBuffers(1,&planevbo);
    glBindBuffer(GL_ARRAY_BUFFER ,planevbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(planeVertices),planeVertices,GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,14*(sizeof(GL_FLOAT)),(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,14*sizeof(GL_FLOAT),(void*)(3*sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,14*sizeof(GL_FLOAT),(void*)(6*sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,14*sizeof(GL_FLOAT),(void*)(8*sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4,3,GL_FLOAT,GL_FALSE,14*sizeof(GL_FLOAT),(void*)(11*sizeof(GL_FLOAT)));

    shared_ptr<Shader> sh = make_shared<Shader>("./parallaxmapping.vert","./parallaxmapping.frag");
    sh->Use();
    Matrix4x4 model = Matrix4x4::Identity();
    // model.Rotate(Vector3(1.0,0.0,0.0),80.0);
    sh->SetMatrix("model",model.Get());
    sh->SetMatrix("view",cam.getMatrix().Get());
    Matrix4x4 projection = Perspective(RADIAN(60),(float)kWidth / kHeight,0.1f,100.0f);
    sh->SetMatrix("projection",projection.Get());

    sh->SetVec3("lightPos",-0.2,0.2,0.5);
    sh->SetVec3("lightColor",1.0,1.0,1.0);
    sh->SetVec3("viewPos",cam.GetPosition().x,cam.GetPosition().y,cam.GetPosition().z);
    sh->SetFloat("heightScale",0.05);


    shared_ptr<Texture2D> tex = make_shared<Texture2D>("../resources/bricks2.jpg");
    shared_ptr<Texture2D> normalTex = make_shared<Texture2D>("../resources/bricks2_normal.jpg");
    shared_ptr<Texture2D> depthTex = make_shared<Texture2D>("../resources/bricks2_disp.jpg");

    sh->SetInt("diffuseMap",0);
    sh->SetInt("normalMap",1);
    sh->SetInt("depthMap",2);

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
        // glClearColor(0.2,0.3,0.3,1.0);
        glClearColor(0.1,0.1,0.1,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        sh->Use();
        sh->SetMatrix("view",cam.getMatrix().Get());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex->Get());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,normalTex->Get());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,depthTex->Get());

        glBindVertexArray(planevao);
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