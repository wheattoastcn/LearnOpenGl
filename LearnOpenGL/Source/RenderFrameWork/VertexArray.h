#pragma once
#include "VertexBufferDesc.h"


class VertexBuffer;
class IndexBuffer;


class VertexArray
{
public:

	/** public function*/
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb,  const IndexBuffer& ib);

	void Bind() const;
	void UnBind() const;

	/** public variables*/

	VertexBufferDesc m_vBufferDesc;

private:
	unsigned int m_RenderID;
};