#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 vTexCoords;
out vec4 vParticleColor;

uniform mat4 uProjection;
uniform vec2 uOffset;
uniform vec4 uColor;
uniform float uScale = 10.0f;

void main()
{
    vTexCoords = vertex.zw;
    vParticleColor = uColor;
    gl_Position = uProjection * vec4(uScale * vertex.xy + uOffset, 0.0, 1.0);
}
