#pragma once

class Ray
{
public:
	Ray(const QVector3D& start, const QVector3D& dir);
	const QVector3D& getStart() const;
	const QVector3D& getDir() const;

	QVector3D getPoint(float distance) const;
	Ray getTransformed(const QMatrix4x4& transform) const;

private:
	QVector3D m_start;
	QVector3D m_dir;
};

inline Ray::Ray(const QVector3D& start, const QVector3D& dir)
	: m_start(start)
	, m_dir(dir)
{
}

inline const QVector3D& Ray::getStart() const
{
	return m_start;
}

inline const QVector3D& Ray::getDir() const
{
	return m_dir;
}

inline QVector3D Ray::getPoint(float distance) const
{
	return m_start + m_dir * distance;
}

inline Ray Ray::getTransformed(const QMatrix4x4& transform) const
{
	auto newStart = transform * m_start;
	auto newDir = transform * (m_start + m_dir) - newStart;
	return Ray(newStart, newDir);
}