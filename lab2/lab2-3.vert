#version 150

in vec3 in_Position;
in vec2 inTexCoord;
uniform float time;
uniform mat4 rotMat;
uniform mat4 totMat;

out vec2 texCoordv;

void main(void)
{
	gl_Position = totMat*rotMat*vec4(in_Position, 1.0);
	texCoordv = inTexCoord;
}
