#include "renderer.h"
#include <iostream>

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

void GLCheckError() {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL fail] ( " << error << " )" << std::endl;
	}
}
