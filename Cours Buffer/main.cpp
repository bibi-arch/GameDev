#include <fstream>
#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3_image/SDL_image.h>

#define VBO_COUNT 4
#define VAO_COUNT 3
#define PROGRAM_COUNT 2
#define TEXTURE_COUNT 2

#define TRIANGLE_VBO 0
#define COLORS_VBO 1
#define SQUARE_VBO 2
#define TRIANGLE_TEXCOORD_VBO 3

#define TRIANGLE_VAO 0
#define SQUARE_VAO 1
#define TEXTURE_TRIANGLE_VAO 2

#define COLOR_PROGRAM 0
#define TEXTURE_PROGRAM 1

#define TRAIANGLE_TEXTURE 0
#define SQUARE_TEXTURE 1

using namespace std;

unsigned int createTexture(const char* fileName){
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	SDL_Surface* sdlSurface = IMG_Load(fileName);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sdlSurface->w, sdlSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sdlSurface->pixels); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_DestroySurface(sdlSurface);

	return textureID;
}

unsigned int createVAO (unsigned int programID, unsigned int verticesVBOID, unsigned int vertexComponentCount, const char* glVertexAttributeName, unsigned int colorVBOID, unsigned int colorComponentCount, const char* colorAttributeName) {

	unsigned int vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, verticesVBOID);
	
}

unsigned int createVBO (float* data, size_t size) {
	unsigned int VBOID;
	glGenBuffers(1, &VBOID);
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	return VBOID;
}

void createVBOs(unsigned int* idArray) {
	float triangleVertices[] = {
			0.0, 0.5,
			0.5, -0.5,
			-0.5, -0.5
		};

	idArray[TRIANGLE_VBO] = createVBO(triangleVertices, sizeof(triangleVertices));

	float colors[] = {
		1.0, 0.0, 0.0, 1.0,
		0.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0
	};

	idArray[COLORS_VBO] = createVBO(colors, sizeof(colors));

	float sqareVertices[] = {
		-0.5, 0.5,
		0.5, 0.5,
		0.5, -0.5,
		-0.5, -0.5
	};

	idArray[SQUARE_VBO] = createVBO(sqareVertices, sizeof(sqareVertices));	

	float triangleTexCoord[] = {
		0.5, 0.0,
		1.0, 1.0,
		0.0, 1.0
	};

	idArray[TRIANGLE_TEXCOORD_VBO] = createVBO(triangleTexCoord, sizeof(triangleTexCoord));
}

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
	
	int programLength;
	return programID;
}

unsigned int loadProgram(const char* fileName) {
	ifstream inputStream(fileName, ios::binary | ios::ate);
	if (inputStream.is_open()) {
		int programLength = inputStream.tellg();
		inputStream.seekg(0, inputStream.beg);
		char* buffer = new char[programLength];
		inputStream.read(buffer, programLength);
		inputStream.close();

		unsigned int programID = glCreateProgram();
		glProgramBinary(programID, 1, buffer, programLength);
		glGetProgramiv(programID, GL_LINK_STATUS, &programLength);
		delete buffer;
		
	}
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

	//unsigned int programID = loadProgram("shaders/colorShape.bin");
	//if (!programID) {

	unsigned int programs[PROGRAM_COUNT];
	programs[COLOR_PROGRAM = createProgram("shaders/colorShape.bin"), create]
	programs[TEXTURE_PROGRAM] = create

	

	glUseProgram(programID);
	glBindVertexArray(vertexArrayID);
	unsigned int vertexArrayID = createVertexArray();
	glBindVertexArray(vertexArrayID);

	

	unsigned int triangleVertexBufferID;
	glGenBuffers(1, &triangleVertexBufferID);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(glGetAttribLocation(programID, "vertexAttribute"));
	glVertexAttribPointer(glGetAttribLocation(programID, "vertexAttribute"), 2, GL_FLOAT, GL_FALSE, 0, NULL);


	

	unsigned int colorVertexBufferID;
	glGenBuffers(1, &colorVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, colorVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), colors, GL_STATIC_DRAW);

	glEnableVertexAttribArray(glGetAttribLocation(programID, "colorAttribute"));
	glVertexAttribPointer(glGetAttribLocation(programID, "colorAttribute"), 4, GL_FLOAT, GL_FALSE, 0, NULL);

	unsigned int programID = createProgram(createVertexShader("shaders/colorShape.vert"), createFragmentShader("shaders/colorShape.frag"));
	unsigned int VBOs[VBO_COUNT];
	createVBOs(VBOs);
	createVAO(programID, VBOs[TRIANGLE_VBO], 2, "vertexAttribute", VBOs[COLORS_VBO], 4, "colorAttribute");



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
	glDeleteBuffers(programID, VBOs);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &vertexArrayID);
	SDL_GL_DestroyContext(glContext);
	SDL_DestroyWindow(sdlWindow);

	SDL_Quit();

	return 0;
}