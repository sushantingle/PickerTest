#pragma once
#include "glm/glm.hpp"

namespace GLCore::Utils {

	class PerspectiveCamera {
	public:

		PerspectiveCamera(float fov, float aspectRatio, float zNear, float zFar, glm::vec3 eyePosition, glm::vec3 upVector, glm::vec3 lookAt);
		~PerspectiveCamera() {}

		void SetPosition(const glm::vec3& position) { m_Position = position; }
		const glm::vec3& GetPosition() const { return m_Position; }

		void SetRotation(float rotation) { m_Rotation = rotation; }
		const float GetRotation() const { return m_Rotation; }

		void SetLookAt(const glm::vec3& lookAt) { m_LookAt = lookAt; }
		const glm::vec3& GetLookAt() const { return m_LookAt; }

		void SetZoom(float rotation) { m_Zoom = rotation;  }
		const float GetZoom() const { return m_Zoom; }

		const glm::vec3& GetUpVector() const { return m_UpVector; }

		void SetAspectRatio(float value) { m_AspectRatio = value; }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		void CalculateMVP();
	private:


		glm::vec3 m_Position;
		glm::vec3 m_LookAt;
		glm::vec3 m_UpVector;
		float m_Rotation;
		float m_Zoom;
		float m_AspectRatio;
		float m_zNear;
		float m_zFar;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

	};
}