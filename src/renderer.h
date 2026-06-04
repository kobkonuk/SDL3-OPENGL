#pragma once

#include "vertexarray.h"
#include "indexbuffer.h"
#include "shader.h"
#include "../vendor/imgui.h" 
#include "../vendor/imgui_internal.h"
#include "../vendor/imconfig.h"
#include "../vendor/imgui_impl_sdl3.h"
#include "../vendor/imgui_impl_opengl3.h"


#include <GL/glew.h>
#include <SDL3/SDL.h>

void GLClearError();
void GLCheckError();

class Renderer {
	public:
		void Clear() const;
		void Draw(const VertexArray *va, 
				const IndexBuffer *ib, 
				const Shader *shader) const;
		void ImGuiInit(SDL_Window *window, SDL_GLContext gl_context);
		void ImGuiDraw();
		void ImGuiEvent(SDL_Event *event);
		void ImGuiDELETEandKILL();
};
