#include "Mesh.h"


Mesh::Mesh(VertexData * vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices)
{
	m_drawCount = numIndices;

	//1���󶨶����������
	glGenVertexArrays(1, &m_vertexArrayObject);//����VAO(�����������)�ڴ�ռ䣬1Ϊ���õĻ���ID
	glBindVertexArray(m_vertexArrayObject);//�󶨶����������

	//2�������ǵĶ������鸴�Ƶ�һ�����㻺���У���OpenGLʹ��
	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);//����VBO(���㻺�����)�ڴ�ռ�
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);//���´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);//���û���������ݸ��Ƶ���ǰ�󶨻���
	//glBufferData(Ŀ�껺�������,�������ݵĴ�С,���͵�ʵ������,�Կ�������������ݵķ�ʽ)

	// 3. �������ǵ��������鵽һ�����������У���OpenGLʹ��
	glGenBuffers(1, &m_elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(indices[0]), indices, GL_STATIC_DRAW);

	//4�����ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);//����OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ�
	//glVertexAttribPointer(ָ������Ҫ���õĶ������ԣ��������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3����
	//ָ�����ݵ����ͣ������Ƿ񱻱�׼������һ����������(Stride)�������������������Ķ���������֮��ļ��������������У�����Ϊ0����
	//��ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)������λ������������Ŀ�ͷ������������0)
	glEnableVertexAttribArray(0);//�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�

	//��ɫ����
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//��������
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

}

Mesh::Mesh(Vertex * vertices, unsigned int numVertices)
{
	m_drawCount = numVertices;

	//1���󶨶����������
	glGenVertexArrays(1, &m_vertexArrayObject);//����VAO(�����������)�ڴ�ռ䣬1Ϊ���õĻ���ID
	glBindVertexArray(m_vertexArrayObject);//�󶨶����������

	//2�������ǵĶ������鸴�Ƶ�һ�����㻺���У���OpenGLʹ��
	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);//����VBO(���㻺�����)�ڴ�ռ�
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);//���´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

	//3�����ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);//����OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ�
	glEnableVertexAttribArray(0);//�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�

	glBindVertexArray(0);

}

Mesh::Mesh(VertexData * vertices, unsigned int numVertexData)
{
	m_drawCount = numVertexData;

	//1���󶨶����������
	glGenVertexArrays(1, &m_vertexArrayObject);//����VAO(�����������)�ڴ�ռ䣬1Ϊ���õĻ���ID
	glBindVertexArray(m_vertexArrayObject);//�󶨶����������

	//2�������ǵĶ������鸴�Ƶ�һ�����㻺���У���OpenGLʹ��
	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);//����VBO(���㻺�����)�ڴ�ռ�
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);//���´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����
	glBufferData(GL_ARRAY_BUFFER, numVertexData * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

	//3�����ö�������ָ��
	//��������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);//����OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ�
	glEnableVertexAttribArray(0);//�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�

	//��ɫ����
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//��������
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
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�߿�ģʽ
		glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, m_drawCount);
	}
	//glDrawArrays(������Ƶ�OpenGLͼԪ���ͣ������������ʼ������������ƵĶ�����)

	glBindVertexArray(0);
}
