#include "glpch.h"
#include "PerspectiveCamera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace GLCore::Utils {

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float zNear, float zFar, glm::vec3 eyePosition, glm::vec3 upVector, glm::vec3 lookAt)
	{
		m_Position = eyePosition;
		m_Rotation = 0.0f;
		m_Zoom = fov;
		m_LookAt = lookAt;
		m_UpVector = upVector;
		m_AspectRatio = aspectRatio;
		m_zNear = zNear;
		m_zFar = zFar;

		CalculateMVP();
	}

	void PerspectiveCamera::CalculateMVP()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_LookAt, m_UpVector);
		m_ProjectionMatrix = glm::perspective(m_Zoom, m_AspectRatio, m_zNear, m_zFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}