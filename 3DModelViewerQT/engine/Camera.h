#pragma once

#include <QVector3D>
#include <QMatrix4x4>

class Camera
{
public:
	void setPerspective(float verticalAngle, float aspectRatio, float nearPlane, float farPlane);
	void setTransform(const QMatrix4x4& transform);

	const QMatrix4x4& getTransform() const;
	const QMatrix4x4& getProjView();

private:
	QMatrix4x4 m_transform;
	QMatrix4x4 m_projection;

	bool m_changes;
	QMatrix4x4 m_projView;	
};