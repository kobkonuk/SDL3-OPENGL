#define  SDL_MAIN_USE_CALLBACKS
#define  STB_IMAGE_IMPLEMENTATION
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../vendor/imgui.h"
#include "../vendor/imgui_impl_sdl3.h"
#include "../vendor/imgui_impl_opengl3.h"
#include "../vendor/stb_image.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "renderer.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "shader.h"
#include "texture.h"

typedef enum {
	MENU,
	PLAYING,
	PAUSE,
	GAME_OVER,
	WIN,
} GameState;

static GameState game_state = MENU;
static SDL_Window *window = nullptr;

//ik this is ugly
static VertexBufferLayout* layout = nullptr;
static VertexBuffer* vb = nullptr;
static IndexBuffer* ib = nullptr;
static VertexArray* va = nullptr;
static Shader* shader = nullptr;
static Renderer* renderer = nullptr;
static Texture* texture = nullptr;

static SDL_GLContext gl_context;


SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {

	int width = 1920;
	int height = 1080;

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initialize : %s ", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	window = SDL_CreateWindow(
		"opengl",
		width,
		height,
		SDL_WINDOW_OPENGL
	);

	if (window == NULL)
		std::cout << "SDL_CreateWindow fail" << std::endl;

	gl_context = SDL_GL_CreateContext(window);

	if (!SDL_GL_SetSwapInterval(-1))
		SDL_GL_SetSwapInterval(1);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		std::cout << "GLEW_NOT_OK" << std::endl; 

	glViewport(0, 0, 1920, 1080);

	std::cout << glGetString(GL_VERSION) << std::endl;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//void(io);
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGuiStyle& = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);
	style.FontScaleDpi = main_scale;

	ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init("version 330 core");

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.5f, 0.5f, 0.5, 0.7f);


	float positions[] = {
	    540.0f,  540.0f, 0.0f, 0.0f,
		800.0f,  540.0f, 1.0f, 0.0f,
		800.0f,  800.0f, 1.0f, 1.0f,
	    540.0f,  800.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	vb = new VertexBuffer(positions, 4 * 4 * sizeof(float));
	ib = new IndexBuffer(indices, 6);
	va = new VertexArray();
	layout = new VertexBufferLayout();
	shader = new Shader("res/shaders/basic.shader");	
	texture = new Texture("res/textures/cirnoinside.jpg");
	renderer = new Renderer();

	layout->Push<float>(2);	
	layout->Push<float>(2);
	va->AddBuffer(*vb , *layout);

	glm::mat4 proj = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));

	glm::mat4 mvp = proj * view * model;

	shader->Bind();
	shader->SetUniformMat4f("u_MVP", mvp);

	texture->Bind();
	shader->SetUniform1i("u_Texture", 0);

	va->Unbind();
	vb->Unbind();
	ib->Unbind();
	shader->Unbind();

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
	
	if (game_state == MENU) {
		
		renderer->Clear();

		GLClearError();

		SDL_GL_SwapWindow(window);
	} 

	else if (game_state == PLAYING) {
	
		renderer->Clear();

		GLClearError();

		shader->Bind();
		renderer->Draw(va, ib, shader);

		GLCheckError();

		SDL_GL_SwapWindow(window);
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
	
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}

	if (game_state == MENU &&
		event->type == SDL_EVENT_KEY_DOWN &&
		event->key.key == SDLK_RETURN) {
		game_state = PLAYING;
		SDL_Log("Game state = playing");
	}

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
	//Writing a cool for loop is cleaner. But not safe enough
	//Im not risking any memory leaks. its not worth it. no im just scared. too scared. im a coward.

	delete vb;
	delete ib;
	delete va;
	delete layout;
	delete shader;
	delete renderer;
	delete texture;	

	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

	SDL_GL_DestroyContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
