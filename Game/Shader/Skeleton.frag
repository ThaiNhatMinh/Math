#version 140
uniform vec3 lightPos;
uniform vec3 EyePos;
uniform sampler2D TexMap;

out vec4 color;

in vec3 vNormal;
in vec2 vUV;
in vec3 vFragPos;
in vec3 vColor;
void main()
{
	vec3 ambient = vec3(0.17f)*vec3(texture(TexMap,vUV));
	
	vec3 lightDir = normalize(-lightPos);
	float diff = max(dot(lightDir,vNormal), 0.0);
	vec3 diffuse = vec3(diff)*vec3(texture(TexMap,vUV));

	vec3 viewDir = normalize(EyePos - vFragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	vec3 reflectDir = reflect(-lightDir,vNormal);
	float spec = pow(max(dot(viewDir,halfwayDir),0.0),32);
	vec3 specular = vec3(spec)*vec3(texture(TexMap,vUV));

	vec3 result = ambient + diffuse + specular;
	color = vec4(result,texture(TexMap,vUV).a);
}