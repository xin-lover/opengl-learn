#include "texture.h"
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../public/stb_image.h"

using namespace luwu;

Texture2D::Texture2D(const char *path)
{
	tex_ = LoadTexture(path);
}

GLuint Texture2D::LoadTexture(const char *path,bool flip)
{
	unsigned int textureID;
	glGenTextures(1,&textureID);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(flip);
	unsigned char *data = stbi_load(path, &width, &height,&nrComponents,0);
	if(data == NULL)
	{
		Logger::Error("load %s fail..\n",path);
		stbi_image_free(data);
		return 0;
	}

	Logger::Error("nComponents:%d\n",nrComponents);
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
		Logger::Error("error texture format");
	}

	glBindTexture(GL_TEXTURE_2D,textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	return textureID;
}

std::shared_ptr<Texture2D> Texture2D::LoadTexture(const unsigned char *data,int width,int height,GLenum format)
{
	unsigned int textureID;
	glGenTextures(1,&textureID);

	glBindTexture(GL_TEXTURE_2D,textureID);
	glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	return std::shared_ptr<Texture2D>(new Texture2D(textureID));
}

std::shared_ptr<Texture2D> Texture2D::LoadTexture(const unsigned char *data,int width,int height,GLenum innerFormat, GLenum outerFormat)
{
	unsigned int textureID;
	glGenTextures(1,&textureID);

	glBindTexture(GL_TEXTURE_2D,textureID);
	glTexImage2D(GL_TEXTURE_2D,0,innerFormat,width,height,0,outerFormat,GL_UNSIGNED_BYTE,data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	return std::shared_ptr<Texture2D>(new Texture2D(textureID));
}

GLuint Texture2D::LoadCubmap(const std::vector<std::string> &faces)
{
	GLuint sktex;
    glGenTextures(1,&sktex);
    glBindTexture(GL_TEXTURE_CUBE_MAP,sktex);
	stbi_set_flip_vertically_on_load(false);
    for(int i=0;i<faces.size();++i)
    {
        int width,height,nrComponents;
        unsigned char *data = stbi_load(faces[i].c_str(),&width,&height,&nrComponents,0);
        if(data != nullptr)
        {
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data);
        }
        else{
            Logger::Error("load texture fail..path:%s\n",faces[i].c_str());
        }
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);

	return sktex;
}