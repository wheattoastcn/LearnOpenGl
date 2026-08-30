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
    vec3 direction;
    float cutOff;
    float outerCutOff;

    /*float constant;
    float linear;
    float quadratic;*/
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform sampler2D u_Specular;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    
    vec3 ambient = material.ambientStrength * light.color;    

    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse =material.diffuseStrength* diff * light.color*intensity;



    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specularStrength * spec * light.color*texture(u_Specular, TexCoord).rgb*intensity;

    FragColor = vec4(ambient+diffuse, 1.0) *texture(u_Texture, TexCoord)+vec4(specular, 1.0);
} 
