#include <GL/glew.h> 
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_main.h>

/// @brief Instanciation d'un nuanceur de sommet.
/// @param source Code source du nuanceur.
/// @return Identificateur de nuanceur.
unsigned int createVertexShader(const char* source[]) {
	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER); 	
	glShaderSource(vertexShaderID, 1, source, NULL);
	glCompileShader(vertexShaderID);

	return vertexShaderID;
}


/// @brief Instanciation d'un programme.
/// @param vertexShaderID Identifiant du nuanceur de sommet.
/// @return Identifiant du programme.
unsigned int createProgram(unsigned int vertexShaderID) {
	unsigned int programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glLinkProgram(programID);

	return programID;
}

unsigned int createVertexArray() {
	unsigned int vertexArrayID;
	glCreateVertexArrays(1, &vertexArrayID);
	
	return vertexArrayID;
}


/// @brief Fonction principale.
/// @param args Nombre d'arguments.
/// @param argv Arguments.
/// @return Code de retour.
int main(int args, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_Window* sdlWindowA = SDL_CreateWindow("A", 500, 500, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_Window* sdlWindowB = SDL_CreateWindow("B", 500, 500, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_Window* sdlWindowC = SDL_CreateWindow("C", 500, 500, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	SDL_GLContext glContext = SDL_GL_CreateContext(sdlWindowA);
	glewInit();
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_PROGRAM_POINT_SIZE);

	SDL_Event sdlEvent;
	
	const char* sourceA[] = {
		"#version 460 core \n"
		"void main() {"
		"	gl_PointSize = 5;"
		"	gl_Position = vec4(0.0, 0.0, 0.0, 1.0);"
		"}"
	};

	unsigned int vertexShaderID = createVertexShader(sourceA);
	unsigned int programID = createProgram(vertexShaderID);
	glDeleteShader(vertexShaderID);
	unsigned int vertexArrayID = createVertexArray();


	bool isUp = true;
	while (isUp)
	{
		while (SDL_PollEvent(&sdlEvent)){

			switch(sdlEvent.type) {
				case SDL_EVENT_KEY_DOWN: 
				SDL_Log("windowID : %i, Type : %s, Key : %d", sdlEvent.window.windowID, "SDL_EVENT_KEY_DOWN", sdlEvent.key.key);
				break;
					
				case SDL_EVENT_MOUSE_BUTTON_DOWN: {
				char buttons[6][20] = {"", "SDL_BUTTON_LEFT", "SDL_BUTTON_MIDDLE", "SDL_BUTTON_RIGHT", "SDL_BUTTON_X1", "SDL_BUTTON_X2"};
				SDL_Log("windowID : %i,Type : %s, Button : %s, Clicks : %i", sdlEvent.window.windowID, "SDL_EVENT_MOUSE_BUTTON_DOWN");
				break;
				}
				case SDL_EVENT_MOUSE_MOTION: 
				SDL_Log("windowID : %i, Type : %s, x : %f, y : %f", sdlEvent.window.windowID, "SDL_EVENT_MOUSE_MOTION", sdlEvent.motion.x, sdlEvent.motion.y, sdlEvent.motion.xrel, sdlEvent.motion.yrel)	;
				break;

				case SDL_EVENT_WINDOW_RESIZED:
				SDL_Log("windowID : %i, Type : %s, Data1 : %i, Data2 : %i", sdlEvent.window.windowID, "SDL_EVENT_WINDOW_RESIZED", sdlEvent.window.data1, sdlEvent.window.data2)	;
				break;

				case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				if (sdlEvent.window.windowID == SDL_GetWindowID(sdlWindowA)) {
					SDL_DestroyWindow(sdlWindowA);
					sdlWindowA = nullptr;
				}
				else if (sdlEvent.window.windowID == SDL_GetWindowID(sdlWindowB)) {
					SDL_DestroyWindow(sdlWindowB);
					sdlWindowB = nullptr; 
				}
				else if (sdlEvent.window.windowID == SDL_GetWindowID(sdlWindowC)) {
					SDL_DestroyWindow(sdlWindowC);
					sdlWindowC = nullptr; 
				}
				break;
				
				case SDL_EVENT_QUIT:
				isUp = false;
				break;

				default:
				break;
			}
			//Gestion de l'affichage.
			if (sdlWindowA) {
				SDL_GL_MakeCurrent(sdlWindowA, glContext);
				glClear(GL_COLOR_BUFFER_BIT);
				glUseProgram(programID);
				glBindVertexArray(vertexArrayID);
				//Dessiner
				glDrawArrays(GL_POINTS, 0, 1);
				SDL_GL_SwapWindow(sdlWindowA);
			}
			if (sdlWindowB) {
				SDL_GL_MakeCurrent(sdlWindowB, glContext);
				glClear(GL_COLOR_BUFFER_BIT);
				glUseProgram(programID);
				glBindVertexArray(vertexArrayID);
				///TODO: Dessiner
				SDL_GL_SwapWindow(sdlWindowB);

			}
			if (sdlWindowC) {
				SDL_GL_MakeCurrent(sdlWindowC, glContext);
				glClear(GL_COLOR_BUFFER_BIT);
				glUseProgram(programID);
				glBindVertexArray(vertexArrayID);
				///TODO: Dessiner
				SDL_GL_SwapWindow(sdlWindowC);

			}
		
		}
		
		/// TODO: Gestion des événements.
		/// TODO: GEstion des mises à jour.
		/// TODO: Gestion de l'affichage
	}
	
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteProgram(programID);
	SDL_GL_DestroyContext(glContext);

	SDL_Quit();

	return 0;

}