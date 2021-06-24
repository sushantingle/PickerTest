#pragma once
#include <GLCore.h>
#include <GLCoreUtils.h>
#include "../BoundingBox.h"
#include "../Vertex.h"
#include "PointCloudUtils.h"

class Point {
public:
	Point(const GLCore::Utils::PerspectiveCameraController& cameraController);
	~Point();
	void Update();

	void OnMouseDown(double xPos, double yPos);
	void OnMouseUp(double xPos, double yPos);

	bool RayhitTest(glm::vec3 normal, glm::vec3 planePoint, glm::vec3& intersectionPoint);
	bool IsInBoundingBox(float worldPosX, float worldPosY);
	void OnPicked(bool selected);
	void SetPointCloudType(PointCloudPointType type) { m_CurrentPointCloudPointType = type; }

	void UpdatePosition(glm::vec3 position) { m_WorldPos = position; }
	void FollowRay(glm::vec3 rayOrigin, glm::vec3 rayDirection);
private:
	GLuint m_QuadVA, m_QuadVB, m_QuadIB;
	const GLCore::Utils::PerspectiveCameraController& m_CameraController;

	glm::vec4 m_SquareBaseColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	glm::vec4 m_SquareAlternateColor = { 0.0f, 1.0f, 0.0f, 1.0f };
	glm::vec4 m_SquareColor = m_SquareBaseColor;

	bool m_IsSelected = false;
	glm::vec3 m_Vertices[4];
	BoundingBox m_BoundingBox;
	glm::vec3 m_WorldPos = glm::vec3(0.0f);

	PointCloudPointType m_CurrentPointCloudPointType = PointCloudPointType::POINT;
	void CreateBoundingBox();
	const GLCore::Utils::Shader* GetShader();
};