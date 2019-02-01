#version 150

in vec4 colorv;
in vec2 texCoordv;
out vec4 out_Color;
out vec2 out_TexCoord;

void main(void)
{
	out_Color = colorv;
	out_TexCoord = texCoordv;
}
