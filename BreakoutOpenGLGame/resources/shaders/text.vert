#version 330 core

layout (location = 0) in vec4 vertex;
out vec2 vTexCoords;

uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * vec4(vertex.xy, 0.0, 1.0);
    vTexCoords = vertex.zw;
}
