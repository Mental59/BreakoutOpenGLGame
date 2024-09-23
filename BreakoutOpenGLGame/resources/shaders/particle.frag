#version 330 core

in vec2 vTexCoords;
in vec4 vParticleColor;

out vec4 oColor;

uniform sampler2D uSprite;

void main()
{
    oColor = (texture(uSprite, vTexCoords) * vParticleColor);
}
