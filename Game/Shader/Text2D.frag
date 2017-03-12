#version 140

in vec2 TexCoord;

uniform sampler2D TexFont;
uniform vec3 InColor;
out vec4 color;

void main()
{
	vec4 sam = vec4(1.0, 1.0, 1.0, texture(TexFont, TexCoord).r);
	color = vec4(InColor,1.0f) * sam;
}