#pragma once
#include "PerspectiveCamera.h"
#include "GLCore/Core/Timestep.h"

#include "GLCore/Events/ApplicationEvent.h"
#include "GLCore/Events/MouseEvent.h"

namespace GLCore::Utils {
	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(float fov, float aspectRatio, float zNear, float zFar, glm::vec3 eyePosition, glm::vec3 upVector, glm::vec3 lookAt);
		~PerspectiveCameraController() {}

		const PerspectiveCamera& GetCamera() const { return m_Camera; }

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

	private:
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		PerspectiveCamera m_Camera;

		float m_CameraRotationSpeed;
		float m_CameraTranslationSpeed;
		glm::vec2 m_LastMousePos;
		float m_MouseSpeed;
		float m_Roll;
		float m_Pitch = -27.0f;
		float m_Yaw;

		bool m_UseMouse = false;

	};
}