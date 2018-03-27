#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ShaderHelper.h"
#include "threemath.h"

GLuint cubeVAO;
GLuint cubeBuf;
GLuint prog;
GLint mv; 
GLint pMat;
Matrix4x4 mat;
Matrix4x4 projMat;

Vector3 lastV;

void PrintMat(float *vv)
{

	for(int i=0;i < 16; ++i)
	{
		printf("%f ",vv[i]);
		if((i + 1) % 4 == 0)
		{
			printf("\n");
		}
	}
}

int init()
{
	glClearColor(0.0,0.0,0.0,1.0);
	//glMatrixMode(GL_PROJECTION);
	//glOrtho(-5,5,-5,5,5,15);
	//glFrustum(-5,5,-5,5,0.3,15);
	//glMatrixMode(GL_MODELVIEW);
	//gluLookAt(0,0,10,0,2,0,0,1,0);

	Quaternion q1(1,-1,2,3);
	Quaternion q2(-1,2,0.5,1);
	Quaternion q = q1 * q2;
	printf("Quaternion test:%f,%f,%f,%f\n",q.x,q.y,q.z,q.w);

	glEnable(GL_CULL_FACE);
	glGenVertexArrays(1,&cubeVAO);
	glBindVertexArray(cubeVAO);

	glGenBuffers(1,&cubeBuf);
	glBindBuffer(GL_ARRAY_BUFFER,cubeBuf);

	Float vertices[] = 
	{
		//front
		-0.5,0.5,0.5,
		-0.5,-0.5,0.5,
		0.5,0.5,0.5,

		-0.5,-0.5,0.5,
		0.5,-0.5,0.5,
		0.5,0.5,0.5,

		//back
		0.5,0.5,-0.5,
		0.5,-0.5,-0.5,
		-0.5,-0.5,-0.5,

		-0.5,-0.5,-0.5,
		-0.5,0.5,-0.5,
		0.5,0.5,-0.5,

		//left	
		-0.5,0.5,0.5,
		-0.5,0.5,-0.5,
		-0.5,-0.5,-0.5,

		-0.5,-0.5,-0.5,
		-0.5,-0.5,0.5,
		-0.5,0.5,0.5,

		//right
		0.5,-0.5,0.5,
		0.5,-0.5,-0.5,
		0.5,0.5,-0.5,

		0.5,0.5,-0.5,
		0.5,0.5,0.5,
		0.5,-0.5,0.5,

		//up	
		-0.5,0.5,0.5,
		0.5,0.5,0.5,
		0.5,0.5,-0.5,

		0.5,0.5,-0.5,
		-0.5,0.5,-0.5,
		-0.5,0.5,0.5,

		//bottom
		0.5,-0.5,0.5,
		-0.5,-0.5,0.5,
		-0.5,-0.5,-0.5,

		-0.5,-0.5,-0.5,
		0.5,-0.5,-0.5,
		0.5,-0.5,0.5,
	};
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

	//load shader
	ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER,"triangles.vert"},
		{GL_FRAGMENT_SHADER,"triangles.frag"},
		{GL_NONE,""}
	};

	prog = LoadShaders(shaders);
	glUseProgram(prog);

	mat=Matrix4x4::Identity();
	mat.Translate(Vector3(0,0,-4));
	//mat.Translate(Vector3(1,0.5,0 ));
	//mat.Rotate(Vector3(0,1,0),50);

	//projMat = Frustum(-5,5,-5,5,3,10);
	projMat = Ortho(-5,5,-5,5,1,10);
	//mat.Rotate(Vector3(1,0,0),30);

	PrintMat(mat.Get());
	
	mv = glGetUniformLocation(prog,"ModelViewMatrix");
	if(mv == -1)
	{
		printf("not find uniform ModelViewMatrix\n");
	}
	glUniformMatrix4fv(mv,1,GL_FALSE,mat.Get());	

	pMat = glGetUniformLocation(prog, "ProjectionMatrix");
	if(pMat == -1)
	{
		printf("not find ProjectionMatrix\n");
	}
	glUniformMatrix4fv(pMat,1,GL_FALSE,projMat.Get());

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	return 0;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(cubeVAO);

	glUseProgram(prog);
	glUniformMatrix4fv(mv,1,GL_FALSE,mat.Get());
	glUniformMatrix4fv(pMat,1,GL_FALSE,projMat.Get());
	glDrawArrays(GL_TRIANGLES,0,36);
	glFlush();
}

void Idle()
{
	//mat.Rotate(Vector3(0,1,0),0.001);
	//glutPostRedisplay();
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
	mat.Translate(Vector3(0,0,4));
	mat *= m;
	mat.Translate(Vector3(0,0,-4));
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

void KeyPressHandle(unsigned char key, int x, int y)
{
	if(key == 'p')
	{
		mat.Translate(Vector3(0,0,4));
		mat.Rotate(Vector3(0,1,0),1);
		mat.Translate (Vector3(0,0,-4));
		glutPostRedisplay();
	}

	if(key == 'o')
	{
		mat.Translate(Vector3(0,0,4));
		mat.Rotate(Vector3(0,1,0),-1);
		mat.Translate (Vector3(0,0,-4));
		glutPostRedisplay();
	}
	
	if(key == 'k')
	{
		mat.Translate(Vector3(0,0,4));
		mat.Rotate(Vector3(1,0,0),1);
		mat.Translate (Vector3(0,0,-4));
		glutPostRedisplay();
	}

	if(key == 'l')
	{
		mat.Translate(Vector3(0,0,4));
		mat.Rotate(Vector3(1,0,0),-1);
		mat.Translate (Vector3(0,0,-4));
		glutPostRedisplay();
	}

	if(key == 'a')
	{
		mat.Translate(Vector3(0.01,0,0));
		glutPostRedisplay();
	}

	if(key == 'd')
	{
		mat.Translate(Vector3(-0.01,0,0));
		glutPostRedisplay();
	}

	if(key == 'w')
	{
		mat.Translate(Vector3(0,0,-0.01));
		glutPostRedisplay();
	}

	if(key == 's')
	{
		mat.Translate(Vector3(0,0,0.01));
		glutPostRedisplay();
	}

	if(key == 't')
	{
		projMat = Ortho(-5,5,-5,5,1,10);
		glutPostRedisplay();
	}

	if(key == 'r')
	{
		projMat = Frustum(-5,5,-5,5,3,10);
		glutPostRedisplay();
	}

	
}

int main(int argc, char* argv[])
{
	glutInit(&argc,argv);
	glutInitContextVersion(3,3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(600,480);
	glutCreateWindow("Cube");

	glewExperimental = GL_TRUE;
	GLint res = glewInit();
	if(res != GLEW_OK)
	{
		printf("glew init fail..\n");
		exit(1);
	}


	if(init() != 0)
	{
		printf("init fail....\n");
		exit(1);
	}

	glutDisplayFunc(display);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyPressHandle);
	glutMouseFunc(MouseUpHandle);
	glutMotionFunc(MouseHandle);
	glutMainLoop();
	
	printf("end main");
	return 0;
}


