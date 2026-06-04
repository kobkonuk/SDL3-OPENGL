#include "renderer.h"
#include <iostream>

static ImGuiIO *io = nullptr;

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

void GLCheckError() {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL fail] ( " << error << " )" << std::endl;
	}
}

void Renderer::Draw(
		const VertexArray* va, 
		const IndexBuffer* ib, 
		const Shader* shader) const {
	shader->Bind();
	va->Bind();
	ib->Bind();
	
	glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear() const {
	glClear(GL_COLOR_BUFFER_BIT); // another one liner abstracted.... heh...
}

void Renderer::ImGuiInit(SDL_Window *window, SDL_GLContext gl_context) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	static float main_scale = 1.0f;
	
	io = &ImGui::GetIO();
	
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);
	style.FontScaleDpi = main_scale;

	ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init("#version 330 core");
}

void Renderer::ImGuiDraw() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	static bool show_demo_window = true;
	static bool show_another_window = false;
	static ImVec4 clear_color = ImVec4(0.5f, 0.5f, 0.5, 0.7f);

	if(show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

	// Docs said to just copy this as an example
	{
		static float f = 0.0f;
		static int counter = 0;
		

		ImGui::Begin("Hello, world!");
		ImGui::Text("This is some useful text.");
		ImGui::Checkbox("Demo Window", &show_demo_window);
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::ColorEdit3("clear color", (float*)&clear_color); 

		if (ImGui::Button("Button")) counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 
				1000.0f / io->Framerate, 
				io->Framerate);
		ImGui::End();
	}

	if (show_another_window) {
		ImGui::Begin("Another Window", &show_another_window);
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me")) show_another_window = false;
		ImGui::End();
	}

	ImGui::Render();
	glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);
	glClearColor(
			clear_color.x * clear_color.w, 
			clear_color.y * clear_color.w, 
			clear_color.z * clear_color.w,
			clear_color.w);

	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::ImGuiEvent(SDL_Event *event) {
	ImGui_ImplSDL3_ProcessEvent(event); //yes im abstracting a single line, suck it losers
}

void Renderer::ImGuiDELETEandKILL() {
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}
