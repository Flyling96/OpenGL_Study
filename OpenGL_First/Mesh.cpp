#include "Mesh.h"


Mesh::Mesh(VertexData * vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices)
{
	m_drawCount = numIndices;

	//1、绑定顶点数组对象
	glGenVertexArrays(1, &m_vertexArrayObject);//分配VAO(顶点数组对象)内存空间，1为设置的缓存ID
	glBindVertexArray(m_vertexArrayObject);//绑定顶点数组对象

	//2、把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);//分配VBO(顶点缓存对象)内存空间
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);//把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);//把用户定义的数据复制到当前绑定缓冲
	//glBufferData(目标缓冲的类型,传输数据的大小,发送的实际数据,显卡管理给定的数据的方式)

	// 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
	glGenBuffers(1, &m_elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(indices[0]), indices, GL_STATIC_DRAW);

	//4、设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);//告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
	//glVertexAttribPointer(指定我们要配置的顶点属性，顶点属性的大小（顶点属性是一个vec3，它由3个值组成，所以大小是3），
	//指定数据的类型，数据是否被标准化（归一化），步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔（数组紧密排列，所以为0），
	//表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0)
	glEnableVertexAttribArray(0);//以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	//颜色数据
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//纹理数据
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

}

Mesh::Mesh(Vertex * vertices, unsigned int numVertices)
{
	m_drawCount = numVertices;

	//1、绑定顶点数组对象
	glGenVertexArrays(1, &m_vertexArrayObject);//分配VAO(顶点数组对象)内存空间，1为设置的缓存ID
	glBindVertexArray(m_vertexArrayObject);//绑定顶点数组对象

	//2、把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);//分配VBO(顶点缓存对象)内存空间
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);//把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

	//3、设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);//告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
	glEnableVertexAttribArray(0);//以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	glBindVertexArray(0);

}

Mesh::Mesh(VertexData * vertices, unsigned int numVertexData)
{
	m_drawCount = numVertexData;

	//1、绑定顶点数组对象
	glGenVertexArrays(1, &m_vertexArrayObject);//分配VAO(顶点数组对象)内存空间，1为设置的缓存ID
	glBindVertexArray(m_vertexArrayObject);//绑定顶点数组对象

	//2、把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);//分配VBO(顶点缓存对象)内存空间
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);//把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
	glBufferData(GL_ARRAY_BUFFER, numVertexData * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

	//3、设置顶点属性指针
	//顶点数据
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);//告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
	glEnableVertexAttribArray(0);//以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	//颜色数据
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//纹理数据
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);
}


Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);

}


void Mesh::Draw(bool isEleMents)
{
	glBindVertexArray(m_vertexArrayObject);

	if (isEleMents)
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//线框模式
		glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, m_drawCount);
	}
	//glDrawArrays(打算绘制的OpenGL图元类型，顶点数组的起始索引，打算绘制的顶点数)

	glBindVertexArray(0);
}
