#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>

#include <glm.hpp>

#include "prism/Core/Pointers.h"
#include "prism/System/System.h"

namespace Prism::Gl
{
	class Shader
	{
	public:
		Shader();
		Shader(const std::string& vert, const std::string& frag);

		static Shader FromFiles(const std::string& vertFile, const std::string& fragFile);
		static Ref<Shader> RefFromFiles(const std::string& vertFile, const std::string& fragFile);
		static Ptr<Shader> PtrFromFiles(const std::string& vertFile, const std::string& fragFile);
		
		virtual ~Shader();
		
		void SetInt(const std::string& name, int v) const;
		void SetFloat(const std::string& name, float v) const;
		void SetFloat2(const std::string& name, const glm::vec2& v) const;
		void SetFloat3(const std::string& name, const glm::vec3& v) const;
		void SetFloat4(const std::string& name, const glm::vec4& v) const;
		void SetMat3(const std::string& name, const glm::mat3& m) const;
		void SetMat4(const std::string& name, const glm::mat4& m) const;
		
		void Bind();
		void Unbind();
		
		void Delete();
	private:
		GLuint m_Program;
		GLuint m_VertexShader;
		GLuint m_FragmentShader;
		
		void Compile(const std::string& vert, const std::string& frag);
		
		static GLuint __CompileShader(GLenum type, const char* src)
		{
			GLuint shader = glCreateShader(type);
			glShaderSource(shader, 1, &src, nullptr);
			glCompileShader(shader);

			GLint isCompiled = 0;

			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

			if (!isCompiled)
			{
				GLint maxLength;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				PR_CORE_ERROR("{0}: {1}", type == GL_VERTEX_SHADER ? "Vertex Shader" : "Fragment Shader", infoLog.data());
				PR_ASSERT(false, "Shader Compilation Error");
			}

			return shader;
		}
;	};
}
