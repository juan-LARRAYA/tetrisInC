#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    // Inicializar SDL (sistema de video)
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error al inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Crear una ventana
    // SDL_CreateWindow(título, posX, posY, ancho, alto, flags)
    SDL_Window *window = SDL_CreateWindow(
        "Mi Primera Ventana en C!",
        SDL_WINDOWPOS_CENTERED,  // centrada horizontalmente
        SDL_WINDOWPOS_CENTERED,  // centrada verticalmente
        800,                      // 800 píxeles de ancho
        600,                      // 600 píxeles de alto
        SDL_WINDOW_SHOWN         // mostrarla inmediatamente
    );

    if (window == NULL) {
        printf("Error al crear ventana: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Mantener la ventana abierta por 3 segundos
    SDL_Delay(3000);  // 3000 milisegundos = 3 segundos

    // Limpiar y cerrar
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
