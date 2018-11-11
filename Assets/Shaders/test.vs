#version 450

struct MatrixState
{
    mat4 mvp;
};

layout(std140, binding=0) u_matrixState;

layout(location = 0) in vec3 i_position;

layout(location = 1) in vec4 i_color;

layout(location = 0) out vec4 o_color;

void main()
{
    gl_Position = u_matrixState.mvp * i_position;
    v_color = a_color;
}
