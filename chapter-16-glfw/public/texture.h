#ifndef __TEXTURE_HEAD__
#define __TEXTURE_HEAD__

#include <glad/glad.h>
#include <memory>
#include <stdio.h>

namespace luwu
{

class Texture2D
{
	public:
		Texture2D(const char *path);
		Texture2D(const Texture2D &other) = delete;
		void operator=(const Texture2D &other) = delete;
		~Texture2D()
		{
			if(tex_ > 0)
			{
				printf("delete texture");
				glDeleteTextures(1,&tex_);
			}
		}

		GLuint Get()
		{
			return tex_;
		}
			
		static GLuint LoadTexture(const char *path,bool flip = true);
		static std::shared_ptr<Texture2D> LoadTexture(const unsigned char *buffer,int width,int height, GLenum format);

	private:
		Texture2D(GLuint tex):tex_(tex){}
		GLuint tex_;
};

}

#endif
