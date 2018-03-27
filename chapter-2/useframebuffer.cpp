#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include "ShaderHelper.h"

#define BUFFER_OFFSET(n) ((GLvoid*)n)
enum VAO_IDs { Triangles,NumVAOs};
enum Buffers_IDs { ArrayBuffer, NumBuffers };

enum Attrib_IDs { vPosition=0 };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices=6;

void init();
void display();

int main(int argc,char* argv[])
{
	glutInit(&argc,argv);
	glutInitContextVersion(3,3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(300,300);
	glutCreateWindow("Frame Buffer");

/*	const GLubyte* name = glGetString(GL_VENDOR); //返回负责当前OpenGL实现厂商的名字
	const GLubyte* biaoshifu = glGetString(GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台
	const GLubyte* OpenGLVersion =glGetString(GL_VERSION); //返回当前OpenGL实现的版本号
	const GLubyte* gluVersion= gluGetString(GLU_VERSION); //返回当前GLU工具库版本
	printf("OpenGL实现厂商的名字：%s\n", name);
	printf("渲染器标识符：%s\n", biaoshifu);
	printf("OOpenGL实现的版本号：%s\n",OpenGLVersion );
	printf("OGLU工具库版本：%s\n", gluVersion);*/ 
	glewExperimental=GL_TRUE;
	glewInit();
	init();
	glutDisplayFunc(display);

	glutMainLoop();
	return 0;
}

void init()
{
	glClearColor(0.0,0.0,0.0,1.0);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-5,5,-5,5,5,15);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0,0,10,0,0,0,0,1,0);

	glGenVertexArrays(NumVAOs,VAOs);
	glBindVertexArray(VAOs[Triangles]);

	GLfloat vertices[NumVertices][2]={
		{-0.90,-0.90},
		{0.85, -0.90},
		{ -0.90, 0.85 },
		{ 0.90, -0.85 },
		{ 0.90, 0.90 },
		{-0.85, 0.90 },
	};

	glGenBuffers(NumBuffers,Buffers);
	glBindBuffer(GL_ARRAY_BUFFER,Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

	ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER,"triangles.vert"},
		{GL_FRAGMENT_SHADER,"triangles.frag"},
		{GL_NONE,NULL},
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);

	glVertexAttribPointer(vPosition,2,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(0));

	glEnableVertexAttribArray(vPosition);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(VAOs[Triangles]);
	glDrawArrays(GL_TRIANGLES,0,NumVertices);

	glFlush();
}

