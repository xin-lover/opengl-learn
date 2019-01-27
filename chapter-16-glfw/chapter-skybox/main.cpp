#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <vector>

#include "../public/shader.h"
#include "../public/texture.h"
#include "../public/threemath.h"
#include "../public/camera.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../public/stb_image_write.h"

using namespace std;
using namespace luwu;

void framebuffer_size_callback(GLFWwindow* window,int width,int height);
void mouse_callback(GLFWwindow* window,double xPos, double yPos);

const int k_width=800;
const int k_height=600;

float lastPos_x =0.0f;
float lastPos_y = 0.0f;
bool firstMouse = true;
Camera camera(Vector3(0,0,3.0f));

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

#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(k_width,k_height,"skybox",nullptr,nullptr);
    if(window == nullptr)
    {
        printf("glfw create window fail...\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    glfwSetCursorPosCallback(window,mouse_callback);
    if(!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)))
    {
        printf("glad load gl fail\n");
        return 1;
    }

    float vertices[]={
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f 
    };

    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    float skBoxVertices[]={
               // positions          // normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    GLuint vbo;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vao);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(GL_FLOAT),(void*)0);

    GLuint cubeVao;
    glGenVertexArrays(1,&cubeVao);
    glBindVertexArray(cubeVao);
    GLuint cubeVbo;
    glGenBuffers(1,&cubeVbo);
    glBindBuffer(GL_ARRAY_BUFFER,cubeVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices),cubeVertices,GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(GL_FLOAT),(void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(GL_FLOAT),(void*)(3 * sizeof(GL_FLOAT)));

    GLuint skBoxVao;
    glGenVertexArrays(1,&skBoxVao);
    glBindVertexArray(skBoxVao);
    GLuint skBoxVbo;
    glGenBuffers(1,&skBoxVbo);
    glBindBuffer(GL_ARRAY_BUFFER,skBoxVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(skBoxVertices),skBoxVertices,GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GL_FLOAT),(void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GL_FLOAT),(void*)(3 * sizeof(GL_FLOAT)));

    GLuint skBoxVao1;
    glGenVertexArrays(1,&skBoxVao1);
    glBindVertexArray(skBoxVao1);
    GLuint skBoxVbo1;
    glGenBuffers(1,&skBoxVbo1);
    glBindBuffer(GL_ARRAY_BUFFER,skBoxVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(skBoxVertices),skBoxVertices,GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GL_FLOAT),(void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GL_FLOAT),(void*)(3 * sizeof(GL_FLOAT)));

    shared_ptr<Shader> sh = make_shared<Shader>("./skybox.vert","./skybox.frag");
    sh->Use();

    Matrix4x4 projection = Perspective(RADIAN(60.0f),(float)k_width / k_height,0.1f,100.0f);
    sh->SetMatrix("projection",projection.Get());

    shared_ptr<Shader> sh2 = make_shared<Shader>("./tex.vert","./tex.frag");
    sh2->Use();

    Matrix4x4 model = Matrix4x4::Identity();
    model.Translate(1.0f,0,-1.0f);
    // model.Rotate(Vector3(0.0f,1.0f,0.0f),30);
    sh2->SetMatrix("model",model.Get());

    // Matrix4x4 view = Matrix4x4::Identity();
    // view.Translate(0,0,-3.0f);
    sh2->SetMatrix("view",camera.getMatrix().Get());

    sh2->SetMatrix("projection",projection.Get());

    shared_ptr<Shader> sh3 = make_shared<Shader>("./envmapping.vert","./envmapping.frag");
    sh3->Use();
    model.Translate(-2.0f,0.0f,0.0f);
    sh3->SetMatrix("model",model.Get());
    sh3->SetMatrix("view",camera.getMatrix().Get());
    sh3->SetMatrix("projection",projection.Get());

    shared_ptr<Shader> sh4 = make_shared<Shader>("./refract.vert","./refract.frag");
    sh4->Use();
    Matrix4x4 model1 = Matrix4x4::Identity();
    model1.Translate(0.0f,1.0f,0.0f);
    sh4->SetMatrix("model",model1.Get());
    sh4->SetMatrix("projection",projection.Get());

    shared_ptr<Texture2D> tex = make_shared<Texture2D>("../resources/container.jpg");

    //skybox
    vector<string> faces={
        // "../resources/skybox/hw_alps/alps_rt.tga",
        // "../resources/skybox/hw_alps/alps_lf.tga",
        // "../resources/skybox/hw_alps/alps_up.tga",
        // "../resources/skybox/hw_alps/alps_dn.tga",
        // "../resources/skybox/hw_alps/alps_ft.tga",
        // "../resources/skybox/hw_alps/alps_bk.tga",

        "../resources/skybox/default/right.jpg",
        "../resources/skybox/default/left.jpg",
        "../resources/skybox/default/top.jpg",
        "../resources/skybox/default/bottom.jpg",
        "../resources/skybox/default/front.jpg",
        "../resources/skybox/default/back.jpg",

        // "../resources/skybox/mp_bleak/bleak-outlook_lf.tga",
        // "../resources/skybox/mp_bleak/bleak-outlook_rt.tga",
        // "../resources/skybox/mp_bleak/bleak-outlook_up.tga",
        // "../resources/skybox/mp_bleak/bleak-outlook_dn.tga",
        // "../resources/skybox/mp_bleak/bleak-outlook_ft.tga",
        // "../resources/skybox/mp_bleak/bleak-outlook_bk.tga",
    };
    GLuint sktex = Texture2D::LoadCubmap(faces);

    // glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    float deltaTime = 0.0f;
    float lastTime = glfwGetTime();
    // glViewport(0,0,k_width,k_height);
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

        

        glClearColor(0.2,0.3,0.3,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sh2->Use();
        sh2->SetMatrix("view",camera.getMatrix().Get());
        glBindTexture(GL_TEXTURE_2D,tex->Get());
        glBindVertexArray(cubeVao);
        glDrawArrays(GL_TRIANGLES,0,36);
        // glCheckError();

        sh3->Use();
        Vector3 camPos = camera.GetPosition();
        sh3->SetVec3("cameraPos",camPos.x,camPos.y,camPos.z);
        sh3->SetMatrix("view",camera.getMatrix().Get());
        glBindTexture(GL_TEXTURE_CUBE_MAP,sktex);
        glBindVertexArray(skBoxVao);
        glDrawArrays(GL_TRIANGLES,0,36);

        sh4->Use();
        sh4->SetVec3("cameraPos",camPos.x,camPos.y,camPos.z);
        sh4->SetMatrix("view",camera.getMatrix().Get());
        glBindTexture(GL_TEXTURE_CUBE_MAP,sktex);
        glBindVertexArray(skBoxVao1);
        glDrawArrays(GL_TRIANGLES,0,36);

        // glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        sh->Use();
        Matrix4x4 view3 = Matrix4x4(Matrix3x3(camera.getMatrix()));
        sh->SetMatrix("view",view3.Get());
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_CUBE_MAP,sktex);
        glDrawArrays(GL_TRIANGLES,0,36);
        // glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window,int width,int height)
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
    camera.ProcessMouseMovement(xOffset,yOffset);
}