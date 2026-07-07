#shader
#version 330 core

layout(location = 0) in vec3 position;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 transform;
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

void main()
{
    gl_Position = p*v*m * transform * vec4(position, 1.0);
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(m*transform))) * aNormal;
    FragPos = vec3(m* transform*vec4(position,1.0));
}
