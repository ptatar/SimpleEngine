#version 450

layout(std140, binding=0) uniform MatrixState
{
    mat4 mvp;
} u_matrixState;

layout(location = 0) in vec3 i_position;

layout(location = 1) in vec4 i_color;

layout(location = 2) in float i_dupa;

layout(location = 0) out vec4 o_color;

void main()
{
    gl_Position = u_matrixState.mvp * vec4(i_position, 1.0);
    o_color = i_color;
}
