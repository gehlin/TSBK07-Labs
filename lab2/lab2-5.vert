#version 150

in vec3 in_Position;
in vec2 inTexCoord;
in vec3 in_Normal;
uniform mat4 rotAnim;
uniform mat4 totMat;
out vec2 texCoordv;
out float lightIntensity;

void main(void)
{
	mat3 normalMatrix1 = mat3(rotAnim);
    vec3 transformedNormal = normalMatrix1 * in_Normal;
	const vec3 light = vec3(0.58, 0.58, 0.58);
	lightIntensity = 1*max(0,dot(normalize(light), normalize(transformedNormal)));
	gl_Position = totMat*rotAnim*vec4(in_Position, 1.0);
	texCoordv = inTexCoord;
}
