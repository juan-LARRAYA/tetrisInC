# Sistema de Rotación en Tetris

## El problema de la rotación en matrices 4x4

### Problema 1: Pieza I arranca en la segunda fila

**Código actual:**
```c
// I - Cyan
{
    {0, 0, 0, 0},  // Fila 0
    {1, 1, 1, 1},  // Fila 1 ← La pieza está aquí
    {0, 0, 0, 0},  // Fila 2
    {0, 0, 0, 0}   // Fila 3
}
```

Cuando `pieceY = 0`, la pieza dibuja desde `y + row`, entonces:
- `y + 0 = 0` → Fila 0 del tablero (vacía en la matriz)
- `y + 1 = 1` → Fila 1 del tablero ← La pieza aparece aquí!

**Solución:** Empezar con `pieceY = -1` para que la pieza aparezca en la fila 0.

### Problema 2: Rotación descentrada (más importante)

Las piezas NO se rotan alrededor de su centro visual, sino del centro de la matriz 4x4.

#### Ejemplo con la pieza T:

**Estado original:**
```
Matriz 4x4:          Visualización en tablero:
0 0 0 0              . . . .
0 1 1 1  ←→         . ■ ■ ■
0 0 1 0              . . ■ .
0 0 0 0              . . . .
```

**Después de rotar 90° (con algoritmo actual):**
```
Matriz 4x4:          Visualización:
0 0 0 0              . . . .
0 0 1 0  ←→         . . ■ .
0 1 1 0              . ■ ■ .
0 1 0 0              . ■ . .
```

**Problema:** La pieza se "movió" hacia la derecha y abajo!

## Solución 1: Sistema SRS (Super Rotation System)

El Tetris oficial usa el **SRS** que incluye:

1. **Puntos de rotación específicos** para cada pieza
2. **Wall kicks** (ajustes de posición después de rotar)
3. **Tabla de offsets** por pieza y rotación

### Implementación del SRS

Cada pieza tiene 4 rotaciones (0°, 90°, 180°, 270°) y cada rotación tiene offsets específicos.

```c
// Offsets para cada rotación de cada pieza
// [tipo_pieza][rotacion][prueba][x/y]
const int SRS_OFFSETS[NUM_PIECES][4][5][2] = {
    // PIECE_I
    {
        // Rotación 0°
        {{0,0}, {-1,0}, {2,0}, {-1,0}, {2,0}},
        // Rotación 90°
        {{-1,0}, {0,0}, {0,0}, {0,1}, {0,-2}},
        // Rotación 180°
        {{-1,1}, {1,1}, {-2,1}, {1,0}, {-2,0}},
        // Rotación 270°
        {{0,1}, {0,1}, {0,1}, {0,-1}, {0,2}}
    },
    // ... más piezas
};
```

**Ventajas:**
- ✅ Rotación exacta como el Tetris oficial
- ✅ Las piezas no se "mueven" al rotar
- ✅ Permite rotar cerca de paredes

**Desventajas:**
- ❌ Muy complejo de implementar
- ❌ Muchos datos hardcodeados

## Solución 2: Kick simple (MÁS FÁCIL)

Un sistema más simple pero efectivo:

```c
void rotatePieceWithKick(int grid[GRID_HEIGHT][GRID_WIDTH],
                         int piece[4][4],
                         int *x, int *y)
{
    // 1. Rotar la pieza
    int rotated[4][4];
    copyPiece(rotated, piece);
    rotatePiece(rotated);

    // 2. Probar la rotación en la posición actual
    if (!checkCollision(grid, rotated, *x, *y)) {
        copyPiece(piece, rotated);
        return;
    }

    // 3. Probar kicks (ajustes de posición)
    int kicks[][2] = {
        {-1, 0},  // Mover izquierda
        {1, 0},   // Mover derecha
        {0, -1},  // Mover arriba
        {-1, -1}, // Diagonal arriba-izquierda
        {1, -1},  // Diagonal arriba-derecha
    };

    for (int i = 0; i < 5; i++) {
        int newX = *x + kicks[i][0];
        int newY = *y + kicks[i][1];

        if (!checkCollision(grid, rotated, newX, newY)) {
            copyPiece(piece, rotated);
            *x = newX;
            *y = newY;
            return;
        }
    }

    // Si ningún kick funcionó, no rotar
}
```

## Solución 3: Matrices más pequeñas (MÁS SIMPLE)

Usar matrices del tamaño exacto de cada pieza:

```c
// Pieza T: 3x3 en lugar de 4x4
const int PIECE_T[3][3] = {
    {0, 1, 0},
    {1, 1, 1},
    {0, 0, 0}
};

// Pieza I: 4x4 o 1x4 dependiendo de la rotación
const int PIECE_I_H[1][4] = {{1, 1, 1, 1}};  // Horizontal
const int PIECE_I_V[4][1] = {{1}, {1}, {1}, {1}};  // Vertical
```

**Ventajas:**
- ✅ Rotación más predecible
- ✅ Menos espacio desperdiciado

**Desventajas:**
- ❌ Más complejo de implementar
- ❌ Necesitas manejar diferentes tamaños

## Mi recomendación: Solución 2 + ajustes

Implementar wall kicks simples + ajustar la posición inicial:

### Paso 1: Ajustar posición inicial

```c
// En main(), cambiar:
int pieceY = -1;  // En lugar de 0
```

Esto hace que las piezas aparezcan más arriba, compensando el espacio vacío en la matriz.

### Paso 2: Implementar wall kicks básicos

Agregar la función `rotatePieceWithKick()` que prueba diferentes posiciones al rotar.

## Comparación visual

### Sin wall kicks:
```
Antes de rotar:     Después de rotar:
. . . .             . . . .
. ■ ■ ■             . . ■ .
. . ■ .             . ■ ■ .
█ █ █ . ← pared     █ █ █ . ← ¡La pieza se movió!
```

### Con wall kicks:
```
Antes de rotar:     Después de rotar:
. . . .             . . . .
■ ■ ■ .             . ■ . .
. ■ . .             . ■ ■ .
█ █ █ .             █ █ █ . ← Se ajustó automáticamente
```

## Código completo del fix

### Fix mínimo (solo posición inicial):

```c
// En main.c, línea ~481
int pieceY = -1;  // Cambiar de 0 a -1
```

### Fix completo (con wall kicks):

Ver archivo: `ROTACION_WALL_KICKS_IMPLEMENTACION.md`

## Recursos adicionales

- [Tetris Wiki - SRS](https://tetris.wiki/SRS)
- [Tetris Guideline](https://tetris.wiki/Tetris_Guideline)
- Especificación oficial del sistema de rotación

## Próximos pasos

1. ✅ Entender el problema
2. ⬜ Implementar fix de posición inicial
3. ⬜ Implementar wall kicks básicos
4. ⬜ Opcional: Implementar SRS completo
