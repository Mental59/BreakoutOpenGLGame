#include "TextRenderer.h"
#include <glad/glad.h>
#include <iostream>
#include <ft2build.h>
#include <freetype/freetype.h>
#include "Shader/ShaderProgram.h"

TextRenderer::~TextRenderer()
{
	for (const auto& textChar : mTextCharacters)
	{
		glDeleteTextures(1, &textChar.second.TextureID);
	}
}

bool TextRenderer::Init(ShaderProgram* textShader, const char* fontPath)
{
	mShader = textShader;
	InitRenderData();
	return InitCharacters(fontPath);
}

void TextRenderer::Draw(const std::string& text, glm::vec2 pos, glm::vec3 color, float scale)
{
	mShader->Bind();
	mShader->SetVec3("uTextColor", &color[0]);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glActiveTexture(GL_TEXTURE0);

	float x = pos.x;
	float y = pos.y;
	for (const auto ch : text)
	{
		TextCharacter textCh = mTextCharacters[ch];

		float xpos = x + textCh.Bearing.x * scale;
		float ypos = y + (mTextCharacters['H'].Bearing.y - textCh.Bearing.y) * scale;

		float w = textCh.Size.x * scale;
		float h = textCh.Size.y * scale;

		float vertices[4][4] = {
			{ xpos,     ypos,       0.0f, 0.0f },
			{ xpos,     ypos + h,   0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 0.0f },
			{ xpos + w, ypos + h,   1.0f, 1.0f },
		};

		glBindTexture(GL_TEXTURE_2D, textCh.TextureID);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (textCh.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}

	glBindVertexArray(0);
}

bool TextRenderer::InitCharacters(const char* fontPath)
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return false;
	}

	FT_Face face;
	if (FT_New_Face(ft, fontPath, 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return false;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		TextCharacter character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		mTextCharacters.insert(std::pair<unsigned char, TextCharacter>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderer::InitRenderData()
{
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);

	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
