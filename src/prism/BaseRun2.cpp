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
#include "Core/Events/WindowEvents.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "Renderer/DynamicMesh.h"
#include "Renderer/Texture.h"

using namespace Prism;

std::vector<float> texturedCube = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  0.0f,  -1.0f,
		0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,  0.0f,  0.0f,  -1.0f,
		0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  0.0f,  0.0f,  -1.0f,
		0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  0.0f,  0.0f,  -1.0f,
		-0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  0.0f,  0.0f,  -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  0.0f,  -1.0f,

		-0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, 0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

		-0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f,  0.0f,
		-0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,  -1.0f, 0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  -1.0f, 0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  -1.0f, 0.0f,  0.0f,
		-0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  -1.0f, 0.0f,  0.0f,
		-0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		0.5f,  -0.5f, 0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  -1.0f, 0.0f,
		0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,  0.0f,  -1.0f, 0.0f,
		0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,
		0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  0.0f,  -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  -1.0f, 0.0f,

		-0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,   0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
		-0.5f, 0.5f,  0.5f,  0.0f, 0.0f,   0.0f,  1.0f,  0.0f,
		-0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,   0.0f,  1.0f,  0.0f
};

void BaseRun2()
{
	Log::Init();
	Core::Window myWindow;

	myWindow.Init();
	myWindow.Create(1000, 1000, "Test Window");
	glm::vec4 clearColor{ 0.2f, 0.0f, 0.3f, 0.0f };

	auto StartTime = std::chrono::high_resolution_clock::now();
	auto LastFrameTime = std::chrono::high_resolution_clock::now();

	auto windowPtr = myWindow.GetNativeWindow();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(windowPtr, true);
	ImGui_ImplOpenGL3_Init("#version 400");

	Renderer::Texture CompanionCubeTexture("res/cube.jpg");
	auto shader = Gl::Shader::RefFromFiles("res/base.vert", "res/base.frag");
	auto Cube = Renderer::DynamicMesh({
		{ Gl::ShaderDataType::Float3, "position" },
		{ Gl::ShaderDataType::Float2, "texcord" },
		{ Gl::ShaderDataType::Float3, "normals" },
	});

	Cube.AddVertexData(texturedCube);
	Cube.FlushVertexData();
	CompanionCubeTexture.Bind(0);

	shader->SetInt("tex", 0);

	glm::vec3 lightPos( 1.f, 1.f, -2.f);
	glm::vec3 lightColor(1.f);
	
	float lightIntens = 1.f;

	glm::vec3 rotation(0.f);
	glm::vec3 lastRotVal(0.f);
	
	glm::mat4 cubeTransformMatrix(1.f);

	const auto rotateMatrix = [&cubeTransformMatrix](float angle, const glm::vec3& axis)
	{
		cubeTransformMatrix = glm::rotate(cubeTransformMatrix, glm::radians(angle), axis);
	};

	int displayWidth, displayHeight;

	myWindow.GetInputManager().SetEventCallback([](Event& e)
		{
			EventHandler Handler(e);

			Handler.Handle<KeyPressedEvent>([](auto e)
				{
					PR_INFO("Keyboard Key {0}",
						(char)(e.GetKey())
					);
				});

			Handler.Handle<WindowResizeEvent>([](auto e)
				{
					PR_INFO("Window size changed {0} {1}", e.GetWidth(), e.GetHeight());
				});
		});

	glfwGetWindowSize(windowPtr, &displayWidth, &displayHeight);
	
	glm::mat4 proj = glm::perspective(glm::radians(90.0f), (float)displayWidth / (float)displayHeight, 0.1f, 100.0f);

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
	
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(windowPtr))
	{
		StartTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration_cast<std::chrono::milliseconds>(LastFrameTime - StartTime).count();
		glfwGetWindowSize(windowPtr, &displayWidth, &displayHeight);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		ImGui::Begin("Cube");
		ImGui::SliderFloat3("Rotation", (float*) &rotation, -90, 90);
		ImGui::End();

		ImGui::Begin("Light");
		ImGui::SliderFloat("X", (float*)(&lightPos[0]), -5, 5);
		ImGui::SliderFloat("Y", (float*)(&lightPos[1]), -5, 5);
		ImGui::SliderFloat("Z", (float*)(&lightPos[2]), -5, 5);
		ImGui::SliderFloat("Intensity", &lightIntens, 0, 5);
		ImGui::ColorEdit3("Color", (float*)&lightColor);
		ImGui::End();
		
		rotateMatrix(rotation.x - lastRotVal.x, { 1.f, 0.f, 0.f });
		rotateMatrix(rotation.y - lastRotVal.y, { 0.f, 1.f, 0.f });
		rotateMatrix(rotation.z - lastRotVal.z, { 0.f, 0.f, 1.f });

		lastRotVal = rotation;
		ImGui::Render();

		myWindow.GetInputManager().ProcessEvents();
		
		glViewport(0, 0, displayWidth, displayHeight);
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->Bind();
		shader->SetMat4("transform", cubeTransformMatrix);
		shader->SetFloat3("lightPos", lightPos);
		shader->SetFloat("lightIntens", lightIntens);
		shader->SetFloat3("lightClr", lightColor);
		shader->SetMat4("projection", proj);
		shader->SetMat4("view", view);
		
		Cube.DrawArrays();
		
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		glfwSwapBuffers(myWindow.GetNativeWindow());
		LastFrameTime = std::chrono::high_resolution_clock::now();
	}
}