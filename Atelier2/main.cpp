#include <GL/glew.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_keycode.h>


/// @brief Instanciation d'un nuanceur de sommet.
/// @param source Code source du nuanceur.
/// @return Identificateur de nuanceur.
unsigned int createVertexShader(const char *source[])
{
	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, source, NULL);
	glCompileShader(vertexShaderID);

	int isValid;
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &isValid);
	if (!isValid)
	{
		int lenght;
		glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &lenght);
		char *message = new char[lenght];
		glGetShaderInfoLog(vertexShaderID, lenght, nullptr, message);
		SDL_Log("%s", message);
		delete[] message;
		glDeleteShader(vertexShaderID);

		return 0;
	}

	return vertexShaderID;
}

/// @brief Instanciation d'un programme.
/// @param vertexShaderID Identifiant du nuanceur de sommet.
/// @return Identifiant du programme.
unsigned int createProgram(unsigned int vertexShaderID)
{
	unsigned int programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glLinkProgram(programID);

	int isValid;
	glGetProgramiv(programID, GL_COMPILE_STATUS, &isValid);
	if (!isValid)
	{
		int lenght;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &lenght);
		char *message = new char[lenght];
		glGetProgramInfoLog(programID, lenght, nullptr, message);
		SDL_Log("%s", message);
		delete[] message;
		glDeleteProgram(programID);

		return 0;
	}

	return programID;
}

unsigned int createVertexArray()
{
	unsigned int vertexArrayID;
	glCreateVertexArrays(1, &vertexArrayID);

	return vertexArrayID;
}

/// @brief Fonction principale.
/// @param argc Nombre d'arguments.
/// @param argv Arguments.
/// @return Code de retour.
int main(int argc, char *argv[])
{

	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_Window *sdlWindow = SDL_CreateWindow("Atelier 2", 500, 500, SDL_WINDOW_OPENGL);

	SDL_GLContext glContext = SDL_GL_CreateContext(sdlWindow);
	glewInit();
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_PROGRAM_POINT_SIZE);

	const char *source[] = {
		"#version 460 core \n"
		"vec2 points[3] = {"
		"   vec2(0.0, 0.5),"
		"   vec2(0.5, -0.5),"
		"   vec2(-0.5, -0.5)"
		"};"
		"void main() {"
		"   gl_PointSize = 3.0;"
		"   gl_Position = vec4(points[gl_VertexID], 0.0, 1.0);"
		"}"
	};

	unsigned int vertexShaderID = createVertexShader(source);
	unsigned int programID = createProgram(vertexShaderID);
	glDeleteShader(vertexShaderID);

	unsigned int vertexArrayID = createVertexArray();

	GLenum mode = -1;

	// Boucle de jeu
	bool isUp = true;
	while (isUp)
	{
		SDL_Event sdlEvent;

		// Gestion des évènements
		while (SDL_PollEvent(&sdlEvent))
		{

			switch (sdlEvent.type)
			{
			case SDL_EVENT_KEY_DOWN:
			{
				switch (sdlEvent.key.scancode)
				{
				case SDL_SCANCODE_P:
					SDL_SetWindowTitle(sdlWindow, "Points");
					mode = GL_POINTS;
					break;
				case SDL_SCANCODE_L:
					SDL_SetWindowTitle(sdlWindow, "Lignes");
					mode = GL_LINE_LOOP;
					break;
				case SDL_SCANCODE_T:
					SDL_SetWindowTitle(sdlWindow, "Triangles");
					mode = GL_TRIANGLES;
					break;
				default:
					break;
				}
				break;
			}
			case SDL_EVENT_KEY_UP:
				SDL_SetWindowTitle(sdlWindow, "Atelier 2");
				mode = -1;
				break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
			{
				switch (sdlEvent.button.button)
				{
				case SDL_BUTTON_LEFT:
					glClearColor(1.0, 0.0, 0.0, 1.0);
					break;
				case SDL_BUTTON_RIGHT:
					glClearColor(0.0, 0.0, 1.0, 1.0);
					break;
				case SDL_BUTTON_MIDDLE:
					glClearColor(0.0, 1.0, 0.0, 1.0);
					break;
				default:
					break;
				}
			}
			break;
			case SDL_EVENT_MOUSE_BUTTON_UP:
				glClearColor(1.0, 1.0, 1.0, 1.0);
				break;
			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				if (sdlEvent.window.windowID == SDL_GetWindowID(sdlWindow))
				{
					SDL_DestroyWindow(sdlWindow);
					sdlWindow = nullptr;
				}
				break;

			case SDL_EVENT_QUIT:
				isUp = false;
				break;

			default:
				break;
			}
		}

		if (sdlWindow)
		{
			SDL_GL_MakeCurrent(sdlWindow, glContext);
			glClear(GL_COLOR_BUFFER_BIT);
				glUseProgram(programID);
				glBindVertexArray(vertexArrayID);
				glDrawArrays(mode, 0, 3);
			SDL_GL_SwapWindow(sdlWindow);
		}
	}

	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteProgram(programID);
	SDL_GL_DestroyContext(glContext);
	SDL_DestroyWindow(sdlWindow);

	SDL_Quit();

	return 0;
}