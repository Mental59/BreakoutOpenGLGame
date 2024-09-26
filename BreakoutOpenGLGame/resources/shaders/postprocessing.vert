#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 vTexCoords;

uniform bool uChaos = false;
uniform bool uConfuse = false;
uniform bool uShake = false;
uniform float uTime = 0.0f;

void main()
{
    gl_Position = vec4(vertex.xy, 0.0f, 1.0f);
    vec2 texture = vertex.zw;

    if (uChaos)
    {
        float strength = 0.3;
        vec2 pos = vec2(texture.x + sin(uTime) * strength, texture.y + cos(uTime) * strength);
        vTexCoords = pos;
    }
    else if (uConfuse)
    {
        vTexCoords = vec2(1.0 - texture.x, 1.0 - texture.y);
    }
    else
    {
        vTexCoords = texture;
    }

    if (uShake)
    {
        float strength = 0.01;
        gl_Position.x += cos(uTime * 10) * strength;
        gl_Position.y += cos(uTime * 15) * strength;
    }
}
