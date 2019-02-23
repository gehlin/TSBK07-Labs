#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;

uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 trans;

out vec3 transformedNormal;

void main(void)
{
	mat3 normalMatrix1 = mat3(trans);
  	transformedNormal = normalMatrix1*inNormal;
	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
	texCoord = inTexCoord;
}
