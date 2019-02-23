#version 150

in vec2 texCoord;
in vec3 transformedNormal;
uniform sampler2D sampler;
out vec4 outColor;

void main(void)
{
	const vec3 light = vec3(0, 1, 0);
	vec3 n = normalize(transformedNormal);
	float lightIntensity = 0.3 + 2*max(0,dot(normalize(light), n));
	outColor = lightIntensity*texture(sampler, texCoord);
}
