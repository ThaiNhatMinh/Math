#version 140

struct Material{
	//sampler2D diffuse;
	//vec3 specular;
	float shininess;
};
struct DirLight // direction light
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
    float linear;
    float quadratic;
};

struct SpotLight{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
    float linear;
    float quadratic;

	float cutOff;
	float outercutOff;
};

in vec2 UVs;
in vec3 vNormals;
in vec3 vFragPos;


uniform vec3 EyePos;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

uniform Material material;
uniform DirLight	dirlight;
#define NUM_POINTLIGHT 2
uniform PointLight pointlight[NUM_POINTLIGHT];
uniform SpotLight spotlight;

out vec4 color;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  
vec3 CalcSpotLight(SpotLight spotlight,vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{

	

	vec3 norm = normalize(vNormals);
	vec3 viewDir = normalize(EyePos - vFragPos);
	
	vec3 result = CalcDirLight(dirlight,norm,viewDir);

	for(int i=0; i<2 ;i++) result += CalcPointLight(pointlight[i],norm,vFragPos,viewDir);

	result += CalcSpotLight(spotlight,norm,vFragPos,viewDir);

	
	
	
	color = vec4(result,1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 ambient = light.ambient*vec3(texture(diffuseMap,UVs));

	vec3 lightDir = normalize(-light.direction);
	float diffuseS = max(0.0, dot(normal, lightDir));
	vec3 diffuse = light.diffuse * (diffuseS * vec3(texture(diffuseMap,UVs)));

	float specularStrength = 0.5;
	vec3 reflectDir = reflect(-lightDir,normal);
	float specularS = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 specular = light.specular * (specularS * vec3(texture(specularMap,UVs))) ;
	return (ambient + diffuse + specular);
}
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 ambient = light.ambient*vec3(texture(diffuseMap,UVs));

	vec3 lightDir = normalize(light.position - fragPos);
	float diffuseS = max(0.0, dot(normal, lightDir));
	vec3 diffuse = light.diffuse * (diffuseS * vec3(texture(diffuseMap,UVs)));

	float specularStrength = 0.5;
	vec3 reflectDir = reflect(-lightDir,normal);
	float specularS = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 specular = light.specular * (specularS * vec3(texture(specularMap,UVs))) ;

	float distance    = length(light.position - vFragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	ambient  *= attenuation;  
    diffuse  *= attenuation;
    specular *= attenuation;  
	return (ambient + diffuse + specular);
}
vec3 CalcSpotLight(SpotLight light,vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 ambient = light.ambient*vec3(texture(diffuseMap,UVs));
	vec3 lightDir = normalize(light.position - vFragPos);
	float diffuseS = max(0.0, dot(normal, lightDir));
	vec3 diffuse = light.diffuse * (diffuseS * vec3(texture(diffuseMap,UVs)));

	float specularStrength = 0.5;
	
	vec3 reflectDir = reflect(-lightDir,normal);
	float specularS = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 specular = light.specular * (specularS * vec3(texture(specularMap,UVs))) ;

	float distance    = length(light.position - vFragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	ambient  *= attenuation;  
    diffuse  *= attenuation;
    specular *= attenuation;  


	float theta = dot(lightDir, normalize(-light.direction));
	float delta = light.cutOff - light.outercutOff;
	float instensity = clamp((theta-light.cutOff)/delta,0.0f, 1.0f);
	vec3 result;
	if(theta>light.cutOff)
	{
		return  (ambient + diffuse*instensity + specular*instensity);
	}
	else return ambient  ;
}