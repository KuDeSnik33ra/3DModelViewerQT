#pragma once

#define MATH_TINY 0.001f

class Plane
{
public:
	Plane(const QVector3D& v0, const QVector3D& v1, const QVector3D& v2);
	void set(const QVector3D& v0, const QVector3D& v1, const QVector3D& v2);

	float getDistance() const;
	const QVector3D& getNormal() const;

	std::optional<float> rayCast(const Ray& ray) const;

private:
	union
	{
		float m_data[4];

		struct
		{
			QVector3D m_normal;
			float m_distance;
		};
	};
	
};

inline Plane::Plane(const QVector3D& v0, const QVector3D& v1, const QVector3D& v2)
{
	m_distance = 0.0f;
	set(v0, v1, v2);
}

inline float Plane::getDistance() const
{
	return m_distance;
}

inline const QVector3D& Plane::getNormal() const
{
	return m_normal;
}

inline void Plane::set(const QVector3D& v0, const QVector3D& v1, const QVector3D& v2)
{
	m_normal = QVector3D::crossProduct(v1 - v0, v2 - v0).normalized();
	m_distance = QVector3D::dotProduct(m_normal, v0);
}

inline std::optional<float> Plane::rayCast(const Ray& ray) const
{
	float dotDir = QVector3D::dotProduct(getNormal(), ray.getDir());
	if (!std::isfinite(dotDir) || (std::abs(dotDir) < MATH_TINY))
	{
		return std::nullopt;
	}
	float dotStart = QVector3D::dotProduct(getNormal(), ray.getStart());
	
	return (getDistance() - dotStart) / dotDir;
}