#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 vTexCoords;

uniform mat4 uModel;
uniform mat4 uProjection;

void main()
{
    vTexCoords = vertex.zw;
    gl_Position = uProjection * uModel * vec4(vertex.xy, 0.0, 1.0);
}
