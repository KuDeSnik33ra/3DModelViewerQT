#include "Camera.h"

void Camera::setTransform(const QMatrix4x4& transform)
{
	m_changes = true;
	m_transform = transform;
}

const QMatrix4x4& Camera::getTransform() const
{
	return m_transform;
}

void Camera::setPerspective(float verticalAngle, float aspectRatio, float nearPlane, float farPlane)
{
	m_changes = true;
	m_projection.setToIdentity();
	m_projection.perspective(verticalAngle, aspectRatio, nearPlane, farPlane);
}

const QMatrix4x4& Camera::getProjView()
{
	if (m_changes)
	{
		auto viewMat = m_transform.inverted();
		m_projView = m_projection * viewMat;
		m_changes = false;
	}
	return m_projView;
}