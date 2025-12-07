# Paso 14: Wall Kicks y Fix de Rotación

## Problemas resueltos

### Problema 1: Pieza I arranca en la segunda fila ✅

**Antes:**
```c
int pieceY = 0;  // La pieza aparece en la fila 1 del tablero
```

**Causa:** La pieza I está definida en la segunda fila de su matriz 4x4:
```
{0, 0, 0, 0},  ← Fila 0: vacía
{1, 1, 1, 1},  ← Fila 1: la pieza está aquí!
{0, 0, 0, 0},
{0, 0, 0, 0}
```

**Solución:**
```c
int pieceY = -1;  // Compensar el espacio vacío en la matriz
```

Ahora la pieza aparece en la fila 0 del tablero (top)!

### Problema 2: Rotación descentrada ✅

**Antes del fix:**
```
Pieza T contra pared:

Posición original:     Intentar rotar:
. . . . . .            . . . . . .
. ■ ■ ■ . .            . . ■ . . .
. . ■ . . .            . ■ ■ . . .  ← Se movió!
█ █ █ █ █ █            █ █ █ █ █ █
pared →                pared →

❌ Rotación fallaba porque la pieza se salía de la grilla
```

**Después del fix (con wall kicks):**
```
Pieza T contra pared:

Posición original:     Rotar con kick:
. . . . . .            . . . . . .
■ ■ ■ . . .            ■ . . . . .
. ■ . . . .            ■ ■ . . . .  ← Se ajustó!
█ █ █ █ █ █            █ █ █ █ █ █
pared →                pared →

✅ La función prueba mover la pieza a la izquierda (-1, 0)
   y la rotación funciona!
```

## ¿Qué son los Wall Kicks?

Los **wall kicks** son ajustes automáticos de posición que se prueban cuando una rotación falla.

### Secuencia de kicks implementada:

```c
int kicks[][2] = {
    {-1, 0},  // 1. Mover izquierda
    {1, 0},   // 2. Mover derecha
    {0, -1},  // 3. Mover arriba
    {-1, -1}, // 4. Diagonal arriba-izquierda
    {1, -1},  // 5. Diagonal arriba-derecha
    {-2, 0},  // 6. Mover 2 posiciones a la izquierda (pieza I)
    {2, 0},   // 7. Mover 2 posiciones a la derecha (pieza I)
};
```

### Cómo funciona:

1. **Intentar rotar** en la posición actual
2. Si falla, **probar kicks** en orden de prioridad
3. **Aplicar el primer kick** que funcione
4. Si ninguno funciona, **cancelar la rotación**

## Implementación

### Nueva función: `rotatePieceWithKicks()`

```c
bool rotatePieceWithKicks(int grid[GRID_HEIGHT][GRID_WIDTH],
                          int currentPiece[4][4],
                          int *x, int *y)
{
    // 1. Rotar la pieza
    int rotated[4][4];
    copyPiece(rotated, currentPiece);
    rotatePiece(rotated);

    // 2. Probar en posición actual
    if (!checkCollision(grid, rotated, *x, *y)) {
        copyPiece(currentPiece, rotated);
        return true;
    }

    // 3. Probar kicks
    for (int i = 0; i < 7; i++) {
        int newX = *x + kicks[i][0];
        int newY = *y + kicks[i][1];

        if (!checkCollision(grid, rotated, newX, newY)) {
            copyPiece(currentPiece, rotated);
            *x = newX;  // Ajustar posición!
            *y = newY;
            return true;
        }
    }

    return false;  // No se pudo rotar
}
```

### Uso en el game loop:

**Antes:**
```c
if (keystate[SDL_SCANCODE_UP]) {
    int rotatedPiece[4][4];
    copyPiece(rotatedPiece, currentPiece);
    rotatePiece(rotatedPiece);

    if (!checkCollision(grid, rotatedPiece, pieceX, pieceY)) {
        copyPiece(currentPiece, rotatedPiece);
    }
}
```

