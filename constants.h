#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SDL.h>

// ============ CONFIGURACIÓN DEL TABLERO ============
#define GRID_WIDTH 10   // Columnas del tablero
#define GRID_HEIGHT 20  // Filas del tablero
#define CELL_SIZE 30    // Tamaño de cada celda en píxeles

// Dimensiones de la ventana (calculadas automáticamente)
#define WINDOW_WIDTH (GRID_WIDTH * CELL_SIZE)
#define WINDOW_HEIGHT (GRID_HEIGHT * CELL_SIZE)

// ============ CONFIGURACIÓN DEL JUEGO ============
#define FALL_DELAY 500       // Velocidad de caída (ms entre cada caída)
#define MOVE_DELAY 100       // Delay para movimiento lateral/abajo (ms)
#define ROTATE_DELAY 150     // Delay para rotación (ms)
#define TARGET_FPS 60        // FPS objetivo
#define FRAME_DELAY (1000 / TARGET_FPS)  // Delay entre frames (ms)

// Configuración de spawn de piezas
#define SPAWN_X 3            // Columna inicial (centro)
#define SPAWN_Y 0            // Fila inicial (arriba)

// ============ TIPOS DE PIEZAS ============
typedef enum
{
    PIECE_I = 0, // Barra
    PIECE_O,     // Cuadrado
    PIECE_T,     // T
    PIECE_S,     // S
    PIECE_Z,     // Z
    PIECE_J,     // J
    PIECE_L,     // L
    NUM_PIECES   // Total de piezas (7)
} PieceType;

// ============ DEFINICIÓN DE LAS 7 PIEZAS ============
// Cada pieza es una matriz 4×4
static const int PIECES[NUM_PIECES][4][4] = {
    // I - Cyan (barra horizontal en la primera fila)
    {
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },

    // O - Amarillo (cuadrado)
    {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    },

    // T - Morado
    {
        {0, 0, 0, 0},
        {0, 1, 1, 1},
        {0, 0, 1, 0},
        {0, 0, 0, 0}
    },

    // S - Verde
    {
        {0, 0, 0, 0},
        {0, 0, 1, 1},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    },

    // Z - Rojo
    {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 1, 1},
        {0, 0, 0, 0}
    },

    // J - Azul
    {
        {0, 0, 0, 0},
        {0, 1, 1, 1},
        {0, 0, 0, 1},
        {0, 0, 0, 0}
    },

    // L - Naranja
    {
        {0, 0, 0, 0},
        {0, 1, 1, 1},
        {0, 1, 0, 0},
        {0, 0, 0, 0}
    },
};

// ============ COLORES DE LAS PIEZAS ============
// Colores RGB para cada tipo de pieza (formato SDL_Color)
static const SDL_Color PIECE_COLORS[NUM_PIECES] = {
    {0, 240, 240, 255},   // I - Cyan (aguamarina)
    {240, 240, 0, 255},   // O - Amarillo
    {160, 0, 240, 255},   // T - Morado
    {0, 240, 0, 255},     // S - Verde
    {240, 0, 0, 255},     // Z - Rojo
    {0, 0, 240, 255},     // J - Azul
    {240, 160, 0, 255}    // L - Naranja
};

// ============ COLORES DEL JUEGO ============
// Colores para la interfaz
#define COLOR_BACKGROUND_R 0
#define COLOR_BACKGROUND_G 0
#define COLOR_BACKGROUND_B 0
#define COLOR_BACKGROUND_A 255

#define COLOR_GRID_R 40
#define COLOR_GRID_G 40
#define COLOR_GRID_B 40
#define COLOR_GRID_A 255

// ============ PUNTUACIÓN ============
// Multiplicador de puntos por líneas eliminadas
// 1 línea = 1² × 100 = 100 puntos
// 2 líneas = 2² × 100 = 400 puntos
// 3 líneas = 3² × 100 = 900 puntos
// 4 líneas = 4² × 100 = 1600 puntos (Tetris!)
#define POINTS_MULTIPLIER 100

// ============ WALL KICKS ============
// Configuración de wall kicks para rotación
#define NUM_WALL_KICKS 7

static const int WALL_KICKS[NUM_WALL_KICKS][2] = {
    {-1, 0},  // Kick izquierda
    {1, 0},   // Kick derecha
    {0, -1},  // Kick arriba
    {-1, -1}, // Kick diagonal arriba-izquierda
    {1, -1},  // Kick diagonal arriba-derecha
    {-2, 0},  // Kick 2 posiciones a la izquierda (para pieza I)
    {2, 0},   // Kick 2 posiciones a la derecha (para pieza I)
};

#endif // CONSTANTS_H
