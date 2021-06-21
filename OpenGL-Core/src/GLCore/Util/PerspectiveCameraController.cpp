#include "glpch.h"
#include "PerspectiveCameraController.h"

#include "GLCore/Core/Input.h"
#include "GLCore/Core/KeyCodes.h"
#include <glfw/glfw3.h>
#include "glm/gtc/matrix_transform.hpp"

#include <GLCore.h>
#include <GLCoreUtils.h>
using namespace GLCore;

namespace GLCore::Utils {

	PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio, float zNear, float zFar, glm::vec3 eyePosition, glm::vec3 upVector, glm::vec3 lookAt) :
		m_Camera(fov, aspectRatio, zNear, zFar, eyePosition, upVector, lookAt)
	{
		m_CameraRotationSpeed = 0.25f;
		m_CameraTranslationSpeed = 5;
		m_MouseSpeed = 0.2f;
		m_LastMousePos = glm::vec2(0, 0);
		m_Pitch = -27.0f;
		m_Roll = 0.0f;
		m_Yaw = -90.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)) * -1.0f;
		direction.y = sin(glm::radians(m_Pitch));
		direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

		m_Camera.SetLookAt(glm::normalize(direction));
	}

	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		// Uniform Buffer Object


		glm::vec3 camPosition = m_Camera.GetPosition();


		if (Input::IsKeyPressed(HZ_KEY_A))
		{
			camPosition -= glm::normalize(glm::cross(m_Camera.GetLookAt(), m_Camera.GetUpVector())) * m_CameraTranslationSpeed;
		}
		else if (Input::IsKeyPressed(HZ_KEY_D))
		{
			camPosition += glm::normalize(glm::cross(m_Camera.GetLookAt(), m_Camera.GetUpVector())) * m_CameraTranslationSpeed;
		}

		if (Input::IsKeyPressed(HZ_KEY_W))
		{
			camPosition += m_Camera.GetLookAt() * m_CameraTranslationSpeed;
		}
		else if (Input::IsKeyPressed(HZ_KEY_S))
		{
			camPosition -= m_Camera.GetLookAt() * m_CameraTranslationSpeed;
		}

		if (Input::IsKeyPressed(HZ_KEY_ESCAPE))
		{
			m_UseMouse = !m_UseMouse;
		}

		m_Camera.SetPosition(camPosition);
		m_Camera.CalculateMVP();
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseMovedEvent>(GLCORE_BIND_EVENT_FN(PerspectiveCameraController::OnMouseMoved));
		dispatcher.Dispatch<WindowResizeEvent>(GLCORE_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
	}

	bool PerspectiveCameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		if (!m_UseMouse) return false;

		static bool firstTime = true;
		if (firstTime)
		{
			m_LastMousePos = glm::vec2(e.GetX(), e.GetY());
			firstTime = false;
		}

		float deltaX = (m_LastMousePos.x - e.GetX()) * m_MouseSpeed;
		float deltaY = (m_LastMousePos.y - e.GetY()) * m_MouseSpeed;

		m_Yaw += deltaX;
		m_Pitch += deltaY;

		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)) * -1.0f;
		direction.y = sin(glm::radians(m_Pitch));
		direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

		m_Camera.SetLookAt(glm::normalize(direction));
		m_LastMousePos = glm::vec2(e.GetX(), e.GetY());
		return false;
	}

	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		float aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetAspectRatio(aspectRatio);

		return false;
	}
}