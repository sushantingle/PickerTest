#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>
#include "../Plane.h"

class ExampleLayer : public GLCore::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	GLCore::Utils::Shader* m_Shader;
	GLCore::Utils::PerspectiveCameraController m_CameraController;

	glm::vec3 m_RayStart = glm::vec3(0.0f);
	glm::vec3 m_RayDirection = glm::vec3(0.0f);
	glm::vec3 m_IntersectionPoint = glm::vec3(0.0f);

	bool IsHittingPlane(double xpos, double ypos);
	glm::vec3 GetWorldPos(double xpos, double ypos);
	bool m_MouseDown;
	bool m_IsSelected;

	Plane* m_Plane;
};