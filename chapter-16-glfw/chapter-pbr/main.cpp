#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <vector>

#include "../public/threemath.h"
#include "../public/shader.h"
#include "../public/camera.h"

using namespace std;
using namespace luwu;

const int kWidth = 640;
const int kHeight = 480;

void frame_size_callback(GLFWwindow *window,int width,int height);
void mouse_callback(GLFWwindow *window,double xPos, double yPos);

float lastPos_x =0.0f;
float lastPos_y = 0.0f;
bool firstMouse = true;
Camera cam(Vector3(0.0f,0.0f,3.0f));

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

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,true);
    #endif ///*define*/__APPLE__

    GLFWwindow *window = glfwCreateWindow(kWidth,kHeight,"pbr",nullptr,nullptr);
    if(window == nullptr)
    {
        printf("glfw create window fail\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,frame_size_callback);
    glfwSetCursorPosCallback(window,mouse_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("glad load gl loader fail..\n");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
glCheckError();

    vector<Vector3> positions;
    vector<Vector3> normals;
    vector<Vector2> uvs;
    vector<int> indices;

    const int kXSegs = 64;
    const int kYSegs = 64;
    const float kPI = 3.14159265359;

    for(int i=0;i<=kYSegs;++i)
    {
        float yseg = (float)i / (float)kYSegs;
        for(int j=0;j<=kXSegs;++j)
        {
            float xseg = (float)j / (float)kXSegs;
            float xPos = cos(xseg * 2.0f * kPI) * sin(yseg * kPI);
            float yPos = cos(yseg*kPI);
            float zPos = sin(xseg * 2.0f * kPI) * sin(yseg * kPI);

            positions.push_back(Vector3(xPos,yPos,zPos));
            uvs.push_back(Vector2(xseg,yseg));
            normals.push_back(Vector3(xPos,yPos,zPos));
        }
    }

    bool oddRow = false;
    for(int i=0;i<kYSegs;++i)
    {
        //三角带组织，到带尾之后再往回组织
        if(!oddRow)
        {
            for(int j=0;j<=kXSegs;++j)
            {
                indices.push_back(i * (kXSegs + 1) + j);
                indices.push_back((i+1) * (kXSegs + 1) + j);
            }
        }
        else
        {
            for(int j=kXSegs;j>=0;--j)
            {
                indices.push_back((i+1) * (kXSegs + 1) + j);
                indices.push_back(i * (kXSegs + 1) + j);
            }
        }

        oddRow = !oddRow;
    }
    int indicesCount = indices.size();

    vector<float> data;
    for(int i=0;i<positions.size();++i)
    {
        data.push_back(positions[i].x);
        data.push_back(positions[i].y);
        data.push_back(positions[i].z);

        if(uvs.size() > 1)
        {
            data.push_back(uvs[i].x);
            data.push_back(uvs[i].y);
        }

        if(normals.size() > 1)
        {
            data.push_back(normals[i].x);
            data.push_back(normals[i].y);
            data.push_back(normals[i].z);
        }
    }


    GLuint sphereVao;
    glGenVertexArrays(1,&sphereVao);
    glBindVertexArray(sphereVao);
    GLuint sphereVbo;
    glGenBuffers(1,&sphereVbo);
    glBindBuffer(GL_ARRAY_BUFFER,sphereVbo);
    glBufferData(GL_ARRAY_BUFFER,data.size() * sizeof(float),&data[0],GL_STATIC_DRAW);
    GLuint sphereEbo;
    glGenBuffers(1,&sphereEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sphereEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(int),&indices[0],GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(GLfloat),(void*)0); 
    glEnableVertexAttribArray(1); 
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,8 * sizeof(GLfloat),(void*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,8 * sizeof(GLfloat),(void*)(5*sizeof(GLfloat)));

        // lights
    // ------
    Vector3 lightPositions[] = {
        Vector3(-10.0f,  10.0f, 10.0f),
        Vector3( 10.0f,  10.0f, 10.0f),
        Vector3(-10.0f, -10.0f, 10.0f),
        Vector3( 10.0f, -10.0f, 10.0f),
    };
    Vector3 lightColors[] = {
        Vector3(300.0f, 300.0f, 300.0f),
        Vector3(300.0f, 300.0f, 300.0f),
        Vector3(300.0f, 300.0f, 300.0f),
        Vector3(300.0f, 300.0f, 300.0f)
    };

    shared_ptr<Shader> sh = make_shared<Shader>("./pbr1.1.vert","./pbr1.1.frag");
    sh->Use();
    Matrix4x4 model = Matrix4x4::Identity();
    sh->SetMatrix("model",model.Get());
    sh->SetMatrix("view",cam.getMatrix().Get());
    Matrix4x4 projection = Perspective(RADIAN(60.0f),(float)kWidth / kHeight,0.01f,100.0f);
    sh->SetMatrix("projection",projection.Get());
    sh->SetVec3("albedo",0.5,0.0,0.0);
    sh->SetFloat("ao",1.0);


    shared_ptr<Shader> dd = make_shared<Shader>("dd.vert","dd.frag");
    dd->Use();
    dd->SetMatrix("model",model.Get());
    dd->SetMatrix("view",cam.getMatrix().Get());
    dd->SetMatrix("projection",projection.Get());

    const int nrRows = 7;
    const int nrCols = 7;
    const float spacing = 2.5;

glCheckError();
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(sphereVao);
        sh->Use();
        sh->SetVec3("camPos",cam.GetPosition().x,cam.GetPosition().y,cam.GetPosition().z);
        sh->SetMatrix("view",cam.getMatrix().Get());
        for(int i=0;i<nrRows;++i)
        {
            sh->SetFloat("metallic",(float)i/nrRows);
            for(int j=0;j<nrCols;++j)
            {
                sh->SetFloat("roughness",max(min((float)j/nrCols,1.0f),0.5f));

                Matrix4x4 model = Matrix4x4::Identity();
                model.Translate((j-(nrCols)/2) * spacing,(i-(nrRows/2))* spacing, 0.0);
                sh->SetMatrix("model",model.Get());
                glDrawElements(GL_TRIANGLE_STRIP,indicesCount,GL_UNSIGNED_INT, 0);
            }
        }

        for(int i=0;i<sizeof(lightPositions) / sizeof(lightPositions[0]);++i)
        {
            Vector3 newPos = lightPositions[i] + Vector3(sin(glfwGetTime() * 5.0) * 5.0,0.0,0.0);
            newPos = lightPositions[i];
            sh->SetVec3("lightPositions["+to_string(i) + "]",newPos.x,newPos.y,newPos.z);
            sh->SetVec3("lightColors["+to_string(i) + "]",lightColors[i].x,lightColors[i].y,lightColors[i].z);

            Matrix4x4 model = Matrix4x4::Identity();
            model.Translate(newPos.x,newPos.y,newPos.z);
            model.Scale(0.5f);
            sh->SetMatrix("model",model.Get());
            glDrawElements(GL_TRIANGLE_STRIP,indicesCount,GL_UNSIGNED_INT, 0);

        }

    //     dd->Use();
    //    glDrawElements(GL_TRIANGLE_STRIP,indicesCount,GL_UNSIGNED_INT, 0); 

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void frame_size_callback(GLFWwindow *window,int width,int height)
{
    glViewport(0,0,width,kHeight);
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