#include"ShaderLibrary.h"

std::unordered_map<ShaderLibrary::ShaderName, std::vector<std::string>> ShaderLibrary::m_ShaderPaths;
std::unordered_map<ShaderLibrary::ShaderName, GLCore::Utils::Shader*> ShaderLibrary::m_ShaderMap;

void ShaderLibrary::LoadShaders()
{
	m_ShaderPaths = {
		std::pair<ShaderName, std::vector<std::string>>
		{
			SHADER_TEST, {  "assets/shaders/test.vert.glsl",
							"assets/shaders/test.frag.glsl",
							""
						 }
		},
		std::pair<ShaderName, std::vector<std::string>>
		{
			SHADER_TRIANGLE_GEOM, {  "assets/shaders/test.vert.glsl",
									 "assets/shaders/test.frag.glsl",
								     "assets/shaders/triangle.geom.glsl"
							      }
		},
		std::pair<ShaderName, std::vector<std::string>>
		{
			SHADER_RECT_GEOM, {  "assets/shaders/test.vert.glsl",
									 "assets/shaders/test.frag.glsl",
									 "assets/shaders/rect.geom.glsl"
								  }
		}
	};

	for (auto data : m_ShaderPaths)
	{

		GLCore::Utils::Shader* shader = GLCore::Utils::Shader::FromGLSLTextFiles(
																					data.second[0],
																					data.second[1],
																					data.second[2]
																				);

		m_ShaderMap.insert(std::pair<ShaderName, GLCore::Utils::Shader*>(data.first, shader));
	}
}

const GLCore::Utils::Shader const * ShaderLibrary::GetShader(ShaderName name)
{
	if (m_ShaderMap.count(name) > 0)
	{
		return m_ShaderMap[name];
	}
	return nullptr;
}