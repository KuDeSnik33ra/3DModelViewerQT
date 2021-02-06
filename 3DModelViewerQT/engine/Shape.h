#pragma once

#include <cmath>
#include <string>
#include <optional>
#include <algorithm>

#include <QMatrix4x4>
#include <QOpenGLBuffer>

#include "Mesh.h"
#include "Material.h"
#include "Line.h"

#include "../math/Ray.h"
#include "../math/Triangle.h"

class SceneManager;

class Shape
{
public:
	Shape(SceneManager* parent, const std::string& id);

	bool createCube(float size);
	bool render(const QMatrix4x4& projView, const QMatrix4x4& model);

public:
	bool setMesh(const MeshPtr& mesh);
	void setMaterial(const MaterialPtr& material);
	void setTransform(const QMatrix4x4& transform);

	void setSelected(bool value);

public:
	const std::string& getID() const;
	const MeshPtr& getMesh() const;
	const MaterialPtr& getMaterial() const;
	const QMatrix4x4& getTransformation() const;

	std::optional<float> rayCast(const Ray& ray) const;

private:
	SceneManager* m_parent;

	std::string m_id;
	MeshPtr m_mesh;
	MaterialPtr m_material;
	QMatrix4x4 m_transform;
	QMatrix4x4 m_invTransform;

	bool m_selected = false;

	std::vector<Triangle> m_triangles;
	QOpenGLBuffer m_arrayBuf;
	QOpenGLBuffer m_indexBuf;

	std::vector<LinePtr> m_lines;
};

using ShapePtr = std::shared_ptr<Shape>;