#include "MouseGesture.h"
#include "../TransformUtil.h"

MouseGesture::MouseGesture(const GLCore::Window& window, const GLCore::Utils::PerspectiveCameraController& cameraController)
	:m_Window(window), m_CameraController(cameraController)
{
	m_Shader = GLCore::Utils::Shader::FromGLSLTextFiles(
		"assets/shaders/ndc.vert.glsl",
		"assets/shaders/ndc.frag.glsl"
	);

	glCreateVertexArrays(1, &m_QuadVA);
	glBindVertexArray(m_QuadVA);

	glCreateBuffers(1, &m_QuadVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, m_Vertices, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };
	glCreateBuffers(1, &m_QuadIB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}
void MouseGesture::OnMouseDragStart(double xPos, double yPos)
{
	m_DragStartWorldPos = TransformUtil::ConvertToNDC(xPos, yPos, 0.0f, m_Window.GetWidth(), m_Window.GetHeight());
	m_MouseDragFinished = false;
	m_CanRender = false;
	std::cout << xPos << "," << yPos << std::endl;
}

void MouseGesture::OnMouseDragMove(double xPos, double yPos)
{
	m_CanRender = true;
	m_DragEndWorldPos = TransformUtil::ConvertToNDC(xPos, yPos, 0.0f, m_Window.GetWidth(), m_Window.GetHeight());
	CalculateGestureBox();
}

void MouseGesture::OnMouseDragEnd(double xPos, double yPos)
{
	m_DragEndWorldPos = TransformUtil::ConvertToNDC(xPos, yPos, 0.0f, m_Window.GetWidth(), m_Window.GetHeight());
	CalculateGestureBox();
	m_CanRender = true;
	m_MouseDragFinished = true;
}

void MouseGesture::CalculateGestureBox()
{
	m_GestureBox.minX = glm::min(m_DragStartWorldPos.x, m_DragEndWorldPos.x);
	m_GestureBox.maxX = glm::max(m_DragStartWorldPos.x, m_DragEndWorldPos.x);

	m_GestureBox.minY = glm::min(m_DragStartWorldPos.y, m_DragEndWorldPos.y);
	m_GestureBox.maxY = glm::max(m_DragStartWorldPos.y, m_DragEndWorldPos.y);

	GenerateVBOData();
}

void MouseGesture::GenerateVBOData()
{
	m_Vertices[0] = glm::vec3(m_GestureBox.minX, m_GestureBox.minY, 0);
	m_Vertices[1] = glm::vec3(m_GestureBox.maxX, m_GestureBox.minY, 0);
	m_Vertices[2] = glm::vec3(m_GestureBox.maxX, m_GestureBox.maxY, 0);
	m_Vertices[3] = glm::vec3(m_GestureBox.minX, m_GestureBox.maxY, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	void* vtbuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(vtbuffer, m_Vertices, sizeof(float) *12);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MouseGesture::OnImGuiRender()
{
	/*ImGui::Begin("Gesture");
	ImGui::Text("MouseStart : %f, %f %f", m_GestureBox.minX, m_GestureBox.minY, m_Vertices[0].z);
	ImGui::Text("MouseEnd : %f, %f %f", m_GestureBox.maxX, m_GestureBox.maxY, m_Vertices[0].z);
	glm::vec3 position = m_CameraController.GetCamera().GetPosition();
	ImGui::Text("Camera Position : %f %f %f", position.x, position.y, position.z);
	ImGui::End();*/
}

void MouseGesture::Render()
{
	if (!m_CanRender) return;

	glUseProgram(m_Shader->GetRendererID());

	int location = glGetUniformLocation(m_Shader->GetRendererID(), "u_Color");
	glUniform4fv(location, 1, glm::value_ptr(m_MouseDragFinished ? m_DragEndColor : m_DragColor));

	glBindVertexArray(m_QuadVA);
	glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, nullptr);
}


