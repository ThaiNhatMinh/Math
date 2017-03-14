#version 140


out vec4 color;

in vec2 TexCoord;
in vec3 Normal;
in vec3 vFragPos;


uniform sampler2D texMap;

void main()
{
  
	
	color = texture(texMap, TexCoord);
}