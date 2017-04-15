#version 140

// neu co bien khong dung toi thi trinh bien dich se tu dong xoa khi bien dich.
in vec3 position;	// location = 0
in vec3 normal;		// location = 1
in vec2 uv;			// location = 2, neu normal khong duoc dung toi no se bi xoa va location cua uv se la 1
in vec2 ws1;			// x: bone y:weight
in vec2 ws2;			// x: bone y:weight
in vec2 ws3;			// x: bone y:weight
in vec2 ws4;			// x: bone y:weight




uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;
uniform mat4 InvBindPose[70];
uniform mat4 Transform[70];

out vec3 vNormal;
out vec2 vUV;
out vec3 vFragPos;
out vec3 vColor;
void main()
{
	vColor = vec3(0.0f);
	vec4 pos = vec4(position,1.0f);
	vec4 finalPos = vec4(0);
	vec3 finalNormal = vec3(0);
	int b;
	if((ws1.x<71 )&&( ws1.x>0))
	{
		b =int(ws1.x);
		finalPos += Transform[b] * pos * vec4(ws1.y) ;
		finalNormal += mat3(Transform[b])* normal * vec3(ws1.y);
	}//else vColor = vec3(0.5f,0.9f,0.5f);

	if((ws2.x<71 )&& (ws2.x>0))
	{
		b =int(ws2.x);
		finalPos +=Transform[b] * pos * vec4(ws2.y) ;
		finalNormal += mat3( Transform[b])* normal * vec3(ws2.y);
	}//else vColor = vec3(0.5f,0.9f,0.5f);

	if((ws3.x<71) && (ws3.x>0))
	{
		b =int(ws3.x);
		finalPos += Transform[b] * pos * vec4(ws3.y) ;
		finalNormal += mat3(Transform[b])* normal * vec3(ws3.y);
	}//else vColor = vec3(0.5f,0.9f,0.5f);

	if((ws4.x<71) &&( ws4.x>0))
	{
		b =int(ws4.x);
		finalPos += Transform[b] * (pos * vec4(ws4.y) );
		finalNormal +=   mat3(Transform[b])* normal * vec3(ws4.y);
	}//else vColor = vec3(0.5f,0.9f,0.5f);

	vColor = vec3(uv.x);
	gl_Position = Proj*View*Model*finalPos;
	vNormal = finalNormal;  

	vUV = uv;
	vFragPos = vec3(Model)*finalNormal;

	
}