#pragma once
#include "Plane.h"

class Triangle
{
public:
	Triangle(const QVector3D& v0, const QVector3D& v1, const QVector3D& v2);
	void set(const QVector3D& v0, const QVector3D& v1, const QVector3D& v2);

	std::optional<float> rayCast(const Ray& ray) const;

private:
	QVector3D m_v0;
	QVector3D m_v1;
	QVector3D m_v2;
	Plane m_plane;
};

inline Triangle::Triangle(const QVector3D& v0, const QVector3D& v1, const QVector3D& v2) 
	: m_v0(v0)
	, m_v1(v1)
	, m_v2(v2)
	, m_plane(v0, v1, v2)
{
}

inline void Triangle::set(const QVector3D& v0, const QVector3D& v1, const QVector3D& v2)
{
	m_v0 = v0;
	m_v1 = v1;
	m_v2 = v2;
	m_plane.set(v0, v1, v2);
}

inline QQuaternion quatBetweenVectors(const QVector3D& from, const QVector3D& to)
{
	QVector3D crossProd = QVector3D::crossProduct(from, to);
	float dotProd = QVector3D::dotProduct(from, to);

	QQuaternion result(dotProd, crossProd);
	result.normalize();
	result.setScalar(result.scalar() + 1.0f);

	if (result.scalar() <= MATH_TINY)
	{
		if (from.z() * from.z() > from.x() * from.x())
			result.setVector(QVector3D(0, from.z(), -from.y()));
		else
			result.setVector(QVector3D(from.y(), -from.x(), 0));
	}

	result.normalize();
	return result;
}

inline std::optional<float> Triangle::rayCast(const Ray& ray) const
{
	if (auto planeDistance = m_plane.rayCast(ray))
	{
		auto planePoint = ray.getPoint(*planeDistance);
		QQuaternion planeRotate = quatBetweenVectors(m_plane.getNormal(), QVector3D(0, 0, 1)).inverted();

		QVector3D planeLocalPoint = planeRotate.rotatedVector(planePoint);
		QVector3D v0 = planeRotate.rotatedVector(m_v0);
		QVector3D v1 = planeRotate.rotatedVector(m_v1);
		QVector3D v2 = planeRotate.rotatedVector(m_v2);

		float AB = (v0.x() - planeLocalPoint.x()) * (v1.y() - planeLocalPoint.y()) - (v1.x() - planeLocalPoint.x()) * (v0.y() - planeLocalPoint.y());
		float BC = (v1.x() - planeLocalPoint.x()) * (v2.y() - planeLocalPoint.y()) - (v2.x() - planeLocalPoint.x()) * (v1.y() - planeLocalPoint.y());
		float CA = (v2.x() - planeLocalPoint.x()) * (v0.y() - planeLocalPoint.y()) - (v0.x() - planeLocalPoint.x()) * (v2.y() - planeLocalPoint.y());
		if ((std::signbit(AB) == std::signbit(BC)) && (std::signbit(BC) == std::signbit(CA)))
		{
			return planeDistance;
		}
	}
	return {};
}