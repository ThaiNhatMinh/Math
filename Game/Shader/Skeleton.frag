#version 140


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


#define MAX_POINTLIGHT 10
#define MAX_SPOTLIGHT 10
uniform PointLight pointlight[MAX_POINTLIGHT];
uniform SpotLight spotlight[MAX_SPOTLIGHT];
uniform int numPointLight;
uniform int numSpotLight;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir,vec3 texel);
vec3 CalcSpotLight(SpotLight light,vec3 normal, vec3 fragPos, vec3 viewDir,vec3 texel);


uniform vec3 lightPos;
uniform vec3 EyePos;
uniform sampler2D TexMap;

out vec4 color;

in vec3 vNormal;
in vec2 vUV;
in vec3 vFragPos;
void main()
{
	//vec3 ambient = vec3(0.17f)*vec3(texture(TexMap,vUV));
	
	//vec3 lightDir = normalize(-lightPos);
	//float diff = max(dot(lightDir,vNormal), 0.0);
	//vec3 diffuse = vec3(diff)*vec3(texture(TexMap,vUV));

	//vec3 viewDir = normalize(EyePos - vFragPos);
	//vec3 halfwayDir = normalize(lightDir + viewDir);
	//vec3 reflectDir = reflect(-lightDir,vNormal);
	//float spec = pow(max(dot(viewDir,halfwayDir),0.0),32);
	//vec3 specular = vec3(spec)*vec3(texture(TexMap,vUV));

	//vec3 result = ambient + diffuse;// + specular;

	vec3 texel = vec3(texture(TexMap,vUV));
	vec3 result = vec3(0);
	vec3 norm = normalize(vNormal);
	vec3 viewDir = normalize(EyePos - vFragPos);

	for(int i=0; i<numSpotLight; i++)
	{
		result +=CalcSpotLight(spotlight[i],norm,vFragPos,viewDir,texel);
	}

	for(int i=0; i<numPointLight; i++)
	{
		result +=CalcPointLight(pointlight[i],norm,vFragPos,viewDir,texel);
	}

	if(numPointLight==0 && numSpotLight==0) result = texel;
	color = vec4(result,texture(TexMap,vUV).a);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir,vec3 texel)
{
	vec3 ambient = light.ambient * texel;

	vec3 lightDir = normalize(light.position - fragPos);
	float diffuseS = max(0.0, dot(normal, lightDir));
	vec3 diffuse = light.diffuse * diffuseS*texel;

	float specularStrength = 0.5;
	vec3 reflectDir = reflect(-lightDir,normal);
	float specularS = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	vec3 specular = light.specular * specularS * texel;

	float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	ambient  *= attenuation;  
    diffuse  *= attenuation;
    specular *= attenuation;  
	return (ambient + diffuse +specular);
}

vec3 CalcSpotLight(SpotLight light,vec3 normal, vec3 fragPos, vec3 viewDir,vec3 texel)
{
	vec3 ambient = light.ambient * texel;
	vec3 lightDir = normalize(light.position - fragPos);
	float diffuseS = max(0.0, dot(normal, lightDir));
	vec3 diffuse = light.diffuse * diffuseS * texel;

	//float specularStrength = 0.5;
	
	vec3 reflectDir = reflect(-lightDir,normal);
	float specularS = pow(max(dot(viewDir, reflectDir), 0.0),4);

	vec3 specular = light.specular * specularS * texel ;

	float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	//ambient  *= attenuation;  
   // diffuse  *= attenuation;
   // specular *= attenuation;  


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
