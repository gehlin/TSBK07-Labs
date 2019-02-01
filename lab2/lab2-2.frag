#version 150

in vec2 texCoordv;
uniform sampler2D tex;

out vec4 outColor;

void main(void)
{;
	outColor = texture(tex, 10*texCoordv);
}
