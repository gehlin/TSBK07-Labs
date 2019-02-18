#version 150

in vec2 texCoordv;
uniform sampler2D sampler;

out vec4 outColor;

uniform vec3 camPlacement;
uniform vec3 lightSourcesColorArr[4];
uniform vec3 lightSourcesDirPosArr[4];
uniform float specularExponent;
uniform bool isDirectional[4];
in vec3 nV;
in vec3 position;

void main(void)
{
	vec3 n = normalize(nV);
	float ambient = 0.3;
	vec3 diffuse = vec3(0,0,0);
	vec3 specular = vec3(0,0,0);
	vec3 v = normalize(camPlacement - position);
	vec3 s;
	for(int i = 0; i < lightSourcesDirPosArr.length(); i++)
	{
		if (isDirectional[i])
		{
			s = normalize(lightSourcesDirPosArr[i]);
		}
		else
		{
			s = normalize(lightSourcesDirPosArr[i] - position);
		}
		vec3 r = normalize(2*dot(s, n)*n - s);
		diffuse += lightSourcesColorArr[i]*max(0,dot(s, n));
		specular += lightSourcesColorArr[i]*pow(max(0,dot(r,v)),125);

	}


	vec4 lightIntensity = vec4(ambient*vec3(1,1,1) + diffuse + specular,1);
	outColor = lightIntensity*texture(sampler, texCoordv);
}
