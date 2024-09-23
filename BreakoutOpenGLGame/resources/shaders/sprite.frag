#version 330 core

in vec2 vTexCoords;
out vec4 oColor;

uniform sampler2D uImage;
uniform vec4 uSpriteColor;

void main()
{
	oColor = uSpriteColor * texture(uImage, vTexCoords);
}
