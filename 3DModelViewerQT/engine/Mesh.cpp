#include "Mesh.h"

void Mesh::createCube(float size)
{
	float halfSize = size / 2.0f;

	m_vertices.push_back({ QVector3D(halfSize, halfSize, halfSize), QVector2D(0.0f, 0.0f) });
	m_vertices.push_back({ QVector3D(halfSize, -halfSize, halfSize), QVector2D(0.0f, 1.0f) });
	m_vertices.push_back({ QVector3D(-halfSize, -halfSize, halfSize), QVector2D(1.0f, 1.0f) });
	m_vertices.push_back({ QVector3D(-halfSize, halfSize, halfSize), QVector2D(1.0f, 0.0f) });

	m_vertices.push_back({ QVector3D(halfSize, halfSize, -halfSize), QVector2D(1.0f, 0.0f) });
	m_vertices.push_back({ QVector3D(halfSize, -halfSize, -halfSize), QVector2D(1.0f, 1.0f) });
	m_vertices.push_back({ QVector3D(-halfSize, -halfSize, -halfSize), QVector2D(0.0f, 1.0f) });
	m_vertices.push_back({ QVector3D(-halfSize, halfSize, -halfSize), QVector2D(0.0f, 0.0f) });

	// Top
	addRectIndices(0, 1, 2, 3);
	// Right
	addRectIndices(0, 4, 5, 1);
	// Back
	addRectIndices(1, 5, 6, 2);
	// Left
	addRectIndices(2, 6, 7, 3);
	// Front
	addRectIndices(3, 7, 4, 0);
	// Bottom
	addRectIndices(7, 6, 5, 4);
}

void Mesh::addTriangleIndices(uint32_t v0, uint32_t v1, uint32_t v2)
{
	m_indices.push_back(v0);
	m_indices.push_back(v1);
	m_indices.push_back(v2);
}

void Mesh::addRectIndices(uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3)
{
	addTriangleIndices(v2, v1, v0);
	addTriangleIndices(v0, v3, v2);
}

const QVector<Mesh::VertexData>& Mesh::getVertices() const
{
	return m_vertices;
}

const QVector<uint32_t>& Mesh::getIndices() const
{
	return m_indices;
}