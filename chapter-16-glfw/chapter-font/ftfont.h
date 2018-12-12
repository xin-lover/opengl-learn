#ifndef __FTFONT_HEAD__
#define __FTFONT_HEAD__
#include <ft2build.h>
#include FT_FREETYPE_H

namespace luwu
{

struct Bitmap
{
	unsigned char* data;
	int width;
	int height;
};

class FTFont
{
	public:
		FTFont(const char* filepath)
		{
			FT_Error error = FT_Init_FreeType(&m_library);
			if(error != FT_Err_Ok)
			{
				printf("freetype init library error\n");
			}

			error = FT_New_Face(m_library,filepath,0,&m_face);
			if(error == FT_Err_Unknown_File_Format)
			{
				printf("format error..\n");
			}
			else if(error)
			{
				printf("can't open or read font file...\n");
			}

		}

		~FTFont()
		{
			FT_Done_Face(m_face);
			FT_Done_FreeType(m_library);
			
		}

		Bitmap Load(FT_ULong charcode)
		{
			FT_Error error = FT_Set_Char_Size(m_face,0,16 * 64,300,300);
			if(error)
			{
				printf("set char size error...\n");
			}

			FT_UInt glyph_index = FT_Get_Char_Index(m_face,charcode);
			error = FT_Load_Glyph(m_face,glyph_index,FT_LOAD_DEFAULT);
			// error = FT_Load_Char(m_face,'A',FT_LOAD_RENDER);
			if(error)
			{
				printf("load glyph error...\n");
			}

			if(m_face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
			{
				printf("format not equal FT_RENDER_MODE_NORMAL.\n");
				FT_Render_Glyph(m_face->glyph,FT_RENDER_MODE_NORMAL);
			}
			FT_GlyphSlot slot = m_face->glyph;
			FT_Bitmap bm = slot->bitmap;

			Bitmap bit;
			bit.data = bm.buffer;
			bit.width = bm.width;
			bit.height = bm.rows;
			return bit;
		}

		FTFont(const FTFont& other) = delete;
		FTFont& operator=(const FTFont& other)= delete;


	private:
		FT_Library m_library;
		FT_Face m_face;
};

}
#endif
