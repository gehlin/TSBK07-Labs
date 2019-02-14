#version 150

in vec3 in_Position;
in vec2 inTexCoord;
out vec2 texCoordv;
in vec3 in_Normal;
uniform mat4 mw;
uniform mat4 worldToProj;
uniform mat4 bladeRot;
out float lightIntensity;

void main(void)
{
	mat3 normalTransformation = mat3(mw);
  	vec3 transformedNormal = normalTransformation*in_Normal;
	const vec3 light = vec3(0.58, 0.58, 0.58);
	lightIntensity = 2*max(0,dot(normalize(light), normalize(transformedNormal)));
	gl_Position = worldToProj*bladeRot*mw*vec4(in_Position, 1.0);
	texCoordv = inTexCoord;
}
