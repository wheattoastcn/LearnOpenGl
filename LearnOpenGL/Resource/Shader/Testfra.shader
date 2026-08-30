#shader
#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
layout(location = 0) out vec4 FragColor;
uniform sampler2D u_Texture;

struct Material {
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
    float shininess;
};

struct Light {
    vec3 color;
    vec3 position;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform sampler2D u_Specular;

void main()
{

    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                light.quadratic * (distance * distance));

    vec3 ambient = material.ambientStrength * light.color*attenuation;    


    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse =material.diffuseStrength* diff * light.color*attenuation;



    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specularStrength * spec * light.color*texture(u_Specular, TexCoord).rgb*attenuation;

    FragColor = vec4(ambient+diffuse, 1.0) *texture(u_Texture, TexCoord)+vec4(specular, 1.0);
} 
