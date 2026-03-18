#include "VertexBuffer.h"
#include <glad/glad.h> 

VertexBuffer::VertexBuffer(const void* data, unsigned int size, unsigned int DrawType)
{
	glGenBuffers(1, &m_RenderID);
	glBindBuffer(GL_ARRAY_BUFFER,m_RenderID);
	glBufferData(GL_ARRAY_BUFFER,size,data,DrawType);

}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RenderID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);	
}

void VertexBuffer::RemoveBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
