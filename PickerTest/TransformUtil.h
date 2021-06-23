#pragma once
#include <GLCore.h>
#include <GLCoreUtils.h>

class TransformUtil{

public:
	static glm::vec3 ConvertScreenToWorld(float screenX, float screenY, float viewWidth, float viewHeight, const GLCore::Utils::PerspectiveCameraController & cameraController)
	{
		float ndcX = (2.0f * screenX / viewWidth) - 1.0f;
		float ndcY = 1.0f - (2.0f * screenY / viewHeight);

		glm::vec4 screenPos = glm::vec4(ndcX, ndcY, 1.0f, 1.0f);
		glm::mat4 invertProj = glm::inverse(cameraController.GetCamera().GetViewProjectionMatrix());
		glm::vec4 worldPos = invertProj * screenPos;
		worldPos /= worldPos.w;
		return glm::vec3(worldPos);
	}

	static glm::vec3 ConvertToNDC(float screenX, float screenY, float z, float viewWidth, float viewHeight)
	{
		glm::vec3 ndc;
		ndc.x = 2.0f * screenX / viewWidth - 1.0f;
		ndc.y = 1.0f - 2.0f * screenY / viewHeight;
		ndc.z = 2.0f * z - 1.0f;
		return ndc;
	}
};