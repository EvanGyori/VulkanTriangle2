#version 430

vec3 positions[3] = vec3[](
    vec3(-0.5f, 0.5f, 1.0f),
    vec3(0.5f, 0.5f, 1.0f),
    vec3(0.0f, -0.5f, 1.0f)
);

vec3 colors[3] = vec3[](
    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f)
);

//layout(location = 0) in vec3 pos;
//layout(location = 1) in vec3 color;

layout(location = 0) out vec3 vColor;

void main()
{
    /*
    gl_Position = vec4(pos, 1.0);
    vColor = color;
    */

    gl_Position = vec4(positions[gl_VertexIndex], 1.0);
    vColor = colors[gl_VertexIndex];
}
