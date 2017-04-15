#include "..\iostream.h"

SMDMesh::SMDMesh()
{
}

SMDMesh::~SMDMesh()
{
}


void SMDMesh::Finalize()
{

	if (!m_iVAO)
	{
		glGenVertexArrays(1, &m_iVAO);
		glGenBuffers(1, &m_iVBO);
		glGenBuffers(1, &m_iEBO);
	}

	std::vector<float> data;
	for (size_t i = 0; i < m_vPositions.size(); ++i)
	{
		data.push_back(m_vPositions[i].x);
		data.push_back(m_vPositions[i].y);
		data.push_back(m_vPositions[i].z);
		if (m_vNormals.size() > 0)
		{
			data.push_back(m_vNormals[i].x);
			data.push_back(m_vNormals[i].y);
			data.push_back(m_vNormals[i].z);
		}
		if (m_vUVs.size() > 0)
		{
			data.push_back(m_vUVs[i].x);
			data.push_back(m_vUVs[i].y);
		}
		if (m_Weights.size() > 0)
		{
			data.push_back((float)m_Weights[i].iBone);
			data.push_back(m_Weights[i].fWeight);
		}
	}
	glBindVertexArray(m_iVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
	glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), &data[0], GL_STATIC_DRAW);
	if (m_vIndices.size() > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vIndices.size() * sizeof(unsigned int), &m_vIndices[0], GL_STATIC_DRAW);
	}

	size_t stride = 3 * sizeof(float);
	if (m_vNormals.size() > 0) stride += 3 * sizeof(float);
	if (m_vUVs.size() > 0) stride += 2 * sizeof(float);
	if (m_Weights.size() > 0) stride += 2 * sizeof(float);
	size_t offset = 0;

	GLuint location = 0;
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
	offset += 3 * sizeof(float);
	location++;
	if (m_vNormals.size() > 0)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
		offset += 3 * sizeof(float);
		location++;
	}
	if (m_vUVs.size() > 0)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
		offset += 2 * sizeof(float);
		location++;
	}
	if (m_Weights.size() > 0)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
		offset += 2 * sizeof(float);
		location++;
	}
	glBindVertexArray(0);

}