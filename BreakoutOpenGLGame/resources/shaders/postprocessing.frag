#version 330 core

in vec2 vTexCoords;
out vec4 oColor;

uniform sampler2D uScene;
uniform vec2 uOffsets[9];
uniform int uEdgeKernel[9];
uniform float uBlurKernel[9];

uniform bool uChaos = false;
uniform bool uConfuse = false;
uniform bool uShake = false;

void main()
{
    oColor = vec4(0.0f);
    vec3 samples[9];

    // sample from texture uOffsets if using convolution matrix
    if(uChaos || uShake)
    {
        for(int i = 0; i < 9; i++)
        {
            samples[i] = vec3(texture(uScene, vTexCoords.st + uOffsets[i]));
        }
    }

    // process effects
    if (uChaos)
    {           
        for(int i = 0; i < 9; i++)
        {
            oColor += vec4(samples[i] * uEdgeKernel[i], 0.0f);
        }
        oColor.a = 1.0f;
    }
    else if (uConfuse)
    {
        oColor = vec4(1.0 - texture(uScene, vTexCoords).rgb, 1.0);
    }
    else if (uShake)
    {
        for(int i = 0; i < 9; i++)
        {
            oColor += vec4(samples[i] * uBlurKernel[i], 0.0f);
        }
        oColor.a = 1.0f;
    }
    else
    {
        oColor =  texture(uScene, vTexCoords);
    }
}
