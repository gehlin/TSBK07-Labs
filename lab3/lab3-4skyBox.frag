#version 150

in vec2 texCoordv;
uniform sampler2D sampler;
out vec4 outColor;

void main(void)
{
	outColor = texture(sampler, texCoordv);
}
