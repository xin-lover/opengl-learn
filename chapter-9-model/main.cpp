#include <stdio.h>
#include <memory>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "../public/shader.h"
#include "../public/model.h"
#include "../public/threemath.h"
#include "../public/program_util.h"

//model
std::shared_ptr<Model> ourmodel;
//shader
std::shared_ptr<Shader> ourshader;

Vector3 lastV;
Matrix4x4 modelMat;
GLint modelLoc;

GLuint vao;
GLuint vbo;
GLuint ibo;

void Display();
void MouseHandle(int x,int y);
void MouseUpHandle(int button, int state, int x, int y);
GLuint LoadImage(const char* path);


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

	ourmodel = std::shared_ptr<Model>(new Model("./nanosuit/nanosuit.obj"));
	ourshader = std::shared_ptr<Shader>(new Shader("./model.vert","./model.frag"));
	ourshader->Use();
	modelMat = Matrix4x4::Identity();
	//modelMat.Rotate(Vector3(1,0,0),30);
	ourshader->SetMatrix("model",modelMat.Get());
	
	Matrix4x4 viewMat = Matrix4x4::Identity();
	viewMat.Translate(Vector3(0,-6,-5));
	ourshader->SetMatrix("view",viewMat.Get());
	
	Matrix4x4 projMat = Ortho(-10,10,-10,10,0,20);
	ourshader->SetMatrix("proj",projMat.Get());

	/*glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	float vertices[12]={
		0,0,0,
		1,0,0,
		1,1,0,
		0,1,0
	};
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

	unsigned int indices[6]={
		0,1,2,2,3,0
	};

	glGenBuffers(1,&ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(GL_FLOAT),(GLvoid*)0);
	glEnableVertexAttribArray(0);*/

	glutDisplayFunc(Display);
	glutMotionFunc(MouseHandle);
	glutMouseFunc(MouseUpHandle);
	glutMainLoop();

	return 0;
}
	
void Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	ourshader->Use();
	ourshader->SetMatrix("model",modelMat.Get());
	ourmodel->Draw(ourshader);
	//glDrawElements (GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
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

