#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../public/camera.h"
#include "../public/texture.h"
#include "../public/shader.h"

using namespace luwu;

using namespace std;

const int kWidth=640;
const int kHeight = 480;

bool firstMouse = true;
double lastPos_x,lastPos_y;

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

void frame_size_callback(GLFWwindow* window,int width,int height)
{
    glViewport(0,0,width,height);
}

void mouse_callback(GLFWwindow* window,double x,double y)
{
    if(firstMouse)
    {
        firstMouse = false;
        lastPos_x = x;
        lastPos_y = y;
    }

    float xOffset = x- lastPos_x;
    float yOffset = lastPos_y-y;
    lastPos_x = x;
    lastPos_y = y;
    cam.ProcessMouseMovement(xOffset,yOffset);
}

void RenderCube();

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,true);
    #endif

    GLFWwindow* window = glfwCreateWindow(kWidth,kHeight,"irradiance conversion",nullptr,nullptr);
    glfwMakeContextCurrent(window);
    if(window == nullptr)
    {
        printf("glfw create window fail.\n");
        glfwTerminate();
        return -1;
    }
    glfwSetWindowSizeCallback(window,frame_size_callback);
    glfwSetCursorPosCallback(window,mouse_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == -1)
    {
        printf("load glad gl loader fail..\n");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

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

    GLuint sphereVAO;
    glGenVertexArrays(1,&sphereVAO);
    glBindVertexArray(sphereVAO);
    GLuint sphereVBO;
    glGenBuffers(1,&sphereVBO);
    glBindBuffer(GL_ARRAY_BUFFER,sphereVBO);
    glBufferData(GL_ARRAY_BUFFER,data.size()*sizeof(float),&(data[0]),GL_STATIC_DRAW);
    GLuint sphereEBO;
    glGenBuffers(1,&sphereEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(int),&(indices[0]),GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,8*sizeof(GL_FLOAT),(void*)(3*sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,8*sizeof(GL_FLOAT),(void*)(5*sizeof(GL_FLOAT)));

    shared_ptr<Shader> sh = make_shared<Shader>("pass.vert","irradiance.frag");
    sh->Use();

    Matrix4x4 model = Matrix4x4::Identity();
    Matrix4x4 project = Perspective(RADIAN(60.0f),(float)kWidth/kHeight,0.1f,100.0f);
    sh->SetMatrix("model",model.Get());
    sh->SetMatrix("view",cam.getMatrix().Get());
    sh->SetMatrix("projection",project.Get());

    shared_ptr<Shader> background_sh = make_shared<Shader>("background.vert","background.frag");
    background_sh->Use();
    background_sh->SetMatrix("model",model.Get());
    background_sh->SetMatrix("view",cam.getMatrix().Get());
    background_sh->SetMatrix("projection",project.Get());

    float deltaTime,lastTime=glfwGetTime();
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
        glClearColor(0.0,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sh->Use();
        sh->SetMatrix("view",cam.getMatrix().Get());
        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLE_STRIP,indicesCount,GL_UNSIGNED_INT,0);
        // glCheckError();

        background_sh->Use();
        background_sh->SetMatrix("view",cam.getMatrix().Get());
        RenderCube();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

unsigned int cubeVAO=0;
unsigned int cubeVBO=0;

void RenderCube()
{
    if(cubeVAO == 0)
    {
        glGenVertexArrays(1,&cubeVAO);
        glBindVertexArray(cubeVAO);
        
        glGenBuffers(1,&cubeVBO);
        glBindBuffer(GL_ARRAY_BUFFER,cubeVBO);
        
        float vertices[]={
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

        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GL_FLOAT),(void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GL_FLOAT),(void*)(3*sizeof(GL_FLOAT)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(GL_FLOAT),(void*)(6*sizeof(GL_FLOAT)));
    }

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES,0,36);
    glBindVertexArray(0);
}