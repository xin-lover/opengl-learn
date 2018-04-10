#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "../public/ShaderHelper.h"
#include "../public/threemath.h"

GLuint vao1;
GLuint vbo1;

GLuint prog;

Vector3 lastV;
Matrix4x4 modelMat;
GLint modelLoc;

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	glBindVertexArray(vao1);
	glUniformMatrix4fv(modelLoc,1,GL_FALSE,modelMat.Get());
	glDrawArrays(GL_TRIANGLES,0,36);

	glBindVertexArray(0);
	glFlush();
}


void MouseHandle(int x, int y)
{
	Float fx = (x/640.0f - 0.5) * 2;
	Float fy = (y/480.0f - 0.5) * 2 * -1;
	Float x2addy2=fx * fx + fy * fy;
	Float z = 0;
	if(x2addy2 < 1)
	{
		z = sqrt(1 - fx * fx - fy * fy);
	}
	
	//printf("x:%f y:%f z:%f\n", fx, fy, z);

	if(lastV.x == 0 && lastV.y == 0 && lastV.z == 0)
	{
		lastV = Vector3(fx,fy,z);
		return;
	}

	Vector3 curV(fx,fy,z);
	Vector3 axis = Vector3::Cross(lastV,curV);
	axis.Normalized();

	Float c = Vector3::Dot(lastV, curV);
	if(c > 1)
	{
		c = 1;
	}

	if(c < -1)
	{
		c = -1;
	}

	Float theta = acos(c) * 180;

	//printf("theta:%f\n",theta);
	Matrix4x4 m = Matrix4x4::Identity();
	m.Rotate(axis,theta);
	modelMat *= m;
	lastV = curV;
	glutPostRedisplay();
}

