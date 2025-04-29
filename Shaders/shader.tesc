#version 430

layout(vertices = 3) out;

layout(location = 0) in vec3 vColor[];

layout(location = 0) out vec3 tcColor[];

void main()
{
    if (gl_InvocationID == 0) {
	gl_TessLevelInner[0] = 20.0;
	gl_TessLevelOuter[0] = 20.0;
	gl_TessLevelOuter[1] = 20.0;
	gl_TessLevelOuter[2] = 20.0;
    }

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    tcColor[gl_InvocationID] = vColor[gl_InvocationID];
}
