#pragma once

#include <optional>

#include <QColor>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class Material
{
public:
	~Material();

	bool setVertexShaderFromSourceFile(const QString& fileName);
	bool setFragmentShaderFromSourceFile(const QString& fileName);
	bool setTexture(const QString& fileName);
	void setColor(const std::optional<QColor>& color);

	bool bind(const QMatrix4x4& projView, const QMatrix4x4& model);

public:
	QOpenGLShaderProgram& getShaders();

private:
	bool m_changes = false;
	QOpenGLShaderProgram m_shaders;

	std::optional<QColor> m_color;
	std::shared_ptr<QOpenGLTexture> m_texture;
};

using MaterialPtr = std::shared_ptr<Material>;