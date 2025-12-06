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

    // Para demo: llenar algunas celdas al azar (piezas ya caídas)
    grid[19][0] = 1; // fila inferior, primera columna
    grid[19][1] = 1;
    grid[19][2] = 1;
    grid[18][0] = 1; // segunda fila desde abajo
    grid[18][1] = 1;

    // ============ PIEZA ACTUAL (la que está cayendo) ============
    // Posición de la pieza en la grilla (coordenadas lógicas)
    int pieceX = 4; // columna (centro de la grilla)
    int pieceY = 0; // fila (arriba del todo)

    // Forma de la pieza: cuadrado 2×2 (pieza O de Tetris)
    // Representamos la pieza como un array 2D de 4×4 (máximo tamaño de pieza)
    int piece[4][4] = {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    };

    // Timer para la caída automática
    Uint32 lastFallTime = SDL_GetTicks(); // milisegundos actuales
    Uint32 fallDelay = 500;                // caer cada 500ms (medio segundo)

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

        // Caída automática de la pieza
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastFallTime >= fallDelay)
        {
            // Ha pasado suficiente tiempo, hacer caer la pieza
            pieceY++; // mover la pieza una fila hacia abajo

            // Reiniciar el timer
            lastFallTime = currentTime;

            // TODO: detectar colisión con el fondo o con otras piezas
            // Por ahora, si llega al fondo, reiniciar arriba
            if (pieceY > GRID_HEIGHT - 3)
            {
                pieceY = 0; // volver arriba
            }
        }

        // Control manual con flechas
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_LEFT])
        {
            pieceX--; // mover izquierda
            SDL_Delay(100); // pequeño delay para no mover muy rápido
        }
        if (keystate[SDL_SCANCODE_RIGHT])
        {
            pieceX++; // mover derecha
            SDL_Delay(100);
        }
        if (keystate[SDL_SCANCODE_DOWN])
        {
            pieceY++; // caída rápida
            SDL_Delay(100);
        }

        // Limitar la pieza a los bordes de la grilla
        if (pieceX < 0)
            pieceX = 0;
        if (pieceX > GRID_WIDTH - 3)
            pieceX = GRID_WIDTH - 3;
        if (pieceY < 0)
            pieceY = 0;

        // 3. RENDER (dibujar en pantalla)
        // Limpiar la pantalla con un color de fondo
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // negro
        SDL_RenderClear(renderer);

        // Dibujar la grilla de Tetris (piezas ya colocadas)
        for (int row = 0; row < GRID_HEIGHT; row++)
        {
            for (int col = 0; col < GRID_WIDTH; col++)
            {
                SDL_Rect cell = {
                    col * CELL_SIZE,
                    row * CELL_SIZE,
                    CELL_SIZE - 1,
                    CELL_SIZE - 1};

                if (grid[row][col] == 1)
                {
                    SDL_SetRenderDrawColor(renderer, 0, 240, 240, 255);
                    SDL_RenderFillRect(renderer, &cell);
                }
                else
                {
                    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
                    SDL_RenderDrawRect(renderer, &cell);
                }
            }
        }

        // Dibujar la pieza actual (la que está cayendo)
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                // Si esta celda de la pieza está ocupada
                if (piece[row][col] == 1)
                {
                    // Calcular la posición en la grilla y en píxeles
                    int gridRow = pieceY + row;
                    int gridCol = pieceX + col;

                    // Verificar que esté dentro de los límites
                    if (gridRow >= 0 && gridRow < GRID_HEIGHT &&
                        gridCol >= 0 && gridCol < GRID_WIDTH)
                    {
                        SDL_Rect cell = {
                            gridCol * CELL_SIZE,
                            gridRow * CELL_SIZE,
                            CELL_SIZE - 1,
                            CELL_SIZE - 1};

                        // Dibujar en color amarillo (pieza O)
                        SDL_SetRenderDrawColor(renderer, 240, 240, 0, 255);
                        SDL_RenderFillRect(renderer, &cell);
                    }
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
