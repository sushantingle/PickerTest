#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>
#include "../Plane.h"

class PickerTest : public GLCore::Layer
{
public:
	PickerTest();
	virtual ~PickerTest();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	GLCore::Utils::Shader* m_Shader;
	GLCore::Utils::PerspectiveCameraController m_CameraController;

	bool IsHittingPlane(double xpos, double ypos);

	bool m_MouseDown = false;
	int m_PickedPlaneIndex = -1;
	std::vector<Plane*> m_Plane;
};