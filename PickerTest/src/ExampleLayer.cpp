#include "ExampleLayer.h"
#include "../TransformUtil.h"

using namespace GLCore;
using namespace GLCore::Utils;

ExampleLayer::ExampleLayer()
	: m_CameraController(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 1000.0f, glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f))
{
	
}

ExampleLayer::~ExampleLayer()
{

}

void ExampleLayer::OnAttach()
{
	EnableGLDebugging();

	m_Shader = Shader::FromGLSLTextFiles(
		"assets/shaders/test.vert.glsl",
		"assets/shaders/test.frag.glsl"
	);

	m_Plane = new Plane(m_Shader, m_CameraController);
}

void ExampleLayer::OnDetach()
{
	delete m_Plane;
	m_Plane = nullptr;
}

void ExampleLayer::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);

	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseButtonPressedEvent>(
		[&](MouseButtonPressedEvent& e)
		{
		m_MouseDown = true;
			// std::cout << "Position : " << e.GetXPos() << "	" << e.GetYPos() << std::endl;
		if (IsHittingPlane(e.GetXPos(), e.GetYPos()))
			m_Plane->OnPicked(true);
		else
			m_Plane->OnPicked(false);

			return false;
		});
	dispatcher.Dispatch<MouseButtonReleasedEvent>(
		[&](MouseButtonReleasedEvent& e)
		{
			m_MouseDown = false;
			m_Plane->OnPicked(false);
			return false;
		});

	dispatcher.Dispatch<MouseMovedEvent>(
		[&](MouseMovedEvent& e)
	{

		/*if (m_MouseDown && IsHittingPlane(e.GetX(), e.GetY()))
			m_SquareColor = m_SquareAlternateColor;*/

		return false;
	});
}

bool ExampleLayer::IsHittingPlane(double xpos, double ypos)
{
	// 1280x720
	// convert screen pos to ndc range
	m_RayStart = TransformUtil::ConvertScreenToWorld(xpos, ypos, 1280.0f, 720.0f, m_CameraController, glm::vec3(0.0f));
	m_RayDirection = glm::normalize(glm::vec3(glm::vec3(m_RayStart) - m_CameraController.GetCamera().GetPosition()));
	return m_Plane->RayhitTest(m_RayStart, m_RayDirection, m_IntersectionPoint);
}

void ExampleLayer::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	m_Plane->Update();
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Controls");

	ImGui::Text("RayStart : %f, %f, %f", m_RayStart.x, m_RayStart.y, m_RayStart.z);
	ImGui::Text("RayDirection : %f, %f, %f", m_RayDirection.x, m_RayDirection.y, m_RayDirection.z);
	ImGui::Text("Intersection : %f, %f, %f", m_IntersectionPoint.x, m_IntersectionPoint.y, m_IntersectionPoint.z);
	ImGui::End();
}
