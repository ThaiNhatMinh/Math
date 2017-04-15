#version 140

// neu co bien khong dung toi thi trinh bien dich se tu dong xoa khi bien dich.
in vec3 position;	// location = 0
in vec3 normal;		// location = 1
in vec2 uv;			// location = 2, neu normal khong duoc dung toi no se bi xoa va location cua uv se la 1



uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;
out vec3 TexCoords;
void main()
{
	
	TexCoords = vec3(position.x,-position.y,position.z);
	vec4 pos = Proj*View*Model*vec4(position, 1.0);
	gl_Position = pos.xyww;


	
}