void MouseUpHandle(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_UP)
		{
			lastV = Vector3(0,0,0);
		}

		if(state == GLUT_DOWN)
		{

			Float fx = (x/640.0f - 0.5) * 2;
			Float fy = (y/480.0f - 0.5) * 2 * -1;
			Float z = sqrt(1 - fx * fx - fy * fy);

			lastV = Vector3(fx,fy,z);
		}
	}
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

	Float vertices[] = 
	{
		//front
		-0.5, 0.5, 0.5,		0.0f,0.0f,1.0,
		-0.5,-0.5, 0.5,		0.0f,0.0f,1.0,
		0.5, -0.5, 0.5,		0.0f,0.0f,1.0,

		0.5, -0.5, 0.5,		0.0f,0.0f,1.0,
		0.5,  0.5, 0.5,		0.0f,0.0f,1.0,
		-0.5,  0.5, 0.5,	0.0f,0.0f,1.0,

		//back
		-0.5, 0.5, -0.5,	0.0f,0.0f,-1.0,
		0.5,  0.5, -0.5,	0.0f,0.0f,-1.0,
		0.5,  -0.5, -0.5, 	0.0f,0.0f,-1.0,

		0.5,  -0.5, -0.5,	0.0f,0.0f,-1.0,
		-0.5, -0.5, -0.5,	0.0f,0.0f,-1.0,
		-0.5,  0.5, -0.5,	0.0f,0.0f,-1.0,

		//left	
		-0.5,  0.5, -0.5, 	-1.0f,0.0f,0.0,	
		-0.5,  -0.5,-0.5,	-1.0f,0.0f,0.0,
		-0.5,  -0.5, 0.5,	-1.0f,0.0f,0.0,

		-0.5,  -0.5, 0.5,	-1.0f,0.0f,0.0,
		-0.5,  0.5,  0.5,	-1.0f,0.0f,0.0,
		-0.5,  0.5, -0.5,	-1.0f,0.0f,0.0,

		//right
		0.5,  0.5, -0.5,	1.0f,0.0f,0.0,
		0.5,  0.5,  0.5,	1.0f,0.0f,0.0,
		0.5, -0.5,  0.5,	1.0f,0.0f,0.0,

		0.5, -0.5,  0.5,	1.0f,0.0f,0.0,
		0.5, -0.5, -0.5,	1.0f,0.0f,0.0,
		0.5,  0.5, -0.5,	1.0f,0.0f,0.0,

		//up	
		 0.5, 0.5, -0.5,	0.0f,1.0f,0.0,
		-0.5, 0.5, -0.5,	0.0f,1.0f,0.0,
		-0.5, 0.5,  0.5,	0.0f,1.0f,0.0,

		-0.5, 0.5,  0.5,	0.0f,1.0f,0.0,
		 0.5, 0.5,  0.5,	0.0f,1.0f,0.0,
		 0.5, 0.5, -0.5,	0.0f,1.0f,0.0,

		//bottom
		0.5,  -0.5, -0.5,	0.0f,-1.0f,0.0,
		0.5,  -0.5,  0.5,	0.0f,-1.0f,0.0,
		-0.5, -0.5,  0.5,	0.0f,-1.0f,0.0,

		-0.5, -0.5,  0.5,	0.0f,-1.0f,0.0,
		-0.5, -0.5, -0.5,	0.0f,-1.0f,0.0,
		 0.5, -0.5, -0.5,	0.0f,-1.0f,0.0,
	};

	glGenVertexArrays(1,&vao1);
	glBindVertexArray(vao1);

	glGenBuffers(1,&vbo1);
	glBindBuffer(GL_ARRAY_BUFFER,vbo1);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GL_FLOAT),(GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GL_FLOAT),(GLvoid*)(3*sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	//shader
	ShaderInfo si[]={
		{GL_VERTEX_SHADER,"spot.vert"},
		{GL_FRAGMENT_SHADER,"spot.frag"},
		{GL_NONE,""}
	};
	prog = LoadShaders(si);
	glUseProgram(prog);

	modelMat = Matrix4x4::Identity();
	//modelMat.Rotate(Vector3(1,0,0),30);
	modelMat.Rotate(Vector3(0,1,0),30);
	modelLoc= glGetUniformLocation(prog,"model");
	if(modelLoc == -1)
	{
		printf("can't find mvp in shader.\n");
	}
	glUniformMatrix4fv(modelLoc,1,GL_FALSE,modelMat.Get());

	Matrix4x4 viewMat = Matrix4x4::Identity();
	viewMat.Translate(Vector3(0,0,-2));
	GLint viewLoc = glGetUniformLocation(prog,"view");
	glUniformMatrix4fv(viewLoc,1,GL_FALSE,viewMat.Get());
	
	Matrix4x4 projMat = Ortho(-2,2,-2,2,0,10);
	GLint projLoc = glGetUniformLocation(prog,"proj");
	glUniformMatrix4fv(projLoc,1,GL_FALSE,projMat.Get());

	Float color[3] = {1.0,1.0,1.0};
	GLint ambientLoc = glGetUniformLocation(prog,"ambient");
	glUniform3fv(ambientLoc,1,color);

	GLint normalMatLoc = glGetUniformLocation(prog,"normalMat");
	glUniformMatrix4fv(normalMatLoc,1,GL_FALSE,modelMat.Get());

	Float lightColor[3] = {1.0,1.0,1.0};
	GLint lgColorLoc = glGetUniformLocation(prog,"lightColor");
	glUniform3fv(lgColorLoc,1,lightColor);

	GLint lgDirLoc = glGetUniformLocation(prog,"lightPos");
	glUniform3f(lgDirLoc,0,0.3,5);

	GLint viewPosLoc= glGetUniformLocation(prog,"viewPos");
	glUniform3f(viewPosLoc,0,0,2);

	GLint shininessLoc = glGetUniformLocation(prog,"shininess");
	glUniform1f(shininessLoc,20);

	GLint strengthLoc = glGetUniformLocation(prog,"strength");
	glUniform1f(strengthLoc,0.5);

	GLint ctLoc = glGetUniformLocation(prog,"constantAttenuation");
	glUniform1f(ctLoc,1);

	GLint ltLoc = glGetUniformLocation(prog,"linearAttenuation");
	printf("ltLoc:%d\n",ltLoc);
	glUniform1f(ltLoc,0.09f);

	GLint qtLoc = glGetUniformLocation(prog,"quadraticAttenuation");
	glUniform1f(qtLoc,0.032f);

	GLint coneLoc = glGetUniformLocation(prog,"coneDir");
	glUniform3f(coneLoc,0,0,-1);

	GLint cutoffLoc = glGetUniformLocation(prog,"spotCosCutoff");
	printf("spotCutoffLoc:%d\n",cutoffLoc);
	glUniform1f(cutoffLoc,cos(12.5f / 180));

	GLint exponentLoc = glGetUniformLocation(prog,"spotExponent");
	printf("exponentLoc:%d\n",exponentLoc);
	glUniform1f(exponentLoc,1);

	glutDisplayFunc(Display);
	glutMotionFunc(MouseHandle);
	glutMouseFunc(MouseUpHandle);
	glutMainLoop();

	return 0;
}
	
