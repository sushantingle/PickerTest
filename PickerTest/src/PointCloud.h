#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>
#include "Point.h"
#include "MouseGesture.h"

class PointCloud : public GLCore::Layer
{
public:
	PointCloud(const GLCore::Window& window);
	virtual ~PointCloud();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	GLCore::Utils::PerspectiveCameraController m_CameraController;
	MouseGesture* m_MouseGesture;
	bool IsHittingPlane(double xpos, double ypos);
	void SelectPointsInGestureArea();

	bool m_MouseDown = false;
	int m_PickedPlaneIndex = -1;
	std::vector<Point*> m_Points;
	const GLCore::Window& m_Window;

	PointCloudPointType m_CurrentPointCloudPointType = PointCloudPointType::TRIANGLE;
};