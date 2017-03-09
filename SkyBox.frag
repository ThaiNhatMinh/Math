#version 140


in vec3 TexCoords;
out vec4 color;
uniform samplerCube Skybox;
void main()
{
	color = vec4(0.0,1.0,1.0,1.0);
	color = texture(Skybox, TexCoords);
}