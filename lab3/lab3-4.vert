#version 150

in vec3 in_Position;
in vec2 inTexCoord;
out vec2 texCoordv;
in vec3 in_Normal;
uniform mat4 mw;
uniform mat4 worldToProj;
uniform mat4 bladeRot;

out vec3 nV;
out vec3 position;

void main(void)
{
	mat3 normalTransformation = mat3(mw);
  	nV = normalTransformation*in_Normal;
	position = vec3(mw*vec4(in_Position,1));
	gl_Position = worldToProj*bladeRot*mw*vec4(in_Position, 1.0);
	texCoordv = inTexCoord;
}
