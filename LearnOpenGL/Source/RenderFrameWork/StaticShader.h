#pragma once
#include <string>
#include "glm/glm.hpp"

class StaticShader
{
private:
	unsigned int m_ShaderProgram;

	mutable bool linklocked = false;
public:
	StaticShader();
	~StaticShader();

	void Bind() const;
	void UnBind() const;

	void Linked() const;


	void AttachNewShaderResource(const std::string& filepath, unsigned int type);

	void SetUniformVariable4f(const char* name, float v1, float v2, float v3, float v4);
	void SetUniformVariable3f(const char* name, float v1, float v2, float v3);
	void SetUniformVariable3f(const char* name, const glm::vec3& v);

	void SetUniformVariablei(const char* name, int v);

	void SetUniformVariable1f(const char* name, float v);

	void SetUniformMatrix4f(const char* name, const glm::mat4& matrix);

private:
	std::string GetshaderResBypath(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);

};