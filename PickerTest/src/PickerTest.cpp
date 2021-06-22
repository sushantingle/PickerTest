#include "PickerTest.h"
#include "../TransformUtil.h"

using namespace GLCore;
using namespace GLCore::Utils;

PickerTest::PickerTest()
	: m_CameraController(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 1000.0f, glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f))
{
	
}

PickerTest::~PickerTest()
{

}

void PickerTest::OnAttach()
{
	EnableGLDebugging();

	m_Shader = Shader::FromGLSLTextFiles(
		"assets/shaders/test.vert.glsl",
		"assets/shaders/test.frag.glsl"
	);

	m_Plane.reserve(10);
	glm::vec3 startPos = glm::vec3(0.0f);
	for (int i = 0; i < 100; ++i)
	{
		m_Plane.push_back(new Plane(m_Shader, m_CameraController));
		
		startPos.x = (i % 20) * 2;
		if (i % 20 == 0) startPos.x = 0;
		startPos.y = (int)(i / 20) * 2;
		m_Plane[i]->UpdatePosition(startPos);
	}
}

void PickerTest::OnDetach()
{
	for(Plane* plane : m_Plane)
		delete plane;
	m_Plane.clear();
}

void PickerTest::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);

	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseButtonPressedEvent>(
		[&](MouseButtonPressedEvent& e)
		{
		m_MouseDown = true;
		if (IsHittingPlane(e.GetXPos(), e.GetYPos()))
		{
			m_Plane[m_PickedPlaneIndex]->OnPicked(true);
		}

		return false;
		});
	dispatcher.Dispatch<MouseButtonReleasedEvent>(
		[&](MouseButtonReleasedEvent& e)
		{
			m_MouseDown = false;
			if (m_PickedPlaneIndex != -1)
				m_Plane[m_PickedPlaneIndex]->OnPicked(false);
			m_PickedPlaneIndex = -1;
			return false;
		});

	dispatcher.Dispatch<MouseMovedEvent>(
		[&](MouseMovedEvent& e)
	{

		if (m_MouseDown && m_PickedPlaneIndex != -1)
		{
			glm::vec3 rayStart = TransformUtil::ConvertScreenToWorld(e.GetX(), e.GetY(), 1280.0f, 720.0f, m_CameraController);
			glm::vec3 rayDirection = glm::normalize(glm::vec3(glm::vec3(rayStart) - m_CameraController.GetCamera().GetPosition()));
			m_Plane[m_PickedPlaneIndex]->FollowRay(rayStart, rayDirection);
		}

		return false;
	});
}

bool PickerTest::IsHittingPlane(double xpos, double ypos)
{
	glm::vec3 rayStart = TransformUtil::ConvertScreenToWorld(xpos, ypos, 1280.0f, 720.0f, m_CameraController);
	glm::vec3 rayDirection = glm::normalize(glm::vec3(glm::vec3(rayStart) - m_CameraController.GetCamera().GetPosition()));
	glm::vec3 intersectionPoint = glm::vec3(0.0f);
	for (int i = 0; i < m_Plane.size(); ++i)
	{
		if (m_Plane[i]->RayhitTest(rayStart, rayDirection, intersectionPoint))
		{
			m_PickedPlaneIndex = i;
			return true;
		}
	}
	return false;
}

void PickerTest::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(auto plane : m_Plane)
		plane->Update();
}

void PickerTest::OnImGuiRender()
{
	ImGui::Begin("Controls");
	ImGui::Text("Mouse : %s", m_MouseDown ? "Down" : "Up");
	ImGui::Text("Picked Plane Index : %d", m_PickedPlaneIndex);
	ImGui::End();
}
