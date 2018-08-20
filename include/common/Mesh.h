#pragma once
#include<GLFW/glfw3.h>
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<vector>

#include"Shader.h"

class Normal
{
public:
	Normal(){x = y =  z = 0;}
	Normal(float a, float b, float c)
	{
		ChangeNormal(a, b, c);
	}
	void ChangeNormal(float a, float b, float c)
	{
		x = a;
		y = b;
		z = c;
	}
	glm::vec3 GetNormal()
	{
		return glm::vec3(x, y, z);
	}
private:
	float x;
	float y;
	float z;
};

class VertexPosition
{
public:
	VertexPosition(){posX = posY = posZ = 0;}
	VertexPosition(float x,float y,float z)
	{
		posX = x;
		posY = y;
		posZ = z;
	}

	glm::vec3 GetVertex()
	{
		return glm::vec3(posX, posY, posZ);
	}
private:
	float posX;
	float posY;
	float posZ;
};

class Color
{
public:
	Color(){r = g = b = a = 1;}
	Color(float x, float y, float z,float w )
	{
		r = x;
		g = y;
		b = z;
		a = w;
	}
private:
	float r, g, b, a;

};

class TextureUV
{

public:
	TextureUV(){u = 0, v = 0;}
	TextureUV(float a, float b)
	{
		u = a;
		v = b;
	}

	void ChangeUV(float a, float b)
	{
		u = a;
		v = b;
	}

private:
	float u, v;


};

class Tangent
{
public:
	Tangent() { x = y = z = 0; }
	Tangent(float a, float b, float c)
	{
		ChangeTangent(a, b, c);
	}

	void ChangeTangent(float a, float b, float c)
	{
		x = a; y = b; z = c;
	}

private:
	float x, y, z;
};

class Bitangent
{
public:
	Bitangent() { x = y = z = 0; }
	Bitangent(float a, float b, float c)
	{
		ChangeBitangent(a, b, c);
	}

	void ChangeBitangent(float a, float b, float c)
	{
		x = a; y = b; z = c;
	}

private:
	float x, y, z;
};

class Vertex
{
public:
	Vertex() {}
	Vertex(VertexPosition v, Color c, TextureUV t)
	{
		vertex = v;
		color = c;
		texture = t;
		normal = Normal(0, 0, 0);
	}

	Vertex(VertexPosition v, Color c, TextureUV t, Normal n)
	{
		vertex = v;
		color = c;
		texture = t;
		normal = n;
	}
	Vertex(VertexPosition v, Color c, TextureUV t, Normal n,Tangent tan,Bitangent b)
	{
		vertex = v;
		color = c;
		texture = t;
		normal = n;
		tangent = tan;
		bitangent = b;
	}

	void ChangeNormal(float a, float b, float c)
	{
		normal.ChangeNormal(a,b,c);
	}
	glm::vec3 GetVertex()
	{
		return vertex.GetVertex();
	}

	VertexPosition vertex;
	Color color;
	TextureUV texture;
	Normal normal;
	Tangent tangent;
	Bitangent bitangent;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	unsigned int m_vertexArrayObject;					//VAO,顶点数组对象
	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<int> indices,std::vector<Texture> textures);
	void Init(std::vector<Vertex> vertices, std::vector<int> indices);
	Mesh(VertexPosition * vertices, unsigned int numVertices);
	Mesh(std::vector<Vertex> vertices);
	void Init(std::vector<Vertex> vertices);
	void Draw(bool isEleMents);
	void Draw(Shader &shader);

	~Mesh();

	std::vector<Vertex> vertices;
	std::vector<int> indices;
	std::vector<Texture> textures;

private:
	enum
	{
		POSITION_VB,
		NUM_BUFFERS
	};
	unsigned int m_vertexArrayBuffers;				//VBO,顶点缓存对象
	unsigned int m_elementBufferObject;				//EBO,索引缓存对象
	unsigned int m_drawCount;


};


Mesh::Mesh(std::vector<Vertex> verticesData, std::vector<int> indicesData, std::vector<Texture> texturesData)
{
	this->vertices = verticesData;
	this->indices = indicesData;
	this->textures = texturesData;
	Init(vertices, indices);
}

void Mesh::Init(std::vector<Vertex> verticesData, std::vector<int> indicesData)
{
	m_drawCount = indicesData.size();

	//1、绑定顶点数组对象
	glGenVertexArrays(1, &m_vertexArrayObject);//分配VAO(顶点数组对象)内存空间，1为设置的缓存ID
	glGenBuffers(1, &m_vertexArrayBuffers);//分配VBO(顶点缓存对象)内存空间
	glGenBuffers(1, &m_elementBufferObject);


	glBindVertexArray(m_vertexArrayObject);//绑定顶点数组对象

										   //2、把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers);//把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
	glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(Vertex), &verticesData[0], GL_STATIC_DRAW);//把用户定义的数据复制到当前绑定缓冲
																										  //glBufferData(目标缓冲的类型,传输数据的大小,发送的实际数据,显卡管理给定的数据的方式)

																										  // 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
	glGenBuffers(1, &m_elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesData.size() * sizeof(indicesData[0]), &indicesData[0], GL_STATIC_DRAW);

	//4、设置顶点属性指针
	glEnableVertexAttribArray(0);//以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);//告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
																			  //glVertexAttribPointer(指定我们要配置的顶点属性，顶点属性的大小（顶点属性是一个vec3，它由3个值组成，所以大小是3），
																			  //指定数据的类型，数据是否被标准化（归一化），步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔（数组紧密排列，所以为0），
																			  //表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0)
																			  //颜色数据
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	//纹理数据
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));

	//法线数据
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	//切线数据
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	//双切线数据
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	glBindVertexArray(0);
}

Mesh::Mesh(VertexPosition * vertices, unsigned int numVertices)
{
	m_drawCount = numVertices;

	//1、绑定顶点数组对象
	glGenVertexArrays(1, &m_vertexArrayObject);//分配VAO(顶点数组对象)内存空间，1为设置的缓存ID
	glBindVertexArray(m_vertexArrayObject);//绑定顶点数组对象

										   //2、把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
	glGenBuffers(1, &m_vertexArrayBuffers);//分配VBO(顶点缓存对象)内存空间
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers);//把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

	//3、设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);//告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
	glEnableVertexAttribArray(0);//以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	glBindVertexArray(0);

}

Mesh::Mesh(std::vector<Vertex> verticesData)
{
	Init(verticesData);
}

void Mesh::Init(std::vector<Vertex> verticesData)
{
	m_drawCount = verticesData.size();

	//1、绑定顶点数组对象
	glGenVertexArrays(1, &m_vertexArrayObject);//分配VAO(顶点数组对象)内存空间，1为设置的缓存ID
	glBindVertexArray(m_vertexArrayObject);//绑定顶点数组对象

										   //2、把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
	glGenBuffers(1, &m_vertexArrayBuffers);//分配VBO(顶点缓存对象)内存空间
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers);//把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
	glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(Vertex), &verticesData[0], GL_STATIC_DRAW);

	//3、设置顶点属性指针
	//顶点数据
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);//告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
	glEnableVertexAttribArray(0);//以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

								 //颜色数据
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//纹理数据
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//法线数据
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	//glDeleteVertexArrays(1, &m_vertexArrayObject);
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

void Mesh::Draw(Shader &shader)
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
	glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}


Mesh::Mesh()
{

}