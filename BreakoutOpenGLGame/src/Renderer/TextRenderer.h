#pragma once

#include "glm/glm.hpp"
#include <string>
#include <unordered_map>

class ShaderProgram;

class TextRenderer
{
public:
	struct TextCharacter
	{
		unsigned int TextureID;  // ID handle of the glyph texture
		glm::ivec2   Size;       // Size of glyph
		glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};

	~TextRenderer();

	bool Init(ShaderProgram* textShader, const char* fontPath);
	void Draw(const std::string& text, glm::vec2 pos, glm::vec3 color, float scale);

private:
	bool InitCharacters(const char* fontPath);
	void InitRenderData();

	unsigned int mVAO, mVBO;
	ShaderProgram* mShader;

	std::unordered_map<unsigned char, TextCharacter> mTextCharacters;
};
