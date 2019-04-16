#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>

#include "../public/shader.h"
#include "../public/texture.h"
#include "../public/threemath.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../public/stb_image_write.h"

using namespace std;
using namespace luwu;

void framebuffer_size_callback(GLFWwindow* window,int width,int height);

const int k_width=800;
const int k_height=600;

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
        printf("%s file %s line %d\n",error.c_str(),__FILE__,__LINE__);
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

    GLFWwindow *window = glfwCreateWindow(k_width,k_height,"framebuffer",nullptr,nullptr);
    if(window == nullptr)
    {
        printf("glfw create window fail...\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    if(!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)))
    {
        printf("glad load gl fail\n");
        return 1;
    }

    float vertices[]={
     // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    // Left face
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left   
    };

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };

    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    GLuint vbo;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vao);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(GL_FLOAT),(void*)(3*sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    GLuint quadVAO;
    glGenVertexArrays(1,&quadVAO);
    glBindVertexArray(quadVAO);
    GLuint quadVBO;
    glGenBuffers(1,&quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER,quadVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices),quadVertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,4 * sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,4 * sizeof(GL_FLOAT),(void*)(2 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    glCheckError();

    shared_ptr<Shader> sh = make_shared<Shader>("./framebuffer.vert","./framebuffer.frag");
    sh->Use();

    Matrix4x4 model = Matrix4x4::Identity();
    model.Translate(0.2f,0,-1.0f);
    model.Rotate(Vector3(0.0f,1.0f,0.0f),30);
    sh->SetMatrix("model",model.Get());
    Matrix4x4 view = Matrix4x4::Identity();
    view.Translate(0,0,-3.0f);
    sh->SetMatrix("view",view.Get());
    Matrix4x4 projection = Perspective(RADIAN(60.0f),(float)k_width / k_height,0.1f,100.0f);
    sh->SetMatrix("projection",projection.Get());

    shared_ptr<Texture2D> tex = make_shared<Texture2D>("../resources/container.jpg");

    shared_ptr<Shader> sh2 = make_shared<Shader>("./quad.vert","./effect.frag");
    sh2->Use();

    glEnable(GL_CULL_FACE);

    //创建缓冲
    GLuint fbo;
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_FRAMEBUFFER,fbo);

    //纹理附件
    // GLuint tt;
    // glGenTextures(1,&tt);
    // glBindTexture(GL_TEXTURE_2D,tt);

    // glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,k_width,k_height,0,GL_RGB,GL_UNSIGNED_BYTE,nullptr);

    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    // glBindTexture(GL_TEXTURE_2D,0);

    // glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,tt,0);

    //创建渲染附件
    GLuint rbo;
    glGenRenderbuffers(1,&rbo);
    glBindRenderbuffer(GL_RENDERBUFFER,rbo);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_RGBA,k_width,k_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER,rbo);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("framebuffer error.\n");
        return -1;
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    bool saved = false;
    // glViewport(0,0,k_width,k_height);
    while(!glfwWindowShouldClose(window))
    {
        if(glfwGetKey(window,GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window,true);
        }

        if(glfwGetKey(window,GLFW_KEY_J))
        {

            glViewport(100,100,800,600);
        }

        glBindFramebuffer(GL_FRAMEBUFFER,fbo);    
    glViewport(0,0,k_width,k_height);
        glClearColor(0.2,0.3,0.3,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        sh->Use();
        glBindTexture(GL_TEXTURE_2D,tex->Get());
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0,36);

        glBindFramebuffer(GL_READ_FRAMEBUFFER,fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);

        glClear(GL_COLOR_BUFFER_BIT);
        glBlitFramebuffer(0,0,k_width,k_height,0,0,k_width,k_height,GL_COLOR_BUFFER_BIT,GL_NEAREST);
        // glBlitFramebuffer(0,0,k_width,k_height,0,0,k_width/2,k_height/2,GL_COLOR_BUFFER_BIT,GL_NEAREST);
        // if(!saved)
        // {
        //     unsigned char data[k_width * 3][k_height*3];
        //     glPixelStorei(GL_PACK_ALIGNMENT,4);
        //     glReadPixels(0,0,k_width,k_height,GL_RGB,GL_UNSIGNED_BYTE,data);
        //     stbi_write_png("./abc.png",k_width,k_height,3,data,0);
        //     saved = true;
        // }
        // glBindVertexArray(0);

        // glBindFramebuffer(GL_FRAMEBUFFER,0);
        // // glViewport(0,k_height,k_width,k_height);
        // glClearColor(0.1,0.1,0.1,1.0);
        // glClear(GL_COLOR_BUFFER_BIT);

        // sh2->Use();
        // glBindTexture(GL_TEXTURE_2D,tt);
        // glBindVertexArray(quadVAO);
        // glDrawArrays(GL_TRIANGLES,0,6);

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