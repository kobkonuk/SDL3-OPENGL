#define  SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <GL/glew.h>
#include <STB/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

static GameState game_state = PLAYING;
static SDL_Window *window = nullptr;

static VertexBufferLayout* layout = nullptr;
static VertexBuffer* vb = nullptr;
static IndexBuffer* ib = nullptr;
static VertexArray* va = nullptr;
static Shader* shader = nullptr;
static Renderer* renderer = nullptr;
static Texture* texture = nullptr;

static SDL_GLContext gl_context;

static int location;

static float r = 0.0f;
static float increment = 0.05f;

static float positions[] = {
   -0.5f, -0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, 1.0f, 0.0f,
	0.5f,  0.5f, 1.0f, 1.0f,
   -0.5f,  0.5f, 0.0f, 1.0f
};

static unsigned int indices[] = {
	0, 1, 2,
	2, 3, 0
};


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
		"red-lantern 3D",
		width,
		height,
		SDL_WINDOW_OPENGL
	);

	if (window == NULL)
		std::cout << "SDL_CreateWindow fail" << std::endl;

	gl_context = SDL_GL_CreateContext(window);

	if (SDL_GL_SetSwapInterval(-1) < 0)
		SDL_GL_SetSwapInterval(1);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		std::cout << "GLEW_NOT_OK" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	vb = new VertexBuffer(positions, 4 * 4 * sizeof(float));
	ib = new IndexBuffer(indices, 6);
	va = new VertexArray();
	layout = new VertexBufferLayout();
	shader = new Shader("res/shaders/basic.shader");	
	texture = new Texture("res/textures/cirnoinside.jpg");

	layout->Push<float>(2);	
	layout->Push<float>(2);
	va->AddBuffer(*vb , *layout);

	shader->Bind();
	shader->SetUniform4f("u_Color", 0.0f, 0.0f, 0.0f, 0.0f);
	shader->SetUniformMat4f("u_MVP", proj);

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
		std::cout << "hello cro" << std::endl;	
	} 

	else if (game_state == PLAYING) {
	
		renderer->Clear();

		GLClearError();

		shader->Bind();
		shader->SetUniform4f("u_Color", r, 0.0f, 0.0f, 0.0f);
		renderer->Draw(va, ib, shader);

		if (r > 1.0f)
			increment = -0.05f;
		else if (r < 0.0f)
			increment = 0.05f;

		r += increment;
		
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
	delete vb;
	delete ib;
	delete va;
	delete layout;
	delete shader;
	delete renderer;
	delete texture;

	SDL_Quit();
}
