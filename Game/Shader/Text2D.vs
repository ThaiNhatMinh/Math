#version 140

in vec4 vertex; 

out vec2 TexCoord;

uniform mat4 Proj;

void main()
{
	//vec2 pos;
	//pos.x = (vertex.x - Screen.x/2.0)/(Screen.x/2.0);
	//pos.y = (vertex.y - Screen.y/2.0 )/(Screen.y/2.0);
	//gl_Position = vec4(pos.xy,0.0,1.0);
	gl_Position = Proj*vec4(vertex.xy,0.0,1.0);
	TexCoord = vertex.zw;

}