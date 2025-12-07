# Tetris en C

Un juego de Tetris completo implementado en C con SDL2 y SQLite.

## Características

- **Juego completo de Tetris** con las 7 piezas clásicas
- **Sistema de rotación** de piezas
- **Sistema de colisiones** preciso
- **Eliminación de líneas** y puntuación
- **Sistema de usuarios** con login y registro
- **Base de datos SQLite** para guardar puntajes persistentes
- **Tabla de high scores** (top 10 mejores puntajes)

## Requisitos

- macOS (probado en macOS con Homebrew)
- SDL2
- SQLite3
- gcc

## Instalación

1. Instalar dependencias con Homebrew:
```bash
brew install sdl2 sqlite
```

2. Compilar el juego:
```bash
make
```

## Cómo jugar

1. Ejecutar el juego:
```bash
./game
```

2. **Menú principal:**
   - Opción 1: Iniciar sesión (con usuario existente)
   - Opción 2: Registrar nuevo usuario
   - Opción 3: Ver top 10 puntajes
   - Opción 4: Salir

3. **Controles del juego:**
   - **← Flecha Izquierda**: Mover pieza a la izquierda
   - **→ Flecha Derecha**: Mover pieza a la derecha
   - **↓ Flecha Abajo**: Caída rápida
   - **↑ Flecha Arriba**: Rotar pieza 90° en sentido horario
   - **ESC**: Salir del juego

## Sistema de puntuación

- 1 línea = 100 puntos
- 2 líneas = 400 puntos (2² × 100)
- 3 líneas = 900 puntos (3² × 100)
- 4 líneas = 1600 puntos (4² × 100)

## Base de datos

El juego crea automáticamente un archivo `tetris.db` que contiene:

- **Tabla users**: Almacena usuarios con username y password
- **Tabla scores**: Almacena puntajes con fecha, líneas eliminadas y username

Los puntajes se guardan automáticamente al terminar cada partida y se muestran los top 10 mejores puntajes.

## Estructura del proyecto

```
.
├── main.c          # Lógica principal del juego
├── database.h      # Header del sistema de base de datos
├── database.c      # Implementación de la base de datos
├── Makefile        # Archivo de compilación
├── tetris.db       # Base de datos SQLite (generada automáticamente)
└── README.md       # Este archivo
```

## Compilación manual

Si quieres compilar manualmente sin usar el Makefile:

```bash
gcc -Wall \
  -I/opt/homebrew/opt/sdl2/include \
  -I/opt/homebrew/opt/sqlite/include \
  main.c database.c \
  -o game \
  -L/opt/homebrew/opt/sdl2/lib -lSDL2 \
  -L/opt/homebrew/opt/sqlite/lib -lsqlite3
```

## Comandos útiles

- `make`: Compilar el juego
- `make clean`: Limpiar archivos compilados
- `make run`: Compilar y ejecutar

## Game Over

Cuando el juego termina:
1. Se guarda automáticamente tu puntaje en la base de datos
2. Se muestra tu puntuación final y líneas eliminadas
3. Se muestra la tabla de top 10 puntajes
4. La ventana se cierra

## Notas de seguridad

**IMPORTANTE**: Este es un proyecto educativo. Las contraseñas se almacenan en **texto plano** en la base de datos.

Para un proyecto de producción, deberías:
- Usar hashing de contraseñas (bcrypt, argon2, etc.)
- Agregar salt a las contraseñas
- Implementar límites de intentos de login
- Validar inputs del usuario

## Autor

Desarrollado como proyecto de aprendizaje de C, SDL2 y SQLite.
