#include "PointCloud.h"
#include "../TransformUtil.h"

using namespace GLCore;
using namespace GLCore::Utils;

PointCloud::PointCloud(const GLCore::Window& window)
	: m_Window(window),
	  m_CameraController(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 1000.0f, glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f))
{
	m_MouseGesture = new MouseGesture(window, m_CameraController);
}

PointCloud::~PointCloud()
{

}

void PointCloud::OnAttach()
{
	EnableGLDebugging();

	m_Shader = Shader::FromGLSLTextFiles(
		"assets/shaders/test.vert.glsl",
		"assets/shaders/test.frag.glsl"
	);

	m_Points.reserve(10);
	glm::vec3 startPos = glm::vec3(0.0f);
	for (int i = 0; i < 100; ++i)
	{
		m_Points.push_back(new Point(m_Shader, m_CameraController));

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
		if (IsHittingPlane(e.GetXPos(), e.GetYPos()))
		{
			m_Points[m_PickedPlaneIndex]->OnPicked(true);
		}
		m_MouseGesture->OnMouseDragStart(e.GetXPos(), e.GetYPos());
		return false;
	});
	dispatcher.Dispatch<MouseButtonReleasedEvent>(
		[&](MouseButtonReleasedEvent& e)
	{
		m_MouseDown = false;
		if (m_PickedPlaneIndex != -1)
			m_Points[m_PickedPlaneIndex]->OnPicked(false);
		m_PickedPlaneIndex = -1;
		m_MouseGesture->OnMouseDragEnd(e.GetXPos(), e.GetYPos());
		return false;
	});

	dispatcher.Dispatch<MouseMovedEvent>(
		[&](MouseMovedEvent& e)
	{

		if (m_MouseDown)
		{
			m_MouseGesture->OnMouseDragMove(e.GetX(), e.GetY());
			if (m_PickedPlaneIndex != -1)
			{
				glm::vec3 rayStart = TransformUtil::ConvertScreenToWorld(e.GetX(), e.GetY(), 1280.0f, 720.0f, m_CameraController);
				glm::vec3 rayDirection = glm::normalize(glm::vec3(glm::vec3(rayStart) - m_CameraController.GetCamera().GetPosition()));
				m_Points[m_PickedPlaneIndex]->FollowRay(rayStart, rayDirection);
			}
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
	ImGui::Text("Mouse : %s", m_MouseDown ? "Down" : "Up");
	ImGui::Text("Picked Plane Index : %d", m_PickedPlaneIndex);
	ImGui::End();
	m_MouseGesture->OnImGuiRender();
}
