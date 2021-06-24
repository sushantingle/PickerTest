#pragma once
#include <GLCore.h>
#include <GLCoreUtils.h>
#include "../BoundingBox.h"

class MouseGesture {
public:
	MouseGesture(const GLCore::Window& window, const GLCore::Utils::PerspectiveCameraController& cameraController);
	void OnMouseDragStart(double xPos, double yPos);
	void OnMouseDragMove(double xPos, double yPos);
	void OnMouseDragEnd(double xPos, double yPos);

	void OnImGuiRender();
	void Render();

	const BoundingBox& GetGestureBox() const { return m_GestureBox; }
	const glm::vec3& GetDragStartPosition() const { return m_DragStartWorldPos; }
	const glm::vec3& GetDragEndPosition() const { return m_DragEndWorldPos; }
private:
	
	BoundingBox m_GestureBox;
	GLuint m_QuadVA, m_QuadVB, m_QuadIB;
	glm::vec3 m_Vertices[4];
	
	glm::vec3 m_DragStartWorldPos;
	glm::vec3 m_DragEndWorldPos;
	const GLCore::Utils::PerspectiveCameraController& m_CameraController;
	GLCore::Utils::Shader* m_Shader;
	const GLCore::Window& m_Window;

	glm::vec4 m_DragColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 m_DragEndColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	bool m_MouseDragFinished = false;
	bool m_CanRender = false;
	void CalculateGestureBox();
	void GenerateVBOData();

};
