#version 150

in vec3 in_Position;
in vec2 inTexCoord;
uniform float time;
uniform mat4 rotMat;

out vec2 texCoordv;

void main(void)
{
	gl_Position = rotMat*vec4(in_Position, 1.0);
	texCoordv = inTexCoord;
}
