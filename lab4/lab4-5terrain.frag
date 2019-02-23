#version 150

in vec2 texCoord;
in vec3 transformedNormal;
uniform sampler2D tex1, tex2;
out vec4 outColor;

in float height;
uniform float t;

void main(void)
{
	const vec3 light = vec3(0, 1, 1);
	vec3 n = normalize(transformedNormal);
	float lightIntensity = 0.3 + 2*max(0,dot(normalize(light), n));

	if(height < 0.1)
	{
		lightIntensity = 0.3 + 2*max(0,dot(normalize(light), sin(t)*n));
		outColor = lightIntensity*vec4(0,0.5,1,1);
	}
	else
	{
		outColor = lightIntensity*texture(tex1, texCoord)+0.2*height*lightIntensity*texture(tex2, texCoord);
	}

}
