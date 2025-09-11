/// @file main.cpp
/// @author Bibiane Gagné Caron
/// @date 2025-09-05
/// @brief Animation de trois fenêtres OpenGL avec SDL3

#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_main.h>
#include <math.h>    

/// Variables globales pour l'animation
float temps = 0.0f;
float increment = 0.01f;
float PI = 3.14159f;

/// @brief Fonction d'animation des fenêtres 
/// @param window Fenêtre à animer
/// @param context Contexte OpenGL associé
/// @param largeur Largeur de la fenêtre
/// @param hauteur Hauteur de la fenêtre
/// @param rouge Composante rouge
/// @param vert Composante verte
/// @param bleu Composante bleue
void animer(SDL_Window* window, SDL_GLContext context, int largeur, int hauteur, float rouge, float vert, float bleu) {
    SDL_GL_MakeCurrent(window, context); 
    glViewport(0, 0, largeur, hauteur);  
    
    float intensite = (sin(temps) + 1.0f) / 2.0f;
    glClearColor(rouge * intensite, vert * intensite, bleu * intensite, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(window); 
}

/// @brief Fonction principale
/// @param args Nombre d'arguments
/// @param argv Arguments
/// @return Code de retour
int main (int args, char* argv[]) {
   
    // Initialisation de SDL et création des fenêtres
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    const SDL_DisplayMode* displayMode = SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay());

    // Gestion d'erreur
    if (!displayMode) {
        return -1; 
    }

    // Centrage des fenêtres à l'écran
    int largeurEcran  = displayMode->w;
    int hauteurEcran = displayMode->h;

    // Création fenêtre verte
    int vertX = (largeurEcran - 350) / 2;
    int vertY = (hauteurEcran - 350)/ 2;
    SDL_Window* sdlWindowB = SDL_CreateWindow("Vert", 350, 350, SDL_WINDOW_OPENGL);
    SDL_SetWindowPosition(sdlWindowB, vertX, vertY);
   
    // Création fenêtre rouge
    int rougeX = vertX - 250;
    int rougeY = (hauteurEcran - 250) / 2;
    SDL_Window* sdlWindowA = SDL_CreateWindow("Rouge", 250, 250, SDL_WINDOW_OPENGL);
    SDL_SetWindowPosition(sdlWindowA, rougeX, rougeY);

    // Création fenêtre bleue
    int bleuX = vertX + 350;
    int bleuY = (hauteurEcran - 450) / 2;
    SDL_Window* sdlWindowC = SDL_CreateWindow("Bleue", 450, 450, SDL_WINDOW_OPENGL);
    SDL_SetWindowPosition(sdlWindowC, bleuX, bleuY);

    // Création des contextes OpenGL pour chaque fenêtre
    SDL_GLContext glContextA = SDL_GL_CreateContext(sdlWindowA);
    SDL_GLContext glContextB = SDL_GL_CreateContext(sdlWindowB);
    SDL_GLContext glContextC = SDL_GL_CreateContext(sdlWindowC);
    
    // Initialisation de GLEW
    SDL_GL_MakeCurrent(sdlWindowA, glContextA); 
    glewInit();
    
    // Boucle principale
    SDL_Event event;

    bool isUp = true;
    while (isUp) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            // Fermeture de la fenêtre
            if (event.type == SDL_EVENT_QUIT) {
                isUp = false;
            }
            //Si une fenêtre est fermée manuellement
            else if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                SDL_WindowID windowID = event.window.windowID;

                if (windowID == SDL_GetWindowID(sdlWindowA)) {
                    SDL_DestroyWindow(sdlWindowA);
                    sdlWindowA = NULL;
                }
                else if (windowID == SDL_GetWindowID(sdlWindowB)) {
                    SDL_DestroyWindow(sdlWindowB);
                    sdlWindowB = NULL;
                    }
                else if (windowID == SDL_GetWindowID(sdlWindowC)) {
                    SDL_DestroyWindow(sdlWindowC);
                    sdlWindowC = NULL;
                }
            }
        }
        
        //Arrêt de l'animation après 3 cycles complets
        int iterations = (int)(temps / (2.0f * PI));
        if (iterations >= 3) {
            isUp = false;
        }
        
        // Animation des fenêtres
        animer(sdlWindowA, glContextA, 250, 250, 1.0f, 0.0f, 0.0f);
        animer(sdlWindowB, glContextB, 350, 350, 0.0f, 1.0f, 0.0f);
        animer(sdlWindowC, glContextC, 450, 450, 0.0f, 0.0f, 1.0f);
        
        // Mise à jour du temps pour l'animation
        temps += increment;  
        
        // Petite pause pour limiter la vitesse de la boucle
        SDL_Delay(16);    
    }
   
    // Nettoyage et fermeture
    SDL_GL_DestroyContext(glContextA);  
    SDL_GL_DestroyContext(glContextB);
    SDL_GL_DestroyContext(glContextC);

    SDL_DestroyWindow(sdlWindowA);
    SDL_DestroyWindow(sdlWindowB);
    SDL_DestroyWindow(sdlWindowC);

    SDL_Quit();

    return 0;
}