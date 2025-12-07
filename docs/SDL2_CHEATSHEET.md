# SDL2 Cheat Sheet - Funciones Esenciales

## Inicialización y Ventanas

### SDL_Init()
```c
int SDL_Init(Uint32 flags);
```
- **Qué hace**: Inicializa SDL
- **Flags comunes**:
  - `SDL_INIT_VIDEO` - Sistema de video
  - `SDL_INIT_AUDIO` - Sistema de audio
  - `SDL_INIT_EVERYTHING` - Todo
- **Retorno**: 0 si éxito, negativo si error
- **Ejemplo**:
  ```c
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      printf("Error: %s\n", SDL_GetError());
  }
  ```

### SDL_CreateWindow()
```c
SDL_Window* SDL_CreateWindow(
    const char* title,    // Título de la ventana
    int x,                // Posición X (SDL_WINDOWPOS_CENTERED)
    int y,                // Posición Y (SDL_WINDOWPOS_CENTERED)
    int w,                // Ancho
    int h,                // Alto
    Uint32 flags          // SDL_WINDOW_SHOWN, SDL_WINDOW_FULLSCREEN, etc.
);
```
- **Retorno**: Puntero a ventana, NULL si error

### SDL_Quit()
```c
void SDL_Quit(void);
```
- **Qué hace**: Cierra SDL y libera recursos
- **Llamar**: Al final del programa

---

## Renderer (Sistema de dibujo)

### SDL_CreateRenderer()
```c
SDL_Renderer* SDL_CreateRenderer(
    SDL_Window* window,   // Ventana donde dibujar
    int index,            // -1 = automático
    Uint32 flags          // SDL_RENDERER_ACCELERATED, etc.
);
```
- **Flags**:
  - `SDL_RENDERER_SOFTWARE` - Sin aceleración
  - `SDL_RENDERER_ACCELERATED` - Con GPU
  - `SDL_RENDERER_PRESENTVSYNC` - Sincronizar con monitor

### SDL_SetRenderDrawColor()
```c
int SDL_SetRenderDrawColor(
    SDL_Renderer* renderer,
    Uint8 r,              // Rojo (0-255)
    Uint8 g,              // Verde (0-255)
    Uint8 b,              // Azul (0-255)
    Uint8 a               // Alpha/transparencia (0-255)
);
```
- **Ejemplo**:
  ```c
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // rojo
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // verde
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // azul
  ```

### SDL_RenderClear()
```c
int SDL_RenderClear(SDL_Renderer* renderer);
```
- **Qué hace**: Limpia toda la pantalla con el color actual
- **Usar**: Al inicio de cada frame

### SDL_RenderPresent()
```c
void SDL_RenderPresent(SDL_Renderer* renderer);
```
- **Qué hace**: Muestra lo dibujado en pantalla (swap buffers)
- **Usar**: Al final de cada frame, después de dibujar todo

### SDL_DestroyRenderer()
```c
void SDL_DestroyRenderer(SDL_Renderer* renderer);
```
- **Qué hace**: Libera memoria del renderer
- **Usar**: Al final del programa

---

## Dibujar Formas

### SDL_Rect (estructura)
```c
typedef struct SDL_Rect {
    int x, y;     // posición
    int w, h;     // ancho, alto
} SDL_Rect;
```
- **Ejemplo**:
  ```c
  SDL_Rect rect = {100, 200, 50, 75};  // x, y, w, h
  ```

### SDL_RenderFillRect()
```c
int SDL_RenderFillRect(
    SDL_Renderer* renderer,
    const SDL_Rect* rect    // NULL = toda la pantalla
);
```
- **Qué hace**: Dibuja un rectángulo RELLENO
- **Ejemplo**:
  ```c
  SDL_Rect rect = {100, 100, 50, 50};
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(renderer, &rect);
  ```

### SDL_RenderDrawRect()
```c
int SDL_RenderDrawRect(
    SDL_Renderer* renderer,
    const SDL_Rect* rect
);
```
- **Qué hace**: Dibuja solo el BORDE del rectángulo

### SDL_RenderDrawLine()
```c
int SDL_RenderDrawLine(
    SDL_Renderer* renderer,
    int x1, int y1,    // punto inicial
    int x2, int y2     // punto final
);
```
- **Ejemplo**:
  ```c
  SDL_RenderDrawLine(renderer, 0, 0, 800, 600); // diagonal
  ```

### SDL_RenderDrawPoint()
```c
int SDL_RenderDrawPoint(
    SDL_Renderer* renderer,
    int x, int y
);
```
- **Qué hace**: Dibuja un solo píxel

---

## Eventos (Input del usuario)

