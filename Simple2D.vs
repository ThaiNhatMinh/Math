#version 140

in vec3 position;
in vec2 uv;


out vec2 UVs;
void main()
{
	

	gl_Position = vec4(position, 1.0);
	UVs =vec2(uv.x,uv.y);
	
}