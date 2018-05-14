#ifndef __TEXTURE_HEAD__
#define __TEXTURE_HEAD__


#include <GL/glew.h>
//#include <GL/gl.h>

namespace luwu
{

class Texture2D
{
	public:
		Texture2D(const char *path);
		//Texture2D(const Texture2D &other) = delete;
		//void operator=(const Texture2D &other) = delete;
		~Texture2D();

		GLuint Get()
		{
			return tex_;
		}
			
		static GLuint LoadTexture(const char *path);

	private:
		GLuint tex_;
};

}

#endif
