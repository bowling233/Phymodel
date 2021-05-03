#version 430

in vec2 tc;
out vec4 color;


layout (binding=0) uniform sampler2D s;

void main(void)
{	color = texture(s,tc);
}
