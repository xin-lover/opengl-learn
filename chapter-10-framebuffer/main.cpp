#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <memory>

#include "../public/shader.h"
#include "../public/threemath.h"


bool isInit = false;
void Init();
void Display();

GLuint vao;
GLuint vbo;

GLuint rbo;
GLuint fbo;

int width=600;
int height=480;

std::shared_ptr<Shader> tShader;

int main(int argc, char *argv[])
{
	glutInit(&argc,argv);
	glutInitContextVersion(3,3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowSize(600,480);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("framebuffer");

	glewExperimental = GL_TRUE;
	GLint res = glewInit();
	if(res != GLEW_OK)
	{
		printf("glew init error..\n");
		exit(1);
	}

	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(Display);
	glutMainLoop();

	return 0;
}

void Init()
{
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	
	float vertices[9] = {
		0,0,0,
		1,0,0,
		1,1,0
	};
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(GL_FLOAT),(GLvoid*)0);
	glEnableVertexAttribArray(0);

	tShader = std::shared_ptr<Shader>(new Shader("./triangle.vert","./triangle.frag"));
	tShader->Use();

	Matrix4x4 modelMat = Matrix4x4::Identity();
	Matrix4x4 viewMat = Matrix4x4::Identity();
	Matrix4x4 projMat = Ortho(-2,2,-2,2,0,10);

	tShader->SetMatrix("model",modelMat.Get());
	tShader->SetMatrix("view",viewMat.Get());
	tShader->SetMatrix("proj",projMat.Get());

	glGenRenderbuffers(1,&rbo);
	glBindRenderbuffer(GL_RENDERBUFFER,rbo);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_RGBA,width,height);

	glGenFramebuffers(1,&fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbo);

	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER,rbo);

}

void Display()
{
	if(!isInit)
	{
		isInit = true;
		Init();
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbo);
	glViewport(0,0,width,height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//渲染
	tShader->Use();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES,0,3);

	glBindFramebuffer(GL_READ_FRAMEBUFFER,fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);

	glViewport(0,0,width,height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//拷贝
	glBlitFramebuffer(0,0,width,height,0,0,width,height,GL_COLOR_BUFFER_BIT,GL_NEAREST);
	glBlitFramebuffer(0,0,width,height,0,0,128,128,GL_COLOR_BUFFER_BIT,GL_NEAREST);
	glFlush();
}
