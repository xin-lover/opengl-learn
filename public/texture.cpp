#include "texture.h"
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../public/stb_image.h"

using namespace luwu;

Texture2D::Texture2D(const char *path)
{
	tex_ = LoadTexture(path);
}

GLuint Texture2D::LoadTexture(const char *path)
{
	unsigned int textureID;
	glGenTextures(1,&textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height,&nrComponents,0);
	if(data == NULL)
	{
		printf("load %s fail..\n",path);
		stbi_image_free(data);
		return 0;
	}

	printf("nComponents:%d\n",nrComponents);
	GLenum format;
	if(nrComponents ==  1)
	{
		format = GL_RED;
	}
	else if(nrComponents == 3)
	{
		format = GL_RGB;
	}
	else if(nrComponents == 4)
	{
		format = GL_RGBA;
	}
	else
	{
		printf("error texture format");
	}

	glBindTexture(GL_TEXTURE_2D,textureID);
	glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	stbi_image_free(data);
	return textureID;
}