### SDL_Event (estructura)
```c
typedef union SDL_Event {
    Uint32 type;              // Tipo de evento
    SDL_KeyboardEvent key;    // Evento de teclado
    SDL_MouseMotionEvent motion;
    SDL_MouseButtonEvent button;
    // ... muchos más
} SDL_Event;
```

### SDL_PollEvent()
```c
int SDL_PollEvent(SDL_Event* event);
```
- **Qué hace**: Revisa si hay eventos pendientes
- **Retorno**: 1 si hay evento, 0 si no hay
- **Usar**: En un loop dentro del game loop
- **Ejemplo**:
  ```c
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
          running = false;
      }
  }
  ```

### Tipos de eventos comunes:
```c
SDL_QUIT              // Usuario cierra ventana
SDL_KEYDOWN           // Tecla presionada
SDL_KEYUP             // Tecla soltada
SDL_MOUSEBUTTONDOWN   // Click de mouse
SDL_MOUSEBUTTONUP     // Soltar click
SDL_MOUSEMOTION       // Movimiento de mouse
```

### SDL_GetKeyboardState()
```c
const Uint8* SDL_GetKeyboardState(int* numkeys);
```
- **Qué hace**: Devuelve array con estado ACTUAL de todas las teclas
- **Usar**: Para movimiento continuo
- **Ejemplo**:
  ```c
  const Uint8* keys = SDL_GetKeyboardState(NULL);
  if (keys[SDL_SCANCODE_W]) {
      // W está presionada
  }
  if (keys[SDL_SCANCODE_SPACE]) {
      // Espacio está presionado
  }
  ```

### Scancodes comunes:
```c
SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT
SDL_SCANCODE_W, SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D
SDL_SCANCODE_SPACE
SDL_SCANCODE_RETURN  // Enter
SDL_SCANCODE_ESCAPE
```

---

## Tiempo y Delays

### SDL_Delay()
```c
void SDL_Delay(Uint32 ms);
```
- **Qué hace**: Pausa el programa por N milisegundos
- **Ejemplo**: `SDL_Delay(16)` = ~60 FPS

### SDL_GetTicks()
```c
Uint32 SDL_GetTicks(void);
```
- **Qué hace**: Devuelve milisegundos desde que SDL_Init() fue llamado
- **Usar**: Para timing, animaciones, etc.
- **Ejemplo**:
  ```c
  Uint32 start = SDL_GetTicks();
  // ... hacer algo ...
  Uint32 elapsed = SDL_GetTicks() - start;
  ```

---

## Errores

### SDL_GetError()
```c
const char* SDL_GetError(void);
```
- **Qué hace**: Devuelve el último mensaje de error
- **Ejemplo**:
  ```c
  if (window == NULL) {
      printf("Error: %s\n", SDL_GetError());
  }
  ```

---

## Patrón típico de un juego en SDL2

```c
#include <SDL2/SDL.h>
#include <stdbool.h>

int main() {
    // 1. INICIALIZAR
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Juego",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED);

    // 2. VARIABLES DEL JUEGO
    bool running = true;
    SDL_Event event;
    int x = 100, y = 100;

    // 3. GAME LOOP
    while (running) {
        // A. EVENTOS
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // B. UPDATE
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_RIGHT]) x += 5;
        if (keys[SDL_SCANCODE_LEFT]) x -= 5;

        // C. RENDER
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect rect = {x, y, 50, 50};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);

        SDL_RenderPresent(renderer);

        // D. FPS
        SDL_Delay(16);
    }

    // 4. CLEANUP
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
```

---

## Colores RGB Comunes

```c
// Básicos
SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);       // Negro
SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanco
SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);     // Rojo
SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);     // Verde
SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);     // Azul

// Secundarios
SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);   // Amarillo
SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);   // Magenta
SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);   // Cyan

// Grises
SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Gris
SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);    // Gris oscuro

// Colores de Tetris
SDL_SetRenderDrawColor(renderer, 0, 240, 240, 255);   // Cyan (I)
SDL_SetRenderDrawColor(renderer, 240, 160, 0, 255);   // Naranja (L)
SDL_SetRenderDrawColor(renderer, 0, 0, 240, 255);     // Azul (J)
SDL_SetRenderDrawColor(renderer, 240, 240, 0, 255);   // Amarillo (O)
SDL_SetRenderDrawColor(renderer, 0, 240, 0, 255);     // Verde (S)
SDL_SetRenderDrawColor(renderer, 160, 0, 240, 255);   // Morado (T)
SDL_SetRenderDrawColor(renderer, 240, 0, 0, 255);     // Rojo (Z)
```

---

## Links Útiles

- **Wiki Oficial**: https://wiki.libsdl.org/SDL2/
- **Lazy Foo Tutorials**: https://lazyfoo.net/tutorials/SDL/
- **GitHub SDL2**: https://github.com/libsdl-org/SDL
