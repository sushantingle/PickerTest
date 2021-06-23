#include "Point.h"

using namespace GLCore;
using namespace GLCore::Utils;

Point::Point(GLCore::Utils::Shader* shader, const GLCore::Utils::PerspectiveCameraController& cameraController)
	: m_Shader(shader), m_CameraController(cameraController)
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
	glUseProgram(m_Shader->GetRendererID());

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_WorldPos));
	glm::mat4 MVP = m_CameraController.GetCamera().GetViewProjectionMatrix() * model;
	int location = glGetUniformLocation(m_Shader->GetRendererID(), "u_ViewProjection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(MVP));

	location = glGetUniformLocation(m_Shader->GetRendererID(), "u_Color");
	glUniform4fv(location, 1, glm::value_ptr(m_SquareColor));

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
	m_SquareColor = selected ? m_SquareAlternateColor : m_SquareBaseColor;
}

bool Point::RayhitTest(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3& intersectionPoint)
{
	return false;
}

void Point::FollowRay(glm::vec3 rayOrigin, glm::vec3 rayDirection)
{
	
}

bool Point::IsInBoundingBox(float worldPosX, float worldPosY)
{
	return false;
}