#include "Point.h"
#include "ShaderLibrary.h"

using namespace GLCore;
using namespace GLCore::Utils;

Point::Point(const GLCore::Utils::PerspectiveCameraController& cameraController)
	: m_CameraController(cameraController)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glCreateVertexArrays(1, &m_QuadVA);
	glBindVertexArray(m_QuadVA);

	m_Vertices[0] = glm::vec3(-0.0f, -0.0f, 0.0f);

	glCreateBuffers(1, &m_QuadVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices), m_Vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	CreateBoundingBox();
}

void Point::CreateBoundingBox()
{
	
}

Point::~Point()
{
	glDeleteVertexArrays(1, &m_QuadVA);
	glDeleteBuffers(1, &m_QuadVB);
	glDeleteBuffers(1, &m_QuadIB);
}

void Point::Update()
{
	auto shaderId = GetShader()->GetRendererID();
	glUseProgram(shaderId);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_WorldPos));
	glm::mat4 MVP = m_CameraController.GetCamera().GetViewProjectionMatrix() * model;
	int location = glGetUniformLocation(shaderId, "u_ViewProjection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(MVP));

	location = glGetUniformLocation(shaderId, "u_Color");
	glUniform4fv(location, 1, glm::value_ptr(m_IsSelected ? m_SquareAlternateColor : m_SquareColor));

	glBindVertexArray(m_QuadVA);
	glDrawArrays(GL_POINTS, 0, 1);
}

void Point::OnMouseDown(double xPos, double yPos)
{

}

void Point::OnMouseUp(double xPos, double yPos)
{

}

void Point::OnPicked(bool selected)
{
	m_IsSelected = selected;
}

bool Point::RayhitTest(glm::vec3 normal, glm::vec3 planePoint, glm::vec3& intersectionPoint)
{
	glm::vec3 rayOrigin = glm::vec3(glm::translate(glm::mat4(1.0f), m_WorldPos) * glm::vec4(m_Vertices[0], 1.0f));
	glm::vec3 rayDirection = glm::normalize(m_CameraController.GetCamera().GetPosition() - rayOrigin);

	float d = glm::dot(normal, planePoint);

	if (glm::dot(normal, rayDirection) == 0.0f)
	{ 
		return false;
	}
	float t = (d - glm::dot(normal, rayOrigin)) / glm::dot(normal, rayDirection);
	intersectionPoint = rayOrigin + t * rayDirection;

	return true;
}

void Point::FollowRay(glm::vec3 rayOrigin, glm::vec3 rayDirection)
{
	
}

bool Point::IsInBoundingBox(float worldPosX, float worldPosY)
{
	return false;
}

const GLCore::Utils::Shader* Point::GetShader()
{
	switch (m_CurrentPointCloudPointType)
	{
	
	case PointCloudPointType::TRIANGLE:
		return ShaderLibrary::GetShader(ShaderLibrary::SHADER_TRIANGLE_GEOM);
	case PointCloudPointType::RECTANGLE:
		return ShaderLibrary::GetShader(ShaderLibrary::SHADER_RECT_GEOM);
	case PointCloudPointType::POINT:
	default:
		return ShaderLibrary::GetShader(ShaderLibrary::SHADER_TEST);
	}
	return nullptr;
}