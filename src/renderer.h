#pragma once

#include "vertexarray.h"
#include "indexbuffer.h"
#include "shader.h"

#include <GL/glew.h>

void GLClearError();
void GLCheckError();

class Renderer {
	private:

	public:
		void Clear() const;
		void Draw(const VertexArray* va, const IndexBuffer* ib, const Shader* shader) const;
};
