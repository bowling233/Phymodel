#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;
out vec2 tc;

layout (location = 3) in vec3 model;
layout (location = 4) in float scale;


uniform mat4 v_matrix;
uniform mat4 proj_matrix;

layout (binding=0) uniform sampler2D s;

mat4 buildScale(float s)
{
mat4 scale = mat4(
 s,0,0,0,
 0,s,0,0,
 0,0,s,0,
 0,0,0,1
 );
 return scale;
}

mat4 buildTranslate(vec3 t)
{
mat4 trans = mat4(
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	t.x,t.y,t.z,1
 );
return trans;
}

void main(void)
{	
	gl_Position = proj_matrix * v_matrix * buildTranslate(model) * buildScale(scale) * vec4(position,1.0);
	tc = tex_coord;
}
