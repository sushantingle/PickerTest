#pragma once
#include <GLCore.h>
#include <GLCoreUtils.h>

namespace TransformUtil {

	glm::vec3 ConvertScreenToWorld(float screenX, float screenY, float viewWidth, float viewHeight, const GLCore::Utils::PerspectiveCameraController& cameraController)
	{
		float ndcX = (2.0f * screenX / viewWidth) - 1.0f;
		float ndcY = 1.0f - (2.0f * screenY / viewHeight);

		glm::vec4 screenPos = glm::vec4(ndcX, ndcY, 1.0f, 1.0f);
		glm::mat4 invertProj = glm::inverse(cameraController.GetCamera().GetViewProjectionMatrix());
		glm::vec4 worldPos = invertProj * screenPos;
		worldPos /= worldPos.w;
		return glm::vec3(worldPos);
	}
}