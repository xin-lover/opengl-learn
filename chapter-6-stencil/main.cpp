#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "../public/ShaderHelper.h"
#include "../public/threemath.h"

GLuint vao1;
GLuint vbo1;
GLuint vao2;
GLuint vbo2;

GLuint prog;

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	//模版值为1的时候绘制
	glStencilFunc(GL_EQUAL,0x1,0x1);
	glStencilOp(GL_REPLACE,GL_REPLACE,GL_REPLACE);

	GLint colorLoc = glGetUniformLocation(prog,"in_color");
	glUniform4f(colorLoc,1,0,0,1);
	glBindVertexArray(vao1);
	glDrawArrays(GL_TRIANGLES,0,3);

	//模板值不为1的时候绘制
	glStencilFunc(GL_NOTEQUAL,0x1,0x1);
	glUniform4f(colorLoc,0,1,0,1);
	glBindVertexArray(vao2);
	glDrawArrays(GL_TRIANGLES,0,3);

	glBindVertexArray(0);
	glFlush();
}

int main(int argc, char *argv[])
{
	glutInit(&argc,argv);
	glutInitContextVersion(3,3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowSize(600,480);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	glutCreateWindow("Depth");
	glutInitWindowSize(0,0);

	glewExperimental = GL_TRUE;
	GLint res = glewInit();
	if(res != GLEW_OK)
	{
		printf("init glew fail.\n");
		exit(1);
	}

	//depth and stencil apply
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glClearStencil(0x0);

	float vertices1[] = 
	{
		-1, 1, 0,
		-1, 0, 0,
		1, 0, 0
	};
	glGenVertexArrays(1,&vao1);
	glBindVertexArray(vao1);

	glGenBuffers(1,&vbo1);
	glBindBuffer(GL_ARRAY_BUFFER,vbo1);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices1),vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
	glEnableVertexAttribArray(0);

	float vertices2[] = 
	{
		-1, 0 + 0.5,  -0.5,
		-1, -1 + 0.5, -0.5,
		1, -1 + 0.5,  -0.5
	};
	glGenVertexArrays(1,&vao2);
	glBindVertexArray(vao2);

	glGenBuffers(1,&vbo2);
	glBindBuffer(GL_ARRAY_BUFFER,vbo2);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices2),vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
	glEnableVertexAttribArray(0);

	//shader
	ShaderInfo si[]={
		{GL_VERTEX_SHADER,"triangle.vert"},
		{GL_FRAGMENT_SHADER,"triangle.frag"},
		{GL_NONE,""}
	};
	prog = LoadShaders(si);
	glUseProgram(prog);

	Matrix4x4 mvp = Ortho(-1,1,-1,1,0,10);
	GLint mvpLoc = glGetUniformLocation(prog,"mvp");
	if(mvpLoc == -1)
	{
		printf("can't find mvp in shader.\n");
	}
	glUniformMatrix4fv(mvpLoc,1,GL_FALSE,mvp.Get());

	GLint colorLoc = glGetUniformLocation(prog,"in_color");
	if(colorLoc == -1)
	{
		printf("Can't find in_color in shader.\n");
	}
	glUniform4f(colorLoc,1,0,0,1);
	glutDisplayFunc(Display);
	glutMainLoop();

	return 0;
}
	
