#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_keycode.h>


#define SDLK_L                      0x0000006cu /**< 'l' */
#define SDLK_T                      0x00000074u /**< 't' */
#define SDLK_P                      0x00000070u /**< 'p' */

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

	SDL_Window *sdlWindowA = SDL_CreateWindow("A", 500, 500, SDL_WINDOW_OPENGL);

	SDL_GLContext glContext = SDL_GL_CreateContext(sdlWindowA);
	glewInit();
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_PROGRAM_POINT_SIZE);

	SDL_Event sdlEvent;

	const char *sourceA[] = {
		"#version 460 core \n"
		"void main() {"
		"	gl_PointSize = 3;"
		"	gl_Position = vec2(0.0, 0.5),"
		"				  vec2(0.5, -0.5),"
		"				  vec2(-0.5, -0.5)"
		"}"
	};

	const char *sourceB[] = {
		"#version 460 core \n"
		"vec2 line[3] = {"
		"		vec2(-0.5, 0.5),"
		"		vec2(0.5, -0.5),"
		"		vec2(-0.5, -0.5)"
		"};"
		"void main() {"
		"	gl_Position = vec4(line[gl_VertexID], 0.0, 1.0);"
		"}"
	};

	const char *sourceC[] = {
		"#version 460 core \n"
		"vec2 triangle[3] = {"
		"		vec2(0.0, 0.5),"
		"		vec2(0.5, -0.5),"
		"		vec2(-0.5, -0.5)"
		"};"
		"void main() {"
		"	gl_Position = vec4(triangle[gl_VertexID], 0.0, 1.0);"
		"}"
	};

	unsigned int vertexShaderIDA;
	unsigned int programIDA = createProgram(vertexShaderIDA);
	glDeleteShader(vertexShaderIDA);
	unsigned int vertexArrayID = createVertexArray();


	// Boucle de jeu
	bool isUp = true;
	while (isUp)
	{
		// Gestion des évènements
		while (SDL_PollEvent(&sdlEvent))
		{

			switch (sdlEvent.type)
			{
			case SDL_EVENT_KEY_DOWN:
			{
				SDL_Log("windowID : %i, Type : %s, Key : %d", sdlEvent.window.windowID, "SDL_EVENT_KEY_DOWN", sdlEvent.key.key);				
				switch (sdlEvent.key.key)
				{
				case SDLK_P:
					vertexShaderIDA = createVertexShader(sourceA);
								
					break;


				case SDLK_L:
				{
					vertexShaderIDA = createVertexShader(sourceB);
										
					break;
				};

				case SDLK_T:
				{
					vertexShaderIDA = createVertexShader(sourceB);
					
					break;
				}
					default:
					break;
				}
			}

			case SDL_EVENT_MOUSE_BUTTON_DOWN:
			{
				char buttons[6][20] = {"", "SDL_BUTTON_LEFT", "SDL_BUTTON_MIDDLE", "SDL_BUTTON_RIGHT", "SDL_BUTTON_X1", "SDL_BUTTON_X2"};
				// SDL_Log("windowID : %i,Type : %s, Button : %s, Clicks : %i", sdlEvent.window.windowID, "SDL_EVENT_MOUSE_BUTTON_DOWN");
				break;
			}
			case SDL_EVENT_MOUSE_MOTION:
				// SDL_Log("windowID : %i, Type : %s, x : %f, y : %f", sdlEvent.window.windowID, "SDL_EVENT_MOUSE_MOTION", sdlEvent.motion.x, sdlEvent.motion.y, sdlEvent.motion.xrel, sdlEvent.motion.yrel)	;
				break;


			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				if (sdlEvent.window.windowID == SDL_GetWindowID(sdlWindowA))
				{
					SDL_DestroyWindow(sdlWindowA);
					sdlWindowA = nullptr;
				}
				break;

			case SDL_EVENT_QUIT:
				isUp = false;
				break;

			default:
				break;
			}
			// Gestion de l'affichage.
			if (sdlWindowA)
			{
				SDL_GL_MakeCurrent(sdlWindowA, glContext);
				glClear(GL_COLOR_BUFFER_BIT);
				glUseProgram(programIDA);
				glBindVertexArray(vertexArrayID);
				// Dessiner
				glDrawArrays(GL_POINTS, 0, 1);
				SDL_GL_SwapWindow(sdlWindowA);
			}
		}

		/// TODO: Gestion des événements.
		/// TODO: GEstion des mises à jour.
		/// TODO: Gestion de l'affichage
	}

	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteProgram(programIDA);
	SDL_GL_DestroyContext(glContext);

	SDL_Quit();

	return 0;
}