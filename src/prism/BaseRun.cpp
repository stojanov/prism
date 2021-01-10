#include <chrono>
#include <iostream>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "BaseRun.h"

#include "Core/Events/KeyEvents.h"
#include "Core/Events/MouseEvents.h"
#include "prism/Core/Window.h"
#include "prism/Core/SystemEventManager.h"
#include "GL/Shader.h"
#include "GL/VertexArray.h"
#include "System/Log.h"
#include "stb_image.h"
#include "Renderer/DynamicMesh.h"

using namespace Prism;

Ref<Gl::Shader> shad = nullptr;
Gl::VertexArray* Triangle = nullptr;
Ref<Gl::VertexBuffer> TriangleVertBuffer;

unsigned VAO;

float vertices[] = {
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
};

bool IMGUI_RENDER = false;

void Init()
{
	shad = Gl::Shader::RefFromFiles("res/base.vert", "res/base.frag");
	
	TriangleVertBuffer = Gl::VertexBuffer::CreateRef(vertices, sizeof(vertices), {
		{
			{ Gl::ShaderDataType::Float3, "position" },
			{ Gl::ShaderDataType::Float3, "color" },
		}
	});
	
	Triangle = new Gl::VertexArray();
	Triangle->AddVertexBuffer(TriangleVertBuffer);
}

void IMGUI()
{
	
	ImGui::SliderFloat3("Vertex 1 Position", (float*)&vertices[0], -1, 1);
	ImGui::SliderFloat3("Vertex 2 Position", (float*)&vertices[6], -1, 1);
	ImGui::SliderFloat3("Vertex 3 Position", (float*)&vertices[12], -1, 1);
	ImGui::ColorEdit3("Vertex 1", (float*)&vertices[3]);
	ImGui::ColorEdit3("Vertex 2", (float*)&vertices[9]);
	ImGui::ColorEdit3("Vertex 3", (float*)&vertices[15]);
	
}

void DrawLoop()
{
	shad->Bind();
	TriangleVertBuffer->SetData(vertices, sizeof(vertices));
	Triangle->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Entry()
{
	Log::Init();
	Core::Window myWindow;

	myWindow.Init();
	myWindow.Create(1000, 1000, "Test Window");
	glm::vec4 clearColor{ 0.0f, 0.8f, 0.0f, 0.0f };

	auto StartTime = std::chrono::high_resolution_clock::now();
	auto LastFrameTime = std::chrono::high_resolution_clock::now();

	auto windowPtr = myWindow.GetNativeWindow();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(windowPtr, true);
	ImGui_ImplOpenGL3_Init("#version 400");

	auto triangleShader = Gl::Shader::RefFromFiles("res/base.vert", "res/base.frag");

	Renderer::DynamicMesh Triangle;

	Triangle.AddVertex(glm::vec3{ -0.5f, -0.5f, 0.0f, });
	Triangle.AddVertex(glm::vec3{ 0.5f, -0.5f, 0.0f, });
	Triangle.AddVertex(glm::vec3{ 0.0f, 0.5f, 0.0f, });
	
	auto colorBuffer = Triangle.CreateNewVertexBuffer({
		{
			Gl::ShaderDataType::Float3, "color"
		} });

	Triangle.AddVertex(colorBuffer, glm::vec3{ 1.0f, 0.0f, 0.0f });
	Triangle.AddVertex(colorBuffer, glm::vec3{ 0.0f, 1.0f, 0.0f });
	Triangle.AddVertex(colorBuffer, glm::vec3{ 1.0f, 0.0f, 1.0f });
	
	Triangle.FlushVertexData();
	Triangle.FlushVertexData(1);
	
	int displayWidth, displayHeight;

	
	while (!glfwWindowShouldClose(windowPtr))
	{
		StartTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration_cast<std::chrono::milliseconds>(LastFrameTime - StartTime).count();
		glfwGetWindowSize(windowPtr, &displayWidth, &displayHeight);
		
		if (IMGUI_RENDER)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			IMGUI();
			ImGui::Render();
		}

		myWindow.GetInputManager().ProcessEvents();
		
		glViewport(0, 0, displayWidth, displayHeight);
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT);

		triangleShader->Bind();
		Triangle.DrawArrays();
		
		if (IMGUI_RENDER)
		{
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		
		glfwSwapBuffers(myWindow.GetNativeWindow());
		LastFrameTime = std::chrono::high_resolution_clock::now();
	}
}