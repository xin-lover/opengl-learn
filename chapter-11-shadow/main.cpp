#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <memory>

#include "../public/shader.h"
#include "../public/threemath.h"
#include "../public/model.h"
#include "../public/texture.h"

bool isInit = false;
void Init();
void Display();

GLuint vao;
GLuint vbo;

GLuint rbo;
GLuint fbo;

GLuint boxVao;
GLuint boxVbo;

GLuint depthTex;

Matrix4x4 planeModelMat;
Matrix4x4 boxModelMat;
Matrix4x4 viewMat;

int width=512;
int height=512;

std::shared_ptr<Shader> tShader;
std::shared_ptr<Shader> modelShader;
std::shared_ptr<Shader> planeShader;
std::shared_ptr<Model> tModel;

GLuint tt;
GLuint LoadImage(const char *path);
void DrawScene();
void KeyRotate(unsigned char key,int x,int y);

int main(int argc, char *argv[])
{
	glutInit(&argc,argv);
	glutInitContextVersion(3,3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowSize(512,512);
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
	glutKeyboardFunc(KeyRotate);
	glutMainLoop();

	return 0;
}

void Init()
{
	//开启混合
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	
	float vertices[30] = {
		-1,0,1,	0,1,
		1,0,1,	1,1,
		1,0,-1,	1,0,

		1,0,-1,	1,0,
		-1,0,-1,0,0,
		-1,0,1,	0,1
		
	};
	/*float vertices[30] = {
		-1,1,0, 0,1,
		-1,-1,0, 0,0,
		1,-1,0,	1,0,

		1,-1,0,	1,0,
		1,1,0, 1,1,
		-1,1,0, 0,1
	};*/
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(GL_FLOAT),(GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(GL_FLOAT),(GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1,&boxVao);
	glBindVertexArray(boxVao);

	glGenBuffers(1,&boxVbo);
	glBindBuffer(GL_ARRAY_BUFFER,boxVbo);

	Float boxvertices[] = 
	{
		//front
		-0.5, 0.5, 0.5,		0.0f,0.0f,
		-0.5,-0.5, 0.5,		0.0f,1.0f,
		0.5, -0.5, 0.5,		1.0f,1.0f,

		0.5, -0.5, 0.5,		1.0f,1.0f,
		0.5,  0.5, 0.5,		1.0f,0.0f,
		-0.5,  0.5, 0.5,	0.0f,0.0f,

		//back
		-0.5, 0.5, -0.5,	0.0f,0.0f,
		0.5,  0.5, -0.5,	1.0f,0.0f,
		0.5,  -0.5, -0.5, 	1.0f,1.0f,

		0.5,  -0.5, -0.5,	1.0f,1.0f,
		-0.5, -0.5, -0.5,	0.0f,1.0f,
		-0.5,  0.5, -0.5,	0.0f,0.0f,

		//left	
		-0.5,  0.5, -0.5, 	0.0f,0.0f,	
		-0.5,  -0.5,-0.5,	1.0f,0.0f,
		-0.5,  -0.5, 0.5,	1.0f,1.0f,

		-0.5,  -0.5, 0.5,	1.0f,1.0f,
		-0.5,  0.5,  0.5,	0.0f,1.0f,
		-0.5,  0.5, -0.5,	0.0f,0.0f,

		//right
		0.5,  0.5, -0.5,	0.0f,0.0f,
		0.5,  0.5,  0.5,	1.0f,0.0f,
		0.5, -0.5,  0.5,	1.0f,1.0f,

		0.5, -0.5,  0.5,	1.0f,1.0f,
		0.5, -0.5, -0.5,	0.0f,1.0f,
		0.5,  0.5, -0.5,	0.0f,0.0f,

		//up	
		 0.5, 0.5, -0.5,	0.0f,0.0f,
		-0.5, 0.5, -0.5,	0.0f,1.0f,
		-0.5, 0.5,  0.5,	1.0f,1.0f,

		-0.5, 0.5,  0.5,	1.0f,1.0f,
		 0.5, 0.5,  0.5,	1.0f,0.0f,
		 0.5, 0.5, -0.5,	0.0f,0.0f,

		//bottom
		0.5,  -0.5, -0.5,	0.0f,0.0f,
		0.5,  -0.5,  0.5,	0.0f,1.0f,
		-0.5, -0.5,  0.5,	1.0f,1.0f,

		-0.5, -0.5,  0.5,	1.0f,1.0f,
		-0.5, -0.5, -0.5,	1.0f,0.0f,
		 0.5, -0.5, -0.5,	0.0f,0.0f,
	};
	glBufferData(GL_ARRAY_BUFFER,sizeof(boxvertices),boxvertices,GL_STATIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(GL_FLOAT),(GLvoid*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(GL_FLOAT),(GLvoid*)((3 * sizeof(GL_FLOAT))));
	glEnableVertexAttribArray(1);

	tShader = std::make_shared<Shader>("./shadow.vert","./shadow.frag");
	planeShader = std::make_shared<Shader>("./plane.vert","./plane.frag");
	modelShader = std::make_shared<Shader>("./model.vert","./model.frag");

	planeModelMat = Matrix4x4::Identity();
	planeModelMat.Translate(Vector3(0,-2, -3));
	planeModelMat.Scale(2.5);

	boxModelMat = Matrix4x4::Identity();
	boxModelMat.Translate(Vector3(0,0,-3));

	viewMat = Matrix4x4::Identity();
	viewMat.Translate(Vector3(-1,0,0));
	viewMat.Rotate(Vector3(0,1,0),-30);
	//viewMat.Rotate(Vector3(0,1,0),15);
	//Matrix4x4 projMat = Ortho(-2,2,-2,2,1,10);
	Matrix4x4 projMat = Frustum(-2,2,-2,2,1,10);

	Matrix4x4 othProjMat = Ortho(-2,2,-2,2,1,10);
	//Matrix4x4 othProjMat =  Frustum(-2,2,-2,2,1,10);
	Matrix4x4 lightView = Matrix4x4::Identity();
	lightView.Translate(Vector3(0,-2.2,2.1));
	lightView.Rotate(Vector3(1,0,0),90);
	//lightView.Rotate(Vector3(0,1,0),-30);
	Matrix4x4 lightSpace = lightView * othProjMat;// othProjMat * lightView;

	tShader->Use();
	tShader->SetMatrix("lightSpace",lightSpace.Get());
	//tShader->SetMatrix("proj",projMat.Get());

	planeShader->Use();
	planeShader->SetMatrix("view",viewMat.Get());
	planeShader->SetMatrix("proj",projMat.Get());
	planeShader->SetMatrix("lightView",lightSpace.Get());

	float *a = lightSpace.Get();
	/*for(int i=0;i < 16; i++)
	{
		printf("%f  ",a[i]);
		if(i % 4 == 3)
		{
			printf("\n");
		}
	}*/

	modelShader->Use();
	modelShader->SetMatrix("view",viewMat.Get());
	modelShader->SetMatrix("proj",projMat.Get());


	//创建帧缓存
	glGenFramebuffers(1,&fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbo);

	//创建深度纹理
	glGenTextures(1,&depthTex);
	glBindTexture(GL_TEXTURE_2D,depthTex);

	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,width,height,0,GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC,GL_LEQUAL);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);


	//绑定
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depthTex,0);
	
	//禁止渲染颜色
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	//load model
	tModel = std::make_shared<Model>("./nanosuit/nanosuit.obj");

	tt = luwu::Texture2D::LoadTexture("./grass.png");
}

void Display()
{
	if(!isInit)
	{
		isInit = true;
		Init();
	}

	//渲染阴影贴图
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbo);
	glViewport(0,0,width,height);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//打开多边形偏移，以避免深度数据的zfighting问题
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.0f,4.0f);

	//渲染
	tShader->Use();
	tShader->SetMatrix("model",boxModelMat.Get());
	glBindVertexArray(boxVao);
	glDrawArrays(GL_TRIANGLES,0,36);
	tShader->SetMatrix("model",planeModelMat.Get());
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES,0,6);

	glDisable(GL_POLYGON_OFFSET_FILL);

	//回到摄像机视角
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glViewport(0,0,width,height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//绘制场景
	planeShader->Use();
	glBindTexture(GL_TEXTURE_2D,tt);
	planeShader->SetMatrix("model",boxModelMat.Get());
	glBindVertexArray(boxVao);
	glDrawArrays(GL_TRIANGLES,0,36);
	planeShader->Use();
	glBindTexture(GL_TEXTURE_2D,depthTex);
	modelShader->SetMatrix("model",planeModelMat.Get());
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES,0,6);
	glBindTexture(GL_TEXTURE_2D,0);

	glFlush();
}

void KeyRotate(unsigned char key, int x, int y)
{
	if(key == 'l')
	{
		viewMat.Rotate(Vector3(0,1,0),1);
		glutPostRedisplay();
	}
	if(key == 'k')
	{
		viewMat.Rotate(Vector3(0,1,0),-1);
		glutPostRedisplay();
	}

	if(key == 'f')
	{
		viewMat.Rotate(Vector3(1,0,0),-1);
		glutPostRedisplay();
	}
	if(key == 'd')
	{
		viewMat.Rotate(Vector3(1,0,0),1);
		glutPostRedisplay();
	}

	if(key == 'u')
	{
		planeModelMat.Scale(0.9);
		boxModelMat.Scale(0.9);
		glutPostRedisplay();
	}

	if(key == 'y')
	{
		planeModelMat.Scale(1.1);
		boxModelMat.Scale(1.1);
		glutPostRedisplay();
	}

}
