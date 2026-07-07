#include "StaticShader.h"
#include "glad/glad.h"
#include <iostream>
#include <sstream>
#include <fstream>


StaticShader::StaticShader()
{
	m_ShaderProgram = glCreateProgram();
}

StaticShader::~StaticShader()
{
	UnBind();
    glDeleteProgram(m_ShaderProgram);
}

void StaticShader::Bind() const
{
   
    glUseProgram(m_ShaderProgram);
}

void StaticShader::UnBind() const
{
    linklocked = false;
    glUseProgram(0);
}

void StaticShader::Linked() const
{
    linklocked = true;

    glLinkProgram(m_ShaderProgram);

    int  success;
    char infoLog[512];

    glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::linked shaderprogram failed\n" << infoLog << std::endl;
    }

    glValidateProgram(m_ShaderProgram);
}

void StaticShader::AttachNewShaderResource(const std::string& filepath, unsigned int type)
{
    std::string shaderResource = GetshaderResBypath(filepath);
    unsigned int shader = CompileShader(type, shaderResource);

    if(!shader)
    {
        return;
    }

    if(!linklocked)
    {
        glAttachShader(m_ShaderProgram, shader);
        glDeleteShader(shader);
    }
    {
        //TODO ??????? ?????????
        return;
    }
}

void StaticShader::SetUniformVariable4f(const char* name, float v1, float v2, float v3, float v4)
{
    int loaction = glGetUniformLocation(m_ShaderProgram, name);
    glUniform4f(loaction, v1, v2, v3, v4);
}

void StaticShader::SetUniformVariable3f(const char* name, float v1, float v2, float v3)
{
    int loaction = glGetUniformLocation(m_ShaderProgram, name);
    glUniform3f(loaction, v1, v2, v3);
}

void StaticShader::SetUniformVariable3f(const char* name, const glm::vec3& v)
{
    int loaction = glGetUniformLocation(m_ShaderProgram, name);
    glUniform3f(loaction, v.x, v.y, v.z);
}

void StaticShader::SetUniformVariablei(const char* name, int v)
{
    int loaction = glGetUniformLocation(m_ShaderProgram, name);
    glUniform1i(loaction, v);
}

void StaticShader::SetUniformVariable1f(const char* name, float v)
{
    int loaction = glGetUniformLocation(m_ShaderProgram, name);
    glUniform1f(loaction, v);
}

void StaticShader::SetUniformMatrix4f(const char* name, const glm::mat4& matrix)
{
    int loaction = glGetUniformLocation(m_ShaderProgram, name);
    glUniformMatrix4fv(loaction, 1, GL_FALSE, &matrix[0][0]);
}

std::string StaticShader::GetshaderResBypath(const std::string& filepath)
{
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream shaderSource;

    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
           //TODO ?????????? ?????????
        }
        else
        {
            shaderSource << line << "\n";
        }

    }

    return shaderSource.str();
}

unsigned int StaticShader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);


    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        std::cout << "Failed to compile shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}
