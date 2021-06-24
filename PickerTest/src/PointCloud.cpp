#include "PointCloud.h"
#include "../TransformUtil.h"

using namespace GLCore;
using namespace GLCore::Utils;

PointCloud::PointCloud(const GLCore::Window& window)
	: m_Window(window),
	  m_CameraController(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 1000.0f, glm::vec3(15.0f, 40.0f, 75.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f))
{
	m_MouseGesture = new MouseGesture(window, m_CameraController);
}

PointCloud::~PointCloud()
{

}

void PointCloud::OnAttach()
{
	EnableGLDebugging();

	m_Points.reserve(10);
	glm::vec3 startPos = glm::vec3(0.0f);
	for (int i = 0; i < 300; ++i)
	{
		m_Points.push_back(new Point(m_CameraController));

		startPos.x = (i % 20) * 2;
		if (i % 20 == 0) startPos.x = 0;
		startPos.y = (int)(i / 20) * 2;
		m_Points[i]->UpdatePosition(startPos);
	}
}

void PointCloud::OnDetach()
{
	for (Point* plane : m_Points)
		delete plane;
	m_Points.clear();

	delete m_MouseGesture;
	m_MouseGesture = nullptr;
}

void PointCloud::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);

	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseButtonPressedEvent>(
		[&](MouseButtonPressedEvent& e)
	{
		m_MouseDown = true;
		m_MouseGesture->OnMouseDragStart(e.GetXPos(), e.GetYPos());
		return false;
	});
	dispatcher.Dispatch<MouseButtonReleasedEvent>(
		[&](MouseButtonReleasedEvent& e)
	{
		m_MouseDown = false;
		m_MouseGesture->OnMouseDragEnd(e.GetXPos(), e.GetYPos());

		SelectPointsInGestureArea();
		return false;
	});

	dispatcher.Dispatch<MouseMovedEvent>(
		[&](MouseMovedEvent& e)
	{

		if (m_MouseDown)
		{
			m_MouseGesture->OnMouseDragMove(e.GetX(), e.GetY());
			SelectPointsInGestureArea();
		}

		return false;
	});
}

bool PointCloud::IsHittingPlane(double xpos, double ypos)
{
	glm::vec3 rayStart = TransformUtil::ConvertScreenToWorld(xpos, ypos, 1280.0f, 720.0f, m_CameraController);
	glm::vec3 rayDirection = glm::normalize(glm::vec3(glm::vec3(rayStart) - m_CameraController.GetCamera().GetPosition()));
	glm::vec3 intersectionPoint = glm::vec3(0.0f);
	for (int i = 0; i < m_Points.size(); ++i)
	{
		if (m_Points[i]->RayhitTest(rayStart, rayDirection, intersectionPoint))
		{
			m_PickedPlaneIndex = i;
			return true;
		}
	}
	return false;
}

void PointCloud::SelectPointsInGestureArea()
{
	const BoundingBox& rect = m_MouseGesture->GetGestureBox();
	glm::vec3 leftScreenPos = TransformUtil::ConvertNDCToScreen(m_MouseGesture->GetDragStartPosition(), m_Window.GetWidth(), m_Window.GetHeight());
	glm::vec3 rightScreenPos = TransformUtil::ConvertNDCToScreen(m_MouseGesture->GetDragEndPosition(), m_Window.GetWidth(), m_Window.GetHeight());

	glm::vec3 leftWorldPos = TransformUtil::ConvertScreenToWorld(leftScreenPos.x, leftScreenPos.y, m_Window.GetWidth(), m_Window.GetHeight(), m_CameraController);
	glm::vec3 rightWorldPos = TransformUtil::ConvertScreenToWorld(rightScreenPos.x, rightScreenPos.y, m_Window.GetWidth(), m_Window.GetHeight(), m_CameraController);

	if (glm::length(rightWorldPos - leftWorldPos) == 0.0f) return;

	glm::vec3 planeSideA = glm::vec3(rightWorldPos.x, leftWorldPos.y, leftWorldPos.z) - leftWorldPos;
	glm::vec3 planeSideB = glm::vec3(leftWorldPos.x, rightWorldPos.y, leftWorldPos.z) - leftWorldPos;
	glm::vec3 normal = glm::normalize(glm::cross(planeSideA, planeSideB));

	float minX = glm::min(leftWorldPos.x, rightWorldPos.x);
	float maxX = glm::max(leftWorldPos.x, rightWorldPos.x);

	float minY = glm::min(leftWorldPos.y, rightWorldPos.y);
	float maxY = glm::max(leftWorldPos.y, rightWorldPos.y);

	for (Point* point : m_Points)
	{
		glm::vec3 intersectionPoint = glm::vec3(0.0f);
		bool selected = false;
		if (point->RayhitTest(normal, leftWorldPos, intersectionPoint))
		{
			if ((intersectionPoint.x < minX || intersectionPoint.x > maxX) ||
				(intersectionPoint.y < minY || intersectionPoint.y > maxY))
			{
				selected = false;
			}
			else
				selected = true;
		}
		point->OnPicked(selected);
		point->SetPointCloudType(selected ? m_CurrentPointCloudPointType : PointCloudPointType::POINT);
	}
}

void PointCloud::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto plane : m_Points)
		plane->Update();

	m_MouseGesture->Render();
}

void PointCloud::OnImGuiRender()
{
	ImGui::Begin("Controls");
	bool selected = false;

	if (ImGui::BeginMenu("Geom Type", true))
	{
		if (ImGui::MenuItem("Triangle", "", &selected))
		{
			m_CurrentPointCloudPointType = PointCloudPointType::TRIANGLE;
		}

		if (ImGui::MenuItem("Rectangle", "", &selected))
		{
			m_CurrentPointCloudPointType = PointCloudPointType::RECTANGLE;
		}

		if (ImGui::MenuItem("Point", "", &selected))
		{
			m_CurrentPointCloudPointType = PointCloudPointType::POINT;
		}
		ImGui::EndMenu();
	}
	ImGui::End();
	m_MouseGesture->OnImGuiRender();
}
