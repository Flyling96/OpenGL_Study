#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;
in vec3 Tangent;
in vec3 Bitangent;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform vec3 viewPos;

struct Material {
	sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 1
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct Light {
    vec3 position;
	//vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

float constant =1.0;
float linear = 0.09;
float quadratic = 0.032;

uniform Material material;
uniform Light light;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcBumpedNormal();


void main()
{    
	vec3 realNormal = CalcBumpedNormal();
	vec3 viewDir = normalize(viewPos - FragPos);

	// 第一阶段：定向光照
    vec3 result =vec3(0,0,0);
	// 第二阶段：点光源
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], realNormal, FragPos, viewDir);  
	

	FragColor = vec4(result,1);
	//FragColor = vec4(0,0,0,1);
}

//平行光光照值计算
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
	// 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoord));
	vec3 specular = light.specular * spec;
    return (ambient + diffuse + specular);
}

//点光源光照值计算
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    //漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
	//镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
					light.quadratic * (distance * distance));    
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoord));
	vec3 specular = light.specular * spec *0.05f;
    ambient  *= attenuation;
    diffuse  *= attenuation;
	specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcBumpedNormal()
{
	vec3 normal = normalize(Normal);
    vec3 tangent = normalize(Tangent);
	vec3 bitangent = normalize(Bitangent);
    vec3 BumpMapNormal = vec3(texture(texture_normal1, TexCoord));
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    mat3 TBN = mat3(tangent, bitangent, normal);
    return  normalize(TBN * BumpMapNormal);
}