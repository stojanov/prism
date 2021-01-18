#pragma once

#include "prism/Core/Layer.h"
#include "prism/GL/Shader.h"
#include "prism/Renderer/DynamicMesh.h"
#include "prism/Renderer/PerspectiveCamera.h"
#include "prism/Renderer/Texture.h"

namespace Prism::Examples
{
	class Base: public Core::Layer
	{
	public:
		Base();
		virtual ~Base() = default;
		
		void OnAttach() override;
		void OnDetach() override;
		void OnDraw() override;
		void OnSystemEvent(Event& e) override;
		void OnGuiDraw() override;
		void OnUpdate(float dt) override;
	private:
		static std::string s_VertFilename;
		static std::string s_FragFilename;
		static std::string s_TextureFilename;

		Renderer::PerspectiveCamera m_Camera;
		Gl::Shader m_Shader;
		Renderer::DynamicMesh m_Cube{
			{ Gl::ShaderDataType::Float3, "position" },
			{ Gl::ShaderDataType::Float2, "texcord" },
			{ Gl::ShaderDataType::Float3, "normals" },
		};
		Renderer::Texture m_Texture;

		glm::mat4 m_CubeTransform;
		glm::vec3 m_LightPosition{ 1.f, 1.f, 1.f };
		glm::vec3 m_LightClr{};
		float m_LightIntensity { 1.f };
	};
}
