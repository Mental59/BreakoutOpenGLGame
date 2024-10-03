#version 330 core

in vec2 vTexCoords;
out vec4 oColor;

uniform sampler2D uTextImage;
uniform vec3 uTextColor;

void main()
{
	oColor = vec4(uTextColor, 1.0 * texture(uTextImage, vTexCoords).r);
}
