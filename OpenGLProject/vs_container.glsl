#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 loc;
layout (location = 2) in vec3 scaling;

uniform mat4 v_matrix;
uniform mat4 p_matrix;

mat4 buildScale(vec3 t)
{
	return mat4(t.x, 0, 0, 0,
	            0, t.y, 0, 0,
	            0, 0, t.z, 0,
	            0, 0, 0, 1);
}

mat4 buildTranslate(vec3 t)
{
return mat4(
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	t.x,t.y,t.z,1
 );
}

mat4 mv_matrix = v_matrix * buildTranslate(loc) * buildScale(scaling);

void main(void)
{
    gl_Position = p_matrix * mv_matrix * vec4(position,1.0);
}