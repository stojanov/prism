#include "Shader.h"

#include <gtc/type_ptr.hpp>
#include "prism/System/FileIO.h"

#include <iostream>

namespace Prism::Gl
{
	Shader Shader::FromFiles(const std::string& vertFile, const std::string& fragFile)
	{
		std::string vert = System::ReadFile(vertFile);
		std::string frag = System::ReadFile(fragFile);
		
		return Shader(vert, frag);
	}

	Ref<Shader> Shader::RefFromFiles(const std::string& vertFile, const std::string& fragFile)
	{
		std::string vert = System::ReadFile(vertFile);
		std::string frag = System::ReadFile(fragFile);
		
		return MakeRef<Shader>(vert, frag);
	}

	Ptr<Shader> Shader::PtrFromFiles(const std::string& vertFile, const std::string& fragFile)
	{
		std::string vert = System::ReadFile(vertFile);
		std::string frag = System::ReadFile(fragFile);
		
		return MakePtr<Shader>(vert, frag);
	}

	Shader::Shader()
	{
		PR_CRITICAL("Creating a shader base");
	}
	
	Shader::Shader(const std::string& vert, const std::string& frag)
	{
		Compile(vert, frag);
	}

	Shader::~Shader()
	{
		Delete();
	}

	void Shader::Compile(const std::string& vert, const std::string& frag)
	{
		m_VertexShader = __CompileShader(GL_VERTEX_SHADER, vert.c_str());
		m_FragmentShader = __CompileShader(GL_FRAGMENT_SHADER, frag.c_str());

		m_Program = glCreateProgram();
		glAttachShader(m_Program, m_VertexShader);
		glAttachShader(m_Program, m_FragmentShader);

		glLinkProgram(m_Program);

		GLint isLinked = 0;
		glGetProgramiv(m_Program, GL_LINK_STATUS, &isLinked);
		
		if (!isLinked)
		{
			GLint maxLen = 0;

			glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &maxLen);
			std::vector<GLchar> infoLog(maxLen);
			glGetProgramInfoLog(m_Program, maxLen, &maxLen, &infoLog[0]);
			
			glDeleteProgram(m_Program);
			glDeleteShader(m_VertexShader);
			glDeleteShader(m_FragmentShader);

			PR_CORE_ERROR("{0}", infoLog.data());
			PR_ASSERT(false, "Error linking program");
		}
		
		glDetachShader(m_Program, m_VertexShader);
		glDetachShader(m_Program, m_FragmentShader);
		glDeleteShader(m_VertexShader);
		glDeleteShader(m_FragmentShader);
	}

	void Shader::SetIntV(const char* name, int count, int* v) const
	{
		GLint loc = glGetUniformLocation(m_Program, name);
		glUniform1iv(loc, count, v);
	}

	void Shader::SetFloatV(const char* name, int count, float* v) const
	{
		GLint loc = glGetUniformLocation(m_Program, name);
		glUniform1fv(loc, count, v);
	}

	void Shader::SetInt(const char* name, int v) const
	{
		GLint loc = glGetUniformLocation(m_Program, name);
		glUniform1i(loc, v);
	}
	
	void Shader::SetFloat(const char* name, float v) const
	{
		GLint loc = glGetUniformLocation(m_Program, name);
		glUniform1f(loc, v);
	}

	void Shader::SetFloat2(const char* name, const glm::vec2& v) const
	{
		GLint loc = glGetUniformLocation(m_Program, name);
		glUniform2f(loc, v.x, v.y);
	}
	
	void Shader::SetFloat3(const char* name, const glm::vec3& v) const
	{
		GLint loc = glGetUniformLocation(m_Program, name);
		glUniform3f(loc, v.x, v.y, v.z);
	}

	void Shader::SetFloat4(const char* name, const glm::vec4& v) const
	{
		GLint loc = glGetUniformLocation(m_Program, name);
		glUniform4f(loc, v.x, v.y, v.z, v.w);
	}

	void Shader::SetMat3(const char* name, const glm::mat3& v) const
	{
		GLint loc = glGetUniformLocation(m_Program, name);
		glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(v));
	}

	
	void Shader::SetMat4(const char* name, const glm::mat4& v) const
	{
		GLint loc = glGetUniformLocation(m_Program, name);
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(v));
	}

	void Shader::Bind()
	{
		glUseProgram(m_Program);
	}

	void Shader::Unbind()
	{
		glUseProgram(0);
	}
	
	void Shader::Delete()
	{
		glDeleteProgram(m_Program);
	}
}
