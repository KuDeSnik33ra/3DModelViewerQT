#include "Line.h"
#include "SceneManager.h"

Line::Line(SceneManager* parent)
	: m_parent(parent)
	, m_indexBuf(QOpenGLBuffer::IndexBuffer)
{
	m_arrayBuf.create();
	m_indexBuf.create();
}

MaterialPtr createMaterial(const QColor& color)
{
	auto material = std::make_shared<Material>();
	if (!material->setVertexShaderFromSourceFile(":/vshader.glsl"))
	{
		return {};
	}
	if (!material->setFragmentShaderFromSourceFile(":/fshaderColor.glsl"))
	{
		return {};
	}
	material->setColor(color);
	return material;
}

bool Line::create(const QVector3D& p0, const QVector3D& p1, float strength, const QColor& color)
{
	Mesh::VertexData vertices[] = {
		{QVector3D(p0.x() - strength, p0.y() - strength,  p0.z() - strength), QVector2D(1.0f, 1.0f)},
		{QVector3D(p0.x() + strength, p0.y() + strength,  p0.z() + strength), QVector2D(1.0f, 1.0f)},
		{QVector3D(p1.x() - strength, p1.y() - strength,  p1.z() - strength), QVector2D(1.0f, 1.0f)},
		{QVector3D(p1.x() + strength, p1.y() + strength,  p1.z() + strength), QVector2D(1.0f, 1.0f)}
	};

	if (!m_arrayBuf.bind())
	{
		return false;
	}
	m_arrayBuf.allocate(vertices, sizeof(vertices));

	GLushort indices[] = {
		 0, 1, 2, 1, 2, 3
	};

	if (!m_indexBuf.bind())
	{
		return false;
	}
	m_indexBuf.allocate(indices, sizeof(indices));

	m_material = createMaterial(color);
	return true;
}

bool Line::render(const QMatrix4x4& projView, const QMatrix4x4& model)
{
	if (!m_material)
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

	m_parent->glDisable(GL_CULL_FACE);
	m_parent->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
	return true;
}