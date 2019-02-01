#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;
out vec2 texCoordv;
out vec4 colorv;
uniform mat4 rotMat;

void main(void)
{
	colorv = abs(vec4(in_Normal, 1.0));
	gl_Position = rotMat*vec4(in_Position, 1.0);
	texCoordv = inTexCoord;
}
