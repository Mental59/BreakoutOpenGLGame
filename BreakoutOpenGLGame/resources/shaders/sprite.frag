#version 330 core

in vec2 vTexCoords;
out vec4 oColor;

uniform sampler2D uImage;
uniform vec3 uSpriteColor;

void main()
{
	oColor = vec4(uSpriteColor, 1.0) * texture(uImage, vTexCoords);
}
