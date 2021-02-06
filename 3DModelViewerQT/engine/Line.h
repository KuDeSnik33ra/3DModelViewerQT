#pragma once

#include <QMatrix4x4>
#include <QOpenGLBuffer>

#include "Material.h"

class SceneManager;

class Line
{
public:
	Line(SceneManager* parent);

	bool create(const QVector3D& p0, const QVector3D& p1, float strength, const QColor& color);
	bool render(const QMatrix4x4& projView, const QMatrix4x4& model);

private:
	SceneManager* m_parent;

	MaterialPtr m_material;
	QOpenGLBuffer m_arrayBuf;
	QOpenGLBuffer m_indexBuf;
};

using LinePtr = std::shared_ptr<Line>;