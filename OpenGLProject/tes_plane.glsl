#version 430 core
uniform mat4 v_matrix;
uniform mat4 p_matrix;
layout (triangles, equal_spacing, cw) in;

void main(void)
{
    gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) +
                  (gl_TessCoord.y * gl_in[1].gl_Position) + 
                  (gl_TessCoord.z * gl_in[2].gl_Position);
}