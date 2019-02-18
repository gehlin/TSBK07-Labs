#version 150

in vec3 in_Position;
in vec2 inTexCoord;
out vec2 texCoordv;
uniform mat4 mw;
uniform mat4 worldToProj;

void main(void)
{
	gl_Position = worldToProj*mw*vec4(in_Position, 1.0);
	texCoordv = inTexCoord;
}
