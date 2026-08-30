#shader
#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
layout(location = 0) out vec4 FragColor;

uniform vec3 viewPos;

struct Material {
    float diffuseStrength;
    float specularStrength;
    float shininess;

    sampler2D Texture;
    sampler2D Specular;
};
uniform Material material;


struct DirLight {
    vec3 direction;
   
    vec3 color;

    float intensity;
    float ambientStrength;
};
uniform DirLight dirLight;

vec3 CalcDirLight(DirLight dirlight, vec3 normal, vec3 viewDir)
{
   vec3 lightDir = normalize(-dirlight.direction);
    

    vec3 ambient = dirlight.ambientStrength * dirlight.color;

   vec3 diffuse = dirlight.color * material.diffuseStrength * max(dot(normal, lightDir), 0.0);
   
   
   vec3 reflectDir = reflect(-lightDir, normal);  
   vec3 specular = dirlight.color * material.specularStrength * pow(max(dot(viewDir, reflectDir), 0.0),material.shininess);


   vec3 result = (ambient+diffuse)*texture(material.Texture, TexCoord).rgb + specular*texture(material.Specular, TexCoord).rgb;

   return result*dirlight.intensity;
}

struct PointLight
{
    vec3 color;
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    float intensity;
    float ambientStrength;
};
uniform PointLight pointLight;

vec3 CalcPointLight(PointLight pointlight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    float distance    = length(pointlight.position - fragPos);
    float attenuation = 1.0 / (pointlight.constant + pointlight.linear * distance + 
                pointlight.quadratic * (distance * distance));

    vec3 lightDir = normalize(pointlight.position - fragPos);


    vec3 ambient = pointlight.ambientStrength * pointlight.color;

    vec3 diffuse = pointlight.color * material.diffuseStrength * max(dot(normal, lightDir), 0.0);
   
   
   vec3 reflectDir = reflect(-lightDir, normal);  
   vec3 specular = pointlight.color * material.specularStrength * pow(max(dot(viewDir, reflectDir), 0.0),material.shininess);


   vec3 result = (ambient+diffuse)*texture(material.Texture, TexCoord).rgb + specular*texture(material.Specular, TexCoord).rgb;

   return result*pointlight.intensity*attenuation;
}


struct SpotLight {
    vec3 color;
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

   float intensity;
};
uniform SpotLight spotLight;

vec3 CalcSpotLight(SpotLight spotlight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(spotlight.position - fragPos);
    float theta = dot(lightDir, normalize(-spotlight.direction));
    float epsilon = spotlight.cutOff - spotlight.outerCutOff;
    float intensity = clamp((theta - spotlight.outerCutOff) / epsilon, 0.0, 1.0);



    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse =material.diffuseStrength* diff * spotlight.color*intensity;


    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specularStrength * spec * spotlight.color*texture(material.Specular, TexCoord).rgb*intensity;

    return diffuse *texture(material.Texture, TexCoord).rgb+specular*spotlight.intensity;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    result += CalcPointLight(pointLight, norm, FragPos, viewDir);
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

