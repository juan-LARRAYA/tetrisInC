#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

// ============ CONSTANTES DE TETRIS ============
#define GRID_WIDTH 10   // Columnas del tablero
#define GRID_HEIGHT 20  // Filas del tablero
#define CELL_SIZE 30    // Tamaño de cada celda en píxeles

// Dimensiones de la ventana
#define WINDOW_WIDTH (GRID_WIDTH * CELL_SIZE)
#define WINDOW_HEIGHT (GRID_HEIGHT * CELL_SIZE)

int main(int argc, char *argv[])
{
    // Inicializar SDL (sistema de video)
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error al inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }
    printf("sistema de video iniciado con exito: \n");

    // Crear una ventana
    SDL_Window *window = SDL_CreateWindow(
        "Tetris en C!",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
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
        -1,                        // usar el primer driver de renderizado disponible
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

    // ============ GRILLA DE TETRIS ============
    // Array 2D que representa el tablero de juego
    // 0 = celda vacía, 1 = celda ocupada
    int grid[GRID_HEIGHT][GRID_WIDTH];

    // Inicializar la grilla vacía (todo en 0)
    for (int row = 0; row < GRID_HEIGHT; row++)
    {
        for (int col = 0; col < GRID_WIDTH; col++)
        {
            grid[row][col] = 0;
        }
    }

    // Para demo: llenar algunas celdas al azar
    grid[19][0] = 1; // fila inferior, primera columna
    grid[19][1] = 1;
    grid[19][2] = 1;
    grid[18][0] = 1; // segunda fila desde abajo
    grid[18][1] = 1;

    // GAME LOOP - El corazón de cualquier juego
    // Este loop se repite constantemente hasta que el usuario cierre la ventana
    while (running)
    {
        // 1. PROCESAR EVENTOS (input del usuario)
        // SDL_PollEvent revisa si hay eventos pendientes (clicks, teclas, etc.)
        while (SDL_PollEvent(&event))
        {
            // Imprimir información del evento para debug
            // printf("Evento detectado - Tipo: %d\n", event.type);

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
        // Por ahora no hay lógica (solo mostramos la grilla)

        // 3. RENDER (dibujar en pantalla)
        // Limpiar la pantalla con un color de fondo
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // negro
        SDL_RenderClear(renderer);

        // Dibujar la grilla de Tetris
        // Recorrer cada celda del array 2D
        for (int row = 0; row < GRID_HEIGHT; row++)
        {
            for (int col = 0; col < GRID_WIDTH; col++)
            {
                // Calcular la posición en píxeles de esta celda
                SDL_Rect cell = {
                    col * CELL_SIZE,      // X: columna × tamaño
                    row * CELL_SIZE,      // Y: fila × tamaño
                    CELL_SIZE - 1,        // ancho (- 1 para dejar espacio entre celdas)
                    CELL_SIZE - 1         // alto
                };

                // Si la celda está ocupada (grid[row][col] == 1)
                if (grid[row][col] == 1)
                {
                    // Dibujar celda cyan (color de Tetris)
                    SDL_SetRenderDrawColor(renderer, 0, 240, 240, 255);
                    SDL_RenderFillRect(renderer, &cell);
                }
                else
                {
                    // Dibujar celda vacía (solo el borde)
                    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255); // gris muy oscuro
                    SDL_RenderDrawRect(renderer, &cell);
                }
            }
        }

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
