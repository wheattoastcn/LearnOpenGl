#pragma once
class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void RemoveBind() const;
private:
	unsigned int m_RenderID;
	unsigned int m_Count;
};