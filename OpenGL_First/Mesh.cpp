#include "Mesh.h"
#include <iostream>


Mesh::Mesh(std::vector<Vertex> verticesData, std::vector<int> indicesData, std::vector<Texture> texturesData)
{
	vertices = verticesData;
	indices = indicesData;
	textures = texturesData;
	Init(vertices, indices);
}

void Mesh::Init(std::vector<Vertex> verticesData, std::vector<int> indicesData)
{
	m_drawCount = indicesData.size();

	//1���󶨶����������
	glGenVertexArrays(1, &m_vertexArrayObject);//����VAO(�����������)�ڴ�ռ䣬1Ϊ���õĻ���ID
	glBindVertexArray(m_vertexArrayObject);//�󶨶����������

	//2�������ǵĶ������鸴�Ƶ�һ�����㻺���У���OpenGLʹ��
	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);//����VBO(���㻺�����)�ڴ�ռ�
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);//���´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����
	glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(Vertex), &verticesData[0], GL_STATIC_DRAW);//���û���������ݸ��Ƶ���ǰ�󶨻���
	//glBufferData(Ŀ�껺�������,�������ݵĴ�С,���͵�ʵ������,�Կ�������������ݵķ�ʽ)

	// 3. �������ǵ��������鵽һ�����������У���OpenGLʹ��
	glGenBuffers(1, &m_elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesData.size() * sizeof(indicesData[0]), &indicesData[0], GL_STATIC_DRAW);

	//4�����ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)0);//����OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ�
																				  //glVertexAttribPointer(ָ������Ҫ���õĶ������ԣ��������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3����
																				  //ָ�����ݵ����ͣ������Ƿ񱻱�׼������һ����������(Stride)�������������������Ķ���������֮��ļ��������������У�����Ϊ0����
																				  //��ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)������λ������������Ŀ�ͷ������������0)
	glEnableVertexAttribArray(0);//�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�

	//��ɫ����
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));
	glEnableVertexAttribArray(1);

	//��������
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texture));
	glEnableVertexAttribArray(2);

	//��������
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

Mesh::Mesh(VertexPosition * vertices, unsigned int numVertices)
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

Mesh::Mesh(std::vector<Vertex> verticesData)
{
	Init(verticesData);
}

void Mesh::Init(std::vector<Vertex> verticesData)
{
	m_drawCount = verticesData.size();

	//1���󶨶����������
	glGenVertexArrays(1, &m_vertexArrayObject);//����VAO(�����������)�ڴ�ռ䣬1Ϊ���õĻ���ID
	glBindVertexArray(m_vertexArrayObject);//�󶨶����������

										   //2�������ǵĶ������鸴�Ƶ�һ�����㻺���У���OpenGLʹ��
	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);//����VBO(���㻺�����)�ڴ�ռ�
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);//���´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����
	glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(Vertex), &verticesData[0], GL_STATIC_DRAW);

	//3�����ö�������ָ��
	//��������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);//����OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ�
	glEnableVertexAttribArray(0);//�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�

	//��ɫ����
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//��������
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//��������
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
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

void Mesh::Draw(Shader shader)
{
	// bind appropriate textures
	int diffuseNr = 1;
	int specularNr = 1;
	int normalNr = 1;
	int heightNr = 1;
	for (int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
										  // retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

												 // now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader.m_program, (name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}


	glBindVertexArray(m_vertexArrayObject);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�߿�ģʽ
	glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}


 Mesh::Mesh()
{

}

