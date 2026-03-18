#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RenderID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RenderID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb,const IndexBuffer& ib)
{
	Bind();
	vb.Bind();
	ib.Bind();
	const auto& elements = m_vBufferDesc.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(0);
		/*
		* 1.	index: 顶点属性的索引，通常是从 0 开始的整数。这个索引对应于着色器中的属性位置。
		  2.	size: 每个顶点属性的组件数量。例如，如果一个顶点位置由 x, y, z 三个分量组成，则 size 为 3。
		  3.	type: 数据类型，指定每个组件的数据类型。常见的类型有：
		   •	GL_FLOAT: 浮点数
		   •	GL_INT: 整数
		   •	GL_UNSIGNED_BYTE: 无符号字节
		  4.	normalized: 指定是否将固定点数据归一化到 [0, 1] 或 [-1, 1] 范围。对于浮点数，这个参数通常为 GL_FALSE。
		  5.	stride: 相邻顶点属性之间的字节偏移量。如果顶点数据是紧凑的（没有间隔），可以设置为 0。
		  6.	pointer: 指向顶点数据的指针，通常是相对于顶点缓冲区的偏移量。
		*/
		glVertexAttribPointer(i, element.count, element.type, element.normalized, m_vBufferDesc.GetStride(), (const void*)offset);
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RenderID);
}

void VertexArray::UnBind() const
{
	glBindVertexArray(0);
}
