#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->textures = textures;
	this->indices = indices;

	setupMesh();
}

void Mesh::Draw(Shader& shader)
{
	shader.Bind();
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		//glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);
			textures[i].Bind(i);
			shader.SetUniform1i("material.diffuse", i);
			
		}
		else if (name == "texture_specular")
		{
			number = std::to_string(specularNr++);
			textures[i].Bind(i);
			shader.SetUniform1i("material.specular", i);
		}


	}
	//textures[0].Bind();
	//shader.SetUniform1i("material.diffuse", 0);
	//textures[0].UnBind();
	shader.SetUniformMat4f("u_model", world);
	vertexArray->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
		glBindVertexArray(0);
}

void Mesh::setupMesh()
{

	vertexArray = new VertexArray();
	vertexArray->Bind();
	indexBuffer = new IndexBuffer(&indices[0], indices.size());
	vertexBuffer = new VertexBuffer(&vertices[0], vertices.size() * sizeof(Vertex));

	vertexBufferLayout = new VertexBufferLayout();
	vertexBufferLayout->Push<float>(3);
	vertexBufferLayout->Push<float>(3);
	vertexBufferLayout->Push<float>(2);

	vertexArray->AddBuffer(*vertexBuffer, *vertexBufferLayout);
	glBindVertexArray(0);

}
