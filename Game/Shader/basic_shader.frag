#version 140


uniform vec3 lightPos;

out vec4 outColor;


in vec3 vNormals;
in vec3 vFragPos;

void main()
{
  
	vec3 diffuse = vec3(1.0f, 1.0f, 1.0f);
	vec3 ambient = vec3(0.1f, 0.1f, 0.1f);
	vec3 specular = vec3(0.4f, 0.4f, 0.4f);

	vec3 lightDir = normalize(lightPos - vFragPos);
	vec3 norm = normalize(vNormals);
	float diff = max(dot(norm, lightDir), 0.0f);
	diffuse = diffuse * vec3(diff);

	outColor = vec4(diffuse + ambient,1.0f);
}