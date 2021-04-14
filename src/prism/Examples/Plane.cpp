#include "Plane.h"

#include "glm/ext/matrix_transform.hpp"

namespace Prism::Examples
{
	Plane::Plane()
	{
		
	}
	
	Plane::Plane(Ref<Gl::Shader> shader)
		:
		m_Shader(shader)
	{
		m_Mesh.AddVertexData(std::vector<float>{
			-0.5f,   0.5f,  0.f,	0.f, 1.f,
			 0.5f,   0.5f,  0.f,	1.f, 1.f,
			-0.5f,  -0.5f,  0.f,    0.f, 0.f,
			 0.5,	-0.5,   0.f,    1.f, 0.f,
		});
		m_Mesh.ConnectVertices(0, 1, 2);
		m_Mesh.ConnectVertices(2, 1, 3);
		m_Mesh.Flush();
	}

	Plane::~Plane()
	{
		m_Mesh.ClearGpuBuffers();
	}

	void Plane::SetShader(Ref<Gl::Shader> shader)
	{
		m_Shader = shader;
	}
	
	void Plane::Render()
	{
		glm::mat4 m(1.f);
		m_Shader->Bind();
		m_Shader->SetMat4("transform", glm::scale(glm::translate(m, m_Position), m_Scale));
	}

	void Plane::Scale(float s)
	{
		m_Scale.x = s;
		m_Scale.z = s;
	}
}