**Ahora:**
```c
if (keystate[SDL_SCANCODE_UP]) {
    rotatePieceWithKicks(grid, currentPiece, &pieceX, &pieceY);
    // ¡Automáticamente ajusta la posición si es necesario!
}
```

## Ejemplos visuales

### Ejemplo 1: Rotación simple (sin kick)

```
Antes:              Después:
. . . .             . . . .
. ■ ■ ■             . . ■ .
. . ■ .             . ■ ■ .
. . . .             . ■ . .

✅ No hay colisión, rota normalmente
```

### Ejemplo 2: Kick a la izquierda

```
Antes:              Intentar rotar:     Con kick (-1, 0):
. . . ■             . . . .             . . . .
. . . ■             . . . ■             . . ■ .
. . . ■  ←→        . . . ■  ←→        . . ■ .
. . . ■             . . . ■             . . ■ .
█ █ █ █             █ █ █ █             █ █ █ █
pared               ❌ colisión!        ✅ funciona!
```

### Ejemplo 3: Kick arriba

```
Antes:              Intentar rotar:     Con kick (0, -1):
. ■ ■ ■             . . ■ .             . . ■ .
. . ■ .             . ■ ■ .             . ■ ■ .
█ █ █ █             █ █ █ █             . ■ . .
piso                ❌ colisión!        ✅ funciona!
```

### Ejemplo 4: Pieza I con kick de 2 posiciones

```
Antes (I vertical):     Rotar a horizontal:    Con kick (-2, 0):
█ . . . ■ .             █ . . . . .            █ . . . . .
█ . . . ■ .             █ . . . . .            █ ■ ■ ■ ■ .
█ . . . ■ .     ←→     █ . . ■ ■ ■ ■   ←→    █ . . . . .
█ . . . ■ .             █ . . . . .            █ . . . . .
                        ❌ se sale!            ✅ funciona!
```

## Comparación con Tetris oficial

### Nuestro sistema (simple wall kicks):
- ✅ Funciona bien para la mayoría de casos
- ✅ Fácil de entender y mantener
- ✅ 7 kicks en orden de prioridad
- ⚠️ No es 100% idéntico al Tetris Guideline

### Sistema SRS oficial:
- ✅ Rotación perfecta según el estándar
- ✅ Kicks específicos por pieza y rotación
- ❌ Muy complejo (tabla de 280+ valores)
- ❌ Difícil de implementar correctamente

Para un proyecto educativo, **nuestro sistema es ideal**.

## Cambios en el código

### Archivos modificados:

**[main.c:232-278](../main.c#L232-L278)**
- Nueva función `rotatePieceWithKicks()`

**[main.c:455](../main.c#L455)**
- Cambio de `pieceY = 0` a `pieceY = -1`

**[main.c:550](../main.c#L550)**
- Nueva pieza también en `pieceY = -1`

**[main.c:607-613](../main.c#L607-L613)**
- Usar `rotatePieceWithKicks()` en lugar de lógica manual

## Beneficios

1. **Rotación más suave:** Las piezas pueden rotar cerca de paredes
2. **Menos frustración:** No se "traban" las rotaciones
3. **Más jugable:** Similar al Tetris que conoces
4. **Posición correcta:** Las piezas aparecen en la fila 0

## Próximos pasos opcionales

Si quieres mejorar aún más:

1. **Hard drop:** Presiona espacio para caída instantánea
2. **Ghost piece:** Mostrar dónde caerá la pieza
3. **Hold:** Guardar una pieza para después
4. **Preview:** Mostrar las próximas 3 piezas
5. **SRS completo:** Implementar el sistema oficial

## Prueba el fix

Compila y prueba:

```bash
make
./game
```

Intenta:
1. ✅ Rotar piezas cerca de las paredes
2. ✅ Rotar la pieza I horizontal/vertical cerca del borde
3. ✅ Rotar piezas L y T en espacios ajustados
4. ✅ Verificar que las piezas aparecen en la fila 0

¡Ahora la rotación funciona mucho mejor! 🎮
