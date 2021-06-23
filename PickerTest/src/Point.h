#pragma once
#include <GLCore.h>
#include <GLCoreUtils.h>
#include "../BoundingBox.h"
#include "../Vertex.h"
#include "PointCloudUtils.h"

class Point {
public:
	Point(GLCore::Utils::Shader* shader, const GLCore::Utils::PerspectiveCameraController& cameraController);
	~Point();
	void Update();

	void OnMouseDown(double xPos, double yPos);
	void OnMouseUp(double xPos, double yPos);

	bool RayhitTest(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3& intersectionPoint);
	bool IsInBoundingBox(float worldPosX, float worldPosY);
	void OnPicked(bool selected);

	void UpdatePosition(glm::vec3 position) { m_WorldPos = position; }
	void FollowRay(glm::vec3 rayOrigin, glm::vec3 rayDirection);
private:
	GLuint m_QuadVA, m_QuadVB, m_QuadIB;
	const GLCore::Utils::PerspectiveCameraController& m_CameraController;
	GLCore::Utils::Shader* m_Shader;

	glm::vec4 m_SquareBaseColor = { 0.8f, 0.2f, 0.3f, 1.0f };
	glm::vec4 m_SquareAlternateColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec4 m_SquareColor = m_SquareBaseColor;

	glm::vec3 m_Vertices[4];
	BoundingBox m_BoundingBox;
	glm::vec3 m_WorldPos = glm::vec3(0.0f);
	glm::vec3 m_Normal;

	PointCloudPointType m_CurrentPointCloudPointType;
	void CreateBoundingBox();
};