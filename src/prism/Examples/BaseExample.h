#pragma once

#include "prism/Interfaces/ILayer.h"
#include "prism/Renderer/DynamicMesh.h"
#include "prism/Renderer/PerspectiveCamera.h"

namespace Prism::Examples
{
	class Base: public ILayer
	{
	public:
		Base(Core::SharedContextRef ctx, const std::string& name);
		virtual ~Base();
		
		void OnAttach() override;
		void OnDetach() override;
		void OnDraw() override;
		void OnSystemEvent(Event& e) override;
		void OnGuiDraw() override;
		void OnUpdate(float dt) override;
	private:
		static std::string s_CubeVertFilename;
		static std::string s_CubeFragFilename;
		static std::string s_CubeTextureFilename;
		static std::string s_PlaneTextureFilename;
		static std::string s_PlaneVertFilename;
		static std::string s_PlaneFragFilename;
		
		bool m_CursorOverGui{ false };
		Renderer::PerspectiveCamera m_Camera{ 45, 980, 750, 0.1f, 100.f };
		Renderer::DynamicMesh m_Cube{
			{ Gl::ShaderDataType::Float3, "position" },
			{ Gl::ShaderDataType::Float2, "texcord" },
			{ Gl::ShaderDataType::Float3, "normals" },
		};
		Renderer::DynamicMesh m_Plane{
			{ Gl::ShaderDataType::Float3, "position" },
			{ Gl::ShaderDataType::Float2, "texcord" },
		};
		
		glm::vec3 m_LastCubePos{ 0.f, 0.f, 0.f };
		glm::vec3 m_CubePosImGui{ 0.f, 0.f, 0.f };

		glm::mat4 m_PlaneTransform{ 1.f };
		glm::mat4 m_CubeTransform{ 1.f };
		glm::vec3 m_LightPosition{ 2.f, 0.f, -2.f };
		glm::vec3 m_LightClr{ 0.8f, 0.8f, 1.f };
		float m_LightIntensity { 1.f };
	};
}
