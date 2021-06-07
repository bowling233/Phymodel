#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 loc;
layout (location = 2) in vec3 norv;

mat4 rotate(vec3 axis, float angle) {
  float s = sin(angle);
  float c = cos(angle);
  float oc = 1.0 - c;
  return mat4(
		oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
    oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
    oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
		0.0,                                0.0,                                0.0,                                1.0
	);
}

mat4 buildRotate(vec3 vectorBefore,vec3 vectorAfter)
{
    vec3 rotationAxis;
    float rotationAngle;
	mat4 rotationMatrix;
	rotationAxis = cross(vectorBefore, vectorAfter);
	rotationAngle = acos(dot(vectorBefore, vectorAfter));
	rotationMatrix = rotate(rotationAxis,rotationAngle);
	return rotationMatrix;
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

uniform mat4 v_matrix;
uniform mat4 p_matrix;

mat4 mv_matrix = v_matrix * buildTranslate(loc) * buildRotate(vec3(0.0,0.0,1.0),norv);

void main(void)
{
    gl_Position = p_matrix * mv_matrix * vec4(position,1.0);
}