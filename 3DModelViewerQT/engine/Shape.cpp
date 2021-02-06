#include "Shape.h"
#include "SceneManager.h"

Shape::Shape(SceneManager* parent, const std::string& id)
	: m_parent(parent)
	, m_id(id)
	, m_indexBuf(QOpenGLBuffer::IndexBuffer)
{
	m_arrayBuf.create();
	m_indexBuf.create();
}

bool Shape::createCube(float size)
{
	auto mesh = std::make_shared<Mesh>();
	mesh->createCube(size);
	return setMesh(mesh);
}

bool Shape::render(const QMatrix4x4& projView, const QMatrix4x4& model)
{
	if (!m_material)
	{
		return false;
	}

	if (!m_mesh)
	{
		return false;
	}

	if (!m_material->bind(projView, model))
	{
		return false;
	}

	auto& shaders = m_material->getShaders();

	if (!m_arrayBuf.bind())
	{
		return false;
	}
	if (!m_indexBuf.bind())
	{
		return false;
	}

	quintptr offset = 0;
	int vertexLocation = shaders.attributeLocation("a_position");
	shaders.enableAttributeArray(vertexLocation);
	shaders.setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(Mesh::VertexData));

	offset += sizeof(QVector3D);
	int texcoordLocation = shaders.attributeLocation("a_texcoord");
	shaders.enableAttributeArray(texcoordLocation);
	shaders.setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(Mesh::VertexData));

	m_parent->glEnable(GL_CULL_FACE);
	m_parent->glDrawElements(GL_TRIANGLES, m_mesh->getIndices().size(), GL_UNSIGNED_INT, nullptr);

	for (const auto& line : m_lines)
	{
		line->render(projView, model);
	}
	return true;
}

//

bool Shape::setMesh(const MeshPtr& mesh)
{
	if (!mesh)
	{
		return false;
	}
	auto vertices = mesh->getVertices();
	auto indices = mesh->getIndices();

	if (!m_arrayBuf.bind())
	{
		return false;
	}
	m_arrayBuf.allocate(vertices.data(), vertices.size() * sizeof(Mesh::VertexData));

	if (!m_indexBuf.bind())
	{
		return false;
	}
	m_indexBuf.allocate(indices.data(), indices.size() * sizeof(uint32_t));

	for (int index = 0; index < indices.size(); index += 3)
	{
		auto v0 = vertices[indices[index]].position;
		auto v1 = vertices[indices[index + 1]].position;
		auto v2 = vertices[indices[index + 2]].position;
		m_triangles.emplace_back(v0, v1, v2);
	}

	m_mesh = mesh;
	return true;
}

void Shape::setMaterial(const MaterialPtr& material)
{
	m_material = material;
}

void Shape::setTransform(const QMatrix4x4& transform)
{
	m_transform = transform;
	m_invTransform = m_transform.inverted();
}

void Shape::setSelected(bool value)
{
	m_lines.clear();
	if (value)
	{
		auto lineX = m_lines.emplace_back(std::make_shared<Line>(m_parent));
		lineX->create(QVector3D(), QVector3D(50, 0, 0), 1.0f, QColor(0, 0, 255, 255));
		auto lineY = m_lines.emplace_back(std::make_shared<Line>(m_parent));
		lineY->create(QVector3D(), QVector3D(0, 50, 0), 1.0f, QColor(0, 255, 0, 255));
		auto lineZ = m_lines.emplace_back(std::make_shared<Line>(m_parent));
		lineZ->create(QVector3D(), QVector3D(0, 0, 50), 1.0f, QColor(255, 0, 0, 255));
	}
	m_selected = value;
}

//

const std::string& Shape::getID() const
{
	return m_id;
}

const MeshPtr& Shape::getMesh() const
{
	return m_mesh;
}

const MaterialPtr& Shape::getMaterial() const
{
	return m_material;
}

const QMatrix4x4& Shape::getTransformation() const
{
	return m_transform;
}

std::optional<float> Shape::rayCast(const Ray& ray) const
{
	std::optional<float> result;
	auto localRay = ray.getTransformed(m_invTransform);
	for (const auto& triangle : m_triangles)
	{
		if (auto dst = triangle.rayCast(localRay))
		{
			if (!result || (*result > dst))
			{
				result = dst;
			}
		}
	}
	return result;
}