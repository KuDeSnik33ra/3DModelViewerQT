#pragma once

#include <memory>

#include <QVector>
#include <QVector2D>
#include <QVector3D>

class Mesh
{
public:
	struct VertexData
	{
		QVector3D position;
		QVector2D texCoord;
	};

public:
	void createCube(float size);

public:
	void addTriangleIndices(uint32_t v0, uint32_t v1, uint32_t v2);
	void addRectIndices(uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3);

public:
	const QVector<VertexData>& getVertices() const;
	const QVector<uint32_t>& getIndices() const;

private:
	QVector<VertexData> m_vertices;
	QVector<uint32_t> m_indices;
};

using MeshPtr = std::shared_ptr<Mesh>;