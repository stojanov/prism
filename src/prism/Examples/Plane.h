#pragma once
#include "prism/GL/Buffer.h"
#include "prism/GL/Shader.h"
#include "prism/Renderer/DynamicMesh.h"

namespace Prism::Examples
{
	class Plane
	{
	public:
		Plane();
		Plane(Ref<Gl::Shader> shader);
		~Plane();
		void Render();
		void Scale(float s);

		void SetShader(Ref<Gl::Shader> shader);
	private:
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		Renderer::DynamicMesh m_Mesh{
			{ Gl::ShaderDataType::Float3, "position" },
			{ Gl::ShaderDataType::Float2, "texcord" },
		};
		Ref<Gl::Shader> m_Shader;
	};
}
