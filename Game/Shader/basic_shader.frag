#version 140


out vec4 color;

in vec2 TexCoord;
in vec3 Normal;
in vec3 vFragPos;
in vec2 lightCoord;

uniform sampler2D texMap;
uniform sampler2D lightMap;
void main()
{
  
	
	color = texture(texMap, TexCoord)*texture(lightMap, lightCoord);
}