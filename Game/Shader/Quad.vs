#version 140
in vec3 position;
in vec2 uv;

out vec2 TexCoord;


void main()
{
	gl_Position = vec4(position.x, position.y, 0.0,1.0);
	TexCoord = uv;
}