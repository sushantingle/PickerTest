#include "Plane.h"

using namespace GLCore;
using namespace GLCore::Utils;

Plane::Plane(GLCore::Utils::Shader* shader, const GLCore::Utils::PerspectiveCameraController& cameraController)
	: m_Shader(shader), m_CameraController(cameraController)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glCreateVertexArrays(1, &m_QuadVA);
	glBindVertexArray(m_QuadVA);

	m_Vertices[0] = glm::vec3(-0.5f, -0.5f, 0.0f );
	m_Vertices[1] = glm::vec3( 0.5f, -0.5f, 0.0f );
	m_Vertices[2] = glm::vec3(0.5f, 0.5f, 0.0f );
	m_Vertices[3] = glm::vec3(-0.5f, 0.5f, 0.0f );
	

	glCreateBuffers(1, &m_QuadVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices), m_Vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };
	glCreateBuffers(1, &m_QuadIB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glm::vec3 sideA = glm::normalize(m_Vertices[0] - m_Vertices[1]);
	glm::vec3 sideB = glm::normalize(m_Vertices[2] - m_Vertices[1]);
	m_Normal = glm::normalize(glm::cross(sideA, sideB));


	CreateBoundingBox();
}

void Plane::CreateBoundingBox()
{
	m_BoundingBox.minX = m_Vertices[0].x;
	m_BoundingBox.maxX = m_Vertices[0].x;

	m_BoundingBox.minY = m_Vertices[0].y;
	m_BoundingBox.maxY = m_Vertices[0].y;

	m_BoundingBox.minZ = m_Vertices[0].z;
	m_BoundingBox.maxZ = m_Vertices[0].z;

	for (int i = 0; i < 4; ++i)
	{
		if (m_BoundingBox.minX > m_Vertices[i].x) m_BoundingBox.minX = m_Vertices[i].x;
		if (m_BoundingBox.maxX < m_Vertices[i].x) m_BoundingBox.maxX = m_Vertices[i].x;
		if (m_BoundingBox.minY > m_Vertices[i].y) m_BoundingBox.minY = m_Vertices[i].y;
		if (m_BoundingBox.maxY < m_Vertices[i].y) m_BoundingBox.maxY = m_Vertices[i].y;
		if (m_BoundingBox.minZ > m_Vertices[i].z) m_BoundingBox.minZ = m_Vertices[i].z;
		if (m_BoundingBox.maxZ < m_Vertices[i].z) m_BoundingBox.maxZ = m_Vertices[i].z;
	}
}

Plane::~Plane()
{
	glDeleteVertexArrays(1, &m_QuadVA);
	glDeleteBuffers(1, &m_QuadVB);
	glDeleteBuffers(1, &m_QuadIB);
}

void Plane::Update()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_Shader->GetRendererID());

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_WorldPos));
	glm::mat4 MVP = m_CameraController.GetCamera().GetViewProjectionMatrix() * model;
	int location = glGetUniformLocation(m_Shader->GetRendererID(), "u_ViewProjection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(MVP));

	location = glGetUniformLocation(m_Shader->GetRendererID(), "u_Color");
	glUniform4fv(location, 1, glm::value_ptr(m_SquareColor));

	glBindVertexArray(m_QuadVA);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Plane::OnMouseDown(double xPos, double yPos)
{

}

void Plane::OnMouseUp(double xPos, double yPos)
{

}

void Plane::OnPicked(bool selected)
{
	m_SquareColor = selected ? m_SquareAlternateColor : m_SquareBaseColor;
}

bool Plane::RayhitTest(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3& intersectionPoint)
{
	float d = glm::dot(m_Normal, m_WorldPos + m_Vertices[0]);

	if (glm::dot(m_Normal, rayDirection) == 0.0f)
		return false;

	float t = (d - glm::dot(m_Normal, rayOrigin)) / glm::dot(m_Normal, rayDirection);
	intersectionPoint = rayOrigin + t * rayDirection;

	return IsInBoundingBox(intersectionPoint.x, intersectionPoint.y);
}

bool Plane::IsInBoundingBox(float worldPosX, float worldPosY)
{
	if (worldPosX < m_BoundingBox.minX || worldPosX > m_BoundingBox.maxX) return false;
	if (worldPosY < m_BoundingBox.minY || worldPosY > m_BoundingBox.maxY) return false;

	return true;
}