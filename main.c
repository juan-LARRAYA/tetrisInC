#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    // Inicializar SDL (sistema de video)
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error al inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Crear una ventana
    SDL_Window *window = SDL_CreateWindow(
        "Mi Primera Ventana en C!",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        printf("Error al crear ventana: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Crear un renderer para dibujar
    // El renderer es como un "pincel" para dibujar en la ventana
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1, // usar el primer driver de renderizado disponible
        SDL_RENDERER_ACCELERATED); // usar aceleración por hardware (GPU)

    if (renderer == NULL)
    {
        printf("Error al crear renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Variable para controlar el loop del juego
    bool running = true;

    // Estructura para capturar eventos (clicks, teclas, etc.)
    SDL_Event event;

    // Posición del cuadrado (x, y)
    int playerX = 400; // centro horizontal (800/2)
    int playerY = 300; // centro vertical (600/2)
    int playerSize = 50; // tamaño del cuadrado en píxeles
    int playerSpeed = 5; // píxeles por frame

    // GAME LOOP - El corazón de cualquier juego
    // Este loop se repite constantemente hasta que el usuario cierre la ventana
    while (running)
    {
        // 1. PROCESAR EVENTOS (input del usuario)
        // SDL_PollEvent revisa si hay eventos pendientes (clicks, teclas, etc.)
        while (SDL_PollEvent(&event))
        {
            // Si el usuario hace click en la X de la ventana
            if (event.type == SDL_QUIT)
            {
                running = false; // salir del loop
            }

            // Si el usuario presiona una tecla
            if (event.type == SDL_KEYDOWN)
            {
                // Si presiona ESC, también cerramos
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
            }
        }

        // 2. UPDATE (actualizar lógica del juego)
        // Leer el estado del teclado (qué teclas están presionadas AHORA)
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);

        // Mover el cuadrado con las flechas
        if (keystate[SDL_SCANCODE_UP])
        {
            playerY -= playerSpeed; // mover arriba
        }
        if (keystate[SDL_SCANCODE_DOWN])
        {
            playerY += playerSpeed; // mover abajo
        }
        if (keystate[SDL_SCANCODE_LEFT])
        {
            playerX -= playerSpeed; // mover izquierda
        }
        if (keystate[SDL_SCANCODE_RIGHT])
        {
            playerX += playerSpeed; // mover derecha
        }

        // Limitar el cuadrado a los bordes de la ventana
        if (playerX < 0)
            playerX = 0;
        if (playerY < 0)
            playerY = 0;
        if (playerX + playerSize > 800)
            playerX = 800 - playerSize;
        if (playerY + playerSize > 600)
            playerY = 600 - playerSize;

        // 3. RENDER (dibujar en pantalla)
        // Limpiar la pantalla con un color de fondo
        SDL_SetRenderDrawColor(renderer, 25, 25, 50, 255); // azul oscuro
        SDL_RenderClear(renderer);

        // Dibujar el cuadrado del jugador
        // SDL_Rect es una estructura que define un rectángulo (x, y, ancho, alto)
        SDL_Rect player = {
            playerX,      // posición X
            playerY,      // posición Y
            playerSize,   // ancho
            playerSize    // alto
        };

        // Cambiar el color del pincel a verde brillante
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // verde (R=0, G=255, B=0)

        // Dibujar el rectángulo relleno
        SDL_RenderFillRect(renderer, &player);

        // Mostrar lo que dibujamos (swap buffers)
        SDL_RenderPresent(renderer);

        // 4. CONTROL DE FPS
        // Esperar un poco para no consumir 100% del CPU
        SDL_Delay(16); // ~60 FPS (1000ms/60 ≈ 16ms por frame)
    }

    // Limpiar y cerrar
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
