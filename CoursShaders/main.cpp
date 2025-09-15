#include <fstream>
#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_keycode.h>

using namespace std;

unsigned int createVertexShader(const char* fileName) {
	
	ifstream file(fileName, ios::ate);
	if (file.is_open()) {
		size_t fileSize = file.tellg();
		char* vertexShaderSource = new char[fileSize + 1];
		vertexShaderSource[fileSize] = NULL;

		file.seekg(0, ios::beg);
		file.read(vertexShaderSource, fileSize);
		file.close();

		unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);
		delete[] vertexShaderSource;
		glCompileShader(vertexShaderID);

		return vertexShaderID;
	}
}

unsigned int createFragmentShader(const char* fileName) {

	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSource = (const char*) SDL_LoadFile(fileName, nullptr);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShaderID);

	return fragmentShaderID;
}

unsigned int createProgram(unsigned int vertexShaderID, unsigned int fragmentShaderID) {
	unsigned int programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	
	return programID;
}

unsigned int createVertexArray() {
	unsigned int vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);

	return vertexArrayID;
}

/// @brief Fonction principale.
/// @param argc Nombre d'arguments.
/// @param argv Arguments 
/// @return 
int main(int argc, char *argv[]){

	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_Window *sdlWindow = SDL_CreateWindow("Nuanceurs", 800, 800, SDL_WINDOW_OPENGL);
	SDL_GLContext glContext = SDL_GL_CreateContext(sdlWindow);
	glewInit();

	unsigned int programID = createProgram(createVertexShader("shaders/colorShape.vert"), createFragmentShader("shaders/colorShape.frag"));
	glUseProgram(programID);
	unsigned int vertexArrayID = createVertexArray();
	glBindVertexArray(vertexArrayID);

	float vertices[] = {
		0.0, 0.5,
		0.5, -0.5,
		-0.5, -0.5
	};

	glEnableVertexAttribArray(glGetAttribLocation(programID, "vertexAttribute"));
	glVertexAttribPointer(glGetAttribLocation(programID, "vertexAttribute"), 2, GL_FLOAT, GL_FALSE, 0, vertices);


	float colors[] = {
		1.0, 0.0, 0.0, 1.0,
		0.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 1.0
	};

	glEnableVertexAttribArray(glGetAttribLocation(programID, "colorAttribute"));
	glVertexAttribPointer(glGetAttribLocation(programID, "colorAttribute"), 4, GL_FLOAT, GL_FALSE, 0, colors);


	bool isUp = true;
	SDL_Event eventData;

	while (isUp) {
		while(SDL_PollEvent(&eventData)) 
			switch (eventData.type) {
				case SDL_EVENT_QUIT:
				isUp = false;
				break;
			}


			glClear(GL_COLOR_BUFFER_BIT);
			glBindVertexArray(vertexArrayID);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			/// TODO: Draw
			SDL_GL_SwapWindow(sdlWindow);
		
	}

	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &vertexArrayID);
	SDL_GL_DestroyContext(glContext);
	SDL_DestroyWindow(sdlWindow);

	SDL_Quit();

	return 0;
}