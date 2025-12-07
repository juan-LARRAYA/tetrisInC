#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // Para rand() y srand()
#include <time.h>   // Para time()
#include <string.h> // Para strcspn()
#include "constants.h" // Constantes del juego (piezas, colores, configuración)
#include "database.h"  // Sistema de usuarios y puntajes

// ============ FUNCIONES DE COLISIÓN ============
// Verifica si una pieza puede estar en una posición dada
bool checkCollision(int grid[GRID_HEIGHT][GRID_WIDTH], int piece[4][4], int x, int y)
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (piece[row][col] == 1)
            {
                int gridRow = y + row;
                int gridCol = x + col;

                // Verificar límites de la grilla
                if (gridCol < 0 || gridCol >= GRID_WIDTH)
                    return true; // Colisión con bordes laterales

                if (gridRow >= GRID_HEIGHT)
                    return true; // Colisión con el fondo

                // Verificar colisión con piezas ya colocadas
                if (gridRow >= 0 && grid[gridRow][gridCol] == 1)
                    return true;
            }
        }
    }
    return false; // No hay colisión
}

// Fija la pieza actual en la grilla
void lockPiece(int grid[GRID_HEIGHT][GRID_WIDTH], int piece[4][4], int x, int y)
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (piece[row][col] == 1)
            {
                int gridRow = y + row;
                int gridCol = x + col;

                if (gridRow >= 0 && gridRow < GRID_HEIGHT &&
                    gridCol >= 0 && gridCol < GRID_WIDTH)
                {
                    grid[gridRow][gridCol] = 1;
                }
            }
        }
    }
}

// Verifica si una fila está completa (todas las celdas ocupadas)
bool isLineComplete(int grid[GRID_HEIGHT][GRID_WIDTH], int row)
{
    for (int col = 0; col < GRID_WIDTH; col++)
    {
        if (grid[row][col] == 0)
            return false; // Hay una celda vacía
    }
    return true; // Todas las celdas están ocupadas
}

// Elimina una fila y hace caer las de arriba
void clearLine(int grid[GRID_HEIGHT][GRID_WIDTH], int lineRow)
{
    // Mover todas las filas de arriba hacia abajo
    for (int row = lineRow; row > 0; row--)
    {
        for (int col = 0; col < GRID_WIDTH; col++)
        {
            grid[row][col] = grid[row - 1][col];
        }
    }
    // La fila superior queda vacía
    for (int col = 0; col < GRID_WIDTH; col++)
    {
        grid[0][col] = 0;
    }
}

// Verifica y elimina todas las líneas completas
int clearCompleteLines(int grid[GRID_HEIGHT][GRID_WIDTH])
{
    int linesCleared = 0;

    // Revisar de abajo hacia arriba
    for (int row = GRID_HEIGHT - 1; row >= 0; row--)
    {
        if (isLineComplete(grid, row))
        {
            clearLine(grid, row);
            linesCleared++;
            row++; // Volver a revisar esta fila (porque ahora tiene contenido nuevo)
        }
    }

    return linesCleared;
}

// Genera un tipo de pieza aleatorio
PieceType getRandomPiece()
{
    return (PieceType)(rand() % NUM_PIECES);
}

// Copia una pieza del array PIECES a currentPiece
void copyPiece(int dest[4][4], const int src[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            dest[i][j] = src[i][j];
        }
    }
}

// Rota una pieza 90 grados en sentido horario
// Algoritmo: transponer + invertir cada fila
void rotatePiece(int piece[4][4])
{
    // Crear una copia temporal
    int temp[4][4];

    // Transponer la matriz (intercambiar filas por columnas)
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            temp[i][j] = piece[j][i];
        }
    }

    // Invertir cada fila para completar la rotación
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            piece[i][j] = temp[i][4 - 1 - j];
        }
    }
}

// Rota una pieza con wall kicks (ajustes de posición)
// Devuelve true si se pudo rotar, false si no
bool rotatePieceWithKicks(int grid[GRID_HEIGHT][GRID_WIDTH],
                          int currentPiece[4][4],
                          int *x, int *y)
{
    // 1. Crear una copia y rotarla
    int rotated[4][4];
    copyPiece(rotated, currentPiece);
    rotatePiece(rotated);

    // 2. Probar la rotación en la posición actual (sin kick)
    if (!checkCollision(grid, rotated, *x, *y))
    {
        copyPiece(currentPiece, rotated);
        return true;
    }

    // 3. Probar diferentes kicks (ajustes de posición)
    // Orden de prioridad: izquierda, derecha, arriba, combinaciones
    for (int i = 0; i < NUM_WALL_KICKS; i++)
    {
        int newX = *x + WALL_KICKS[i][0];
        int newY = *y + WALL_KICKS[i][1];

        if (!checkCollision(grid, rotated, newX, newY))
        {
            copyPiece(currentPiece, rotated);
            *x = newX;
            *y = newY;
            return true;
        }
    }

    // No se pudo rotar en ninguna posición
    return false;
}

// Menú de login/registro en la terminal
bool showLoginMenu(char* username) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║           🎮  TETRIS EN C  🎮                   ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("  1. 🔐 Iniciar sesión\n");
    printf("  2. ✨ Registrar nuevo usuario\n");
    printf("  3. 🏆 Ver top 10 puntajes\n");
    printf("  4. 🚪 Salir\n");
    printf("\n");
    printf("Opción: ");
    fflush(stdout);

    int option;
    if (scanf("%d", &option) != 1) {
        // Limpiar el buffer si la entrada no es válida
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        printf("❌ Entrada inválida. Intenta de nuevo.\n");
        return false;
    }

    // Limpiar el buffer del newline
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    char password[50];

    switch (option) {
        case 1: // Login
            printf("\n--- INICIAR SESIÓN ---\n");
            printf("Usuario: ");
            fflush(stdout);
            if (fgets(username, 50, stdin) == NULL) return false;
            username[strcspn(username, "\n")] = 0; // Remover newline

            printf("Contraseña: ");
            fflush(stdout);
            if (fgets(password, 50, stdin) == NULL) return false;
            password[strcspn(password, "\n")] = 0;

            if (loginUser(username, password)) {
                printf("\n✅ ¡Bienvenido, %s!\n", username);
                printf("Preparando el juego...\n\n");
                return true;
            } else {
                printf("\n❌ Credenciales incorrectas.\n");
                return false;
            }

        case 2: // Registro
            printf("\n--- REGISTRAR USUARIO ---\n");
            printf("Nuevo usuario: ");
            fflush(stdout);
            if (fgets(username, 50, stdin) == NULL) return false;
            username[strcspn(username, "\n")] = 0;

            printf("Contraseña: ");
            fflush(stdout);
            if (fgets(password, 50, stdin) == NULL) return false;
            password[strcspn(password, "\n")] = 0;

            if (registerUser(username, password)) {
                printf("\n✅ ¡Usuario '%s' registrado exitosamente!\n", username);
                printf("Ahora puedes iniciar sesión (opción 1).\n");
            }
            return false;

        case 3: // Ver top scores
            printTopScores();
            printf("Presiona ENTER para continuar...");
            getchar();
            return false;

        case 4: // Salir
            printf("\n👋 ¡Hasta luego!\n\n");
            exit(0);

        default:
            printf("❌ Opción inválida.\n");
            return false;
    }
}

int main(int argc, char *argv[])
{
    // Inicializar generador de números aleatorios
    srand(time(NULL));

    // Mensaje de bienvenida
    printf("\033[2J\033[H"); // Limpiar pantalla
    printf("════════════════════════════════════════════════════════\n");
    printf("    🎮  BIENVENIDO A TETRIS EN C  🎮\n");
    printf("════════════════════════════════════════════════════════\n");
    printf("\n💡 TIP: Interactúa con este menú en la terminal\n");
    printf("   La ventana del juego se abrirá después del login.\n\n");

    // Inicializar base de datos
    if (!initDatabase()) {
        printf("⚠️  Error al inicializar base de datos.\n");
        printf("   El juego continuará sin guardar puntajes.\n\n");
    }

    // Menú de login/registro
    char username[50];
    bool loggedIn = false;

    while (!loggedIn) {
        loggedIn = showLoginMenu(username);
    }

    // Inicializar SDL (sistema de video)
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error al inicializar SDL: %s\n", SDL_GetError());
        closeDatabase();
        return 1;
    }
    printf("Sistema de video iniciado con éxito. ¡A jugar!\n");

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

    // ============ PIEZA ACTUAL (la que está cayendo) ============
    int pieceX = SPAWN_X;       // columna (empezar en el centro)
    int pieceY = SPAWN_Y;       // fila (arriba del todo)
    PieceType currentType = getRandomPiece(); // Tipo de pieza actual (ALEATORIO!)

    // Copiar la forma de la pieza actual
    int currentPiece[4][4];
    copyPiece(currentPiece, PIECES[currentType]);

    // Timer para la caída automática
    Uint32 lastFallTime = SDL_GetTicks();
    Uint32 fallDelay = FALL_DELAY; // Velocidad de caída

    // Puntuación
    int score = 0;
    int totalLinesCleared = 0;

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
                // Guardar el puntaje actual antes de salir
                printf("\n=== PARTIDA GUARDADA ===\n");
                printf("Usuario: %s\n", username);
                printf("Puntuación: %d\n", score);
                printf("Líneas eliminadas: %d\n", totalLinesCleared);

                if (saveScore(username, score, totalLinesCleared)) {
                    printf("¡Puntaje guardado exitosamente!\n");
                }

                printTopScores();
                running = false; // salir del loop
            }

            // Si el usuario presiona una tecla
            if (event.type == SDL_KEYDOWN)
            {
                // Si presiona ESC, guardar y cerrar
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    // Guardar el puntaje actual antes de salir
                    printf("\n=== PARTIDA GUARDADA ===\n");
                    printf("Usuario: %s\n", username);
                    printf("Puntuación: %d\n", score);
                    printf("Líneas eliminadas: %d\n", totalLinesCleared);

                    if (saveScore(username, score, totalLinesCleared)) {
                        printf("¡Puntaje guardado exitosamente!\n");
                    }

                    printTopScores();
                    running = false;
                }
            }
        }

        // 2. UPDATE (actualizar lógica del juego)

        // Caída automática de la pieza
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastFallTime >= fallDelay)
        {
            // Intentar mover la pieza hacia abajo
            if (!checkCollision(grid, currentPiece, pieceX, pieceY + 1))
            {
                pieceY++; // mover hacia abajo si no hay colisión
            }
            else
            {
                // COLISIÓN: La pieza tocó el fondo o otra pieza
                // Fijar la pieza en la grilla
                lockPiece(grid, currentPiece, pieceX, pieceY);

                // Verificar y eliminar líneas completas
                int linesCleared = clearCompleteLines(grid);
                if (linesCleared > 0)
                {
                    totalLinesCleared += linesCleared;
                    // Sistema de puntuación: más líneas a la vez = más puntos
                    int points = linesCleared * linesCleared * POINTS_MULTIPLIER;
                    score += points;
                    printf("¡%d línea(s) eliminada(s)! Puntos: +%d | Total: %d\n",
                           linesCleared, points, score);
                }

                // Crear nueva pieza arriba (ALEATORIA!)
                pieceX = SPAWN_X;
                pieceY = SPAWN_Y;
                currentType = getRandomPiece();
                copyPiece(currentPiece, PIECES[currentType]);

                // Verificar Game Over
                if (checkCollision(grid, currentPiece, pieceX, pieceY))
                {
                    printf("\n=== GAME OVER ===\n");
                    printf("Usuario: %s\n", username);
                    printf("Puntuación final: %d\n", score);
                    printf("Líneas eliminadas: %d\n", totalLinesCleared);

                    // Guardar puntaje en la base de datos
                    if (saveScore(username, score, totalLinesCleared)) {
                        printf("¡Puntaje guardado exitosamente!\n");
                    }

                    // Mostrar tabla de mejores puntajes
                    printTopScores();

                    running = false;
                }
            }

            // Reiniciar el timer
            lastFallTime = currentTime;
        }

        // Control manual con flechas
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_LEFT])
        {
            // Intentar mover izquierda solo si no hay colisión
            if (!checkCollision(grid, currentPiece, pieceX - 1, pieceY))
            {
                pieceX--;
            }
            SDL_Delay(MOVE_DELAY);
        }
        if (keystate[SDL_SCANCODE_RIGHT])
        {
            // Intentar mover derecha solo si no hay colisión
            if (!checkCollision(grid, currentPiece, pieceX + 1, pieceY))
            {
                pieceX++;
            }
            SDL_Delay(MOVE_DELAY);
        }
        if (keystate[SDL_SCANCODE_DOWN])
        {
            // Caída rápida
            if (!checkCollision(grid, currentPiece, pieceX, pieceY + 1))
            {
                pieceY++;
            }
            SDL_Delay(MOVE_DELAY);
        }
        if (keystate[SDL_SCANCODE_UP])
        {
            // Rotar la pieza con wall kicks
            // La función ajustará automáticamente la posición si es necesario
            rotatePieceWithKicks(grid, currentPiece, &pieceX, &pieceY);
            SDL_Delay(ROTATE_DELAY);
        }

        // 3. RENDER (dibujar en pantalla)
        // Limpiar la pantalla con un color de fondo
        SDL_SetRenderDrawColor(renderer, COLOR_BACKGROUND_R, COLOR_BACKGROUND_G, COLOR_BACKGROUND_B, COLOR_BACKGROUND_A);
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
                    // Piezas fijadas (usar color cyan por ahora)
                    SDL_SetRenderDrawColor(renderer, 0, 240, 240, 255);
                    SDL_RenderFillRect(renderer, &cell);
                }
                else
                {
                    // Grilla vacía
                    SDL_SetRenderDrawColor(renderer, COLOR_GRID_R, COLOR_GRID_G, COLOR_GRID_B, COLOR_GRID_A);
                    SDL_RenderDrawRect(renderer, &cell);
                }
            }
        }

        // Dibujar la pieza actual (la que está cayendo)
        SDL_Color color = PIECE_COLORS[currentType];
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                // Si esta celda de la pieza está ocupada
                if (currentPiece[row][col] == 1)
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

                        // Dibujar con el color correspondiente a la pieza
                        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                        SDL_RenderFillRect(renderer, &cell);
                    }
                }
            }
        }

        // Mostrar lo que dibujamos (swap buffers)
        SDL_RenderPresent(renderer);

        // 4. CONTROL DE FPS
        // Esperar un poco para no consumir 100% del CPU
        SDL_Delay(FRAME_DELAY);
    }

    // Limpiar y cerrar
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    closeDatabase();

    return 0;
}
