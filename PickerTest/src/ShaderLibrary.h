#pragma once
#include <GLCore.h>
#include <GLCoreUtils.h>

class ShaderLibrary {

public:
	enum ShaderName{
		SHADER_TEST,
		SHADER_TRIANGLE_GEOM,
		SHADER_RECT_GEOM
	};
	static void LoadShaders();
	static const GLCore::Utils::Shader const * GetShader(ShaderName name);

private:
	//Shader Path order: Vertex Shader, Fragment Shader, Geometry Shader
	static std::unordered_map<ShaderName, std::vector<std::string>> m_ShaderPaths;
	static std::unordered_map<ShaderName, GLCore::Utils::Shader*> m_ShaderMap;
};