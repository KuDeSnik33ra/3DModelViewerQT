#include "Material.h"
#include <QImage>

Material::~Material()
{
	m_texture = nullptr;
}

bool Material::setVertexShaderFromSourceFile(const QString& fileName)
{
	return (m_changes = m_shaders.addShaderFromSourceFile(QOpenGLShader::Vertex, fileName));
}

bool Material::setFragmentShaderFromSourceFile(const QString& fileName)
{
	return (m_changes = m_shaders.addShaderFromSourceFile(QOpenGLShader::Fragment, fileName));
}

bool Material::setTexture(const QString& fileName)
{
	QImage image(fileName);
	if (image.isNull())
	{
		return false;
	}
		
	m_texture = std::make_shared<QOpenGLTexture>(image.mirrored());
	m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
	m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
	m_texture->setWrapMode(QOpenGLTexture::Repeat);
	return true;
}

void Material::setColor(const std::optional<QColor>& color)
{
	m_color = color;
}

bool Material::bind(const QMatrix4x4& projView, const QMatrix4x4& model)
{
	if (m_changes && !m_shaders.link())
	{
		return false;
	}
	if (!m_shaders.bind())
	{
		return false;
	}
	if (m_color)
	{
		m_shaders.setUniformValue("color", *m_color);
	}
	if (m_texture)
	{
		m_texture->bind();
		m_shaders.setUniformValue("texture", 0);
	}
	m_shaders.setUniformValue("mvp_matrix", projView * model);
	return true;
}

QOpenGLShaderProgram& Material::getShaders()
{
	return m_shaders;
}