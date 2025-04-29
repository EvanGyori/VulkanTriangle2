#version 430

layout(location = 0) in vec3 teColor;

layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = vec4(teColor, 1.0);
}
