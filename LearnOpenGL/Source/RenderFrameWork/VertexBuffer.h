#pragma once



class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size, unsigned int DrawType);
	~VertexBuffer();

	void Bind() const;
	void RemoveBind() const;
private:
	unsigned int m_RenderID;
};