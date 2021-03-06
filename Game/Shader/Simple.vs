#version 140

// neu co bien khong dung toi thi trinh bien dich se tu dong xoa khi bien dich.
in vec3 position;	// location = 0
in vec3 normal;		// location = 1
in vec2 uv;			// location = 2, neu normal khong duoc dung toi no se bi xoa va location cua uv se la 1



out vec2 vUV;
out vec3 vNormal;
out vec3 vFragPos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

void main()
{
	

	gl_Position = Proj*View*Model*vec4(position, 1.0);
	vNormal = mat3(Model)* normal;
	vUV = uv;
	vFragPos  = vec3(Model * vec4(position, 1.0f));
	
}