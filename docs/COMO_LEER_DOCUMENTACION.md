# Cómo Leer Documentación y Entender Librerías en C

## ¿Cómo sé cómo funciona SDL_Renderer?

Hay **3 formas** de aprender cómo usar una librería en C:

---

## 1. Documentación Oficial Online (La mejor)

### SDL2 Wiki (OFICIAL):
**https://wiki.libsdl.org/SDL2/FrontPage**

Búsquedas específicas:
- **SDL_Renderer**: https://wiki.libsdl.org/SDL2/SDL_Renderer
- **SDL_CreateRenderer**: https://wiki.libsdl.org/SDL2/SDL_CreateRenderer
- **SDL_RenderFillRect**: https://wiki.libsdl.org/SDL2/SDL_RenderFillRect
- **SDL_Rect**: https://wiki.libsdl.org/SDL2/SDL_Rect

### Cómo buscar en la documentación:

1. Ve a https://wiki.libsdl.org/SDL2/
2. En el buscador escribe: "SDL_CreateRenderer"
3. Verás:
   - **Syntax**: Cómo se usa la función
   - **Parameters**: Qué parámetros recibe
   - **Return Value**: Qué devuelve
   - **Remarks**: Explicaciones adicionales
   - **Example**: Código de ejemplo

---

## 2. Leer los archivos Header (.h)

En C, **los archivos .h definen TODA la API** de una librería.

### ¿Qué son los archivos header?

Son archivos que contienen:
- Declaraciones de funciones
- Definiciones de estructuras (structs)
- Constantes y enums
- Comentarios con documentación

### Ejemplo: SDL_Renderer

Vamos a ver el archivo que acabas de abrir: `SDL_render.h`

```bash
# Ver el archivo completo
cat /opt/homebrew/opt/sdl2/include/SDL2/SDL_render.h

# Buscar la definición de SDL_Renderer
grep -n "typedef.*SDL_Renderer" /opt/homebrew/opt/sdl2/include/SDL2/SDL_render.h
```

SDL_Renderer es un **tipo opaco** (opaque type):

```c
typedef struct SDL_Renderer SDL_Renderer;
```

Esto significa:
- Es un puntero a una estructura
- No ves su contenido interno (está oculto)
- Solo puedes usarlo a través de funciones de SDL
- **Es un patrón común en C** para encapsulación

### Buscar funciones en los headers:

```bash
# Buscar todas las funciones relacionadas con Renderer
grep "SDL_Renderer" /opt/homebrew/opt/sdl2/include/SDL2/SDL_render.h | head -20
```

---

## 3. Usar el comando `man` o herramientas de búsqueda

### En Mac/Linux:

```bash
# Ver la página man de SDL (si está instalada)
man SDL_CreateRenderer

# Buscar en todos los archivos .h de SDL2
grep -r "SDL_CreateRenderer" /opt/homebrew/opt/sdl2/include/
```

---

## Ejercicio Práctico: Investiguemos SDL_Renderer juntos

### Paso 1: Ver la definición en el header

```bash
# Buscar la definición de SDL_Renderer
grep -A 5 "struct SDL_Renderer" /opt/homebrew/opt/sdl2/include/SDL2/SDL_render.h
```

### Paso 2: Buscar funciones relacionadas

```bash
# Ver todas las funciones que usan SDL_Renderer
grep "SDL_Renderer" /opt/homebrew/opt/sdl2/include/SDL2/SDL_render.h | grep "extern"
```

Verás funciones como:
- `SDL_CreateRenderer()` - Crear renderer
- `SDL_DestroyRenderer()` - Destruir renderer
- `SDL_SetRenderDrawColor()` - Cambiar color
- `SDL_RenderClear()` - Limpiar pantalla
- `SDL_RenderFillRect()` - Dibujar rectángulo
- `SDL_RenderPresent()` - Mostrar en pantalla

### Paso 3: Leer la documentación de una función

Vamos a ver `SDL_CreateRenderer`:

```c
// De SDL_render.h
extern DECLSPEC SDL_Renderer * SDLCALL SDL_CreateRenderer(
    SDL_Window * window,
    int index,
    Uint32 flags
);
```

**Descomposición:**
- `SDL_Renderer *` → Devuelve un puntero a SDL_Renderer
- `SDL_Window * window` → Necesita una ventana
- `int index` → Índice del driver de renderizado (-1 = automático)
- `Uint32 flags` → Opciones (SDL_RENDERER_ACCELERATED, etc.)

---

## 4. Tipos de Datos en SDL2

### Tipos opacos (no ves el contenido):
```c
SDL_Window *     // Puntero a ventana
SDL_Renderer *   // Puntero a renderer
SDL_Texture *    // Puntero a textura
```

**No puedes hacer** `renderer->algo` porque no conoces su estructura interna.

### Tipos públicos (puedes ver y modificar):
```c
typedef struct SDL_Rect {
    int x, y;      // posición
    int w, h;      // ancho, alto
} SDL_Rect;
```

**Puedes hacer**:
```c
SDL_Rect rect;
rect.x = 100;
rect.y = 200;
rect.w = 50;
rect.h = 50;
```

---

## 5. Patrón común en C: Crear y Destruir

```c
// CREAR (devuelve puntero)
SDL_Renderer *renderer = SDL_CreateRenderer(...);

// USAR
SDL_RenderClear(renderer);
SDL_RenderFillRect(renderer, &rect);

// DESTRUIR (liberar memoria)
SDL_DestroyRenderer(renderer);
```

**Regla de oro**: Por cada `Create` debe haber un `Destroy`.

---

## 6. Cómo leo los FLAGS (opciones)

En SDL_render.h verás:

```c
typedef enum {
    SDL_RENDERER_SOFTWARE = 0x00000001,
    SDL_RENDERER_ACCELERATED = 0x00000002,
    SDL_RENDERER_PRESENTVSYNC = 0x00000004,
    SDL_RENDERER_TARGETTEXTURE = 0x00000008
} SDL_RendererFlags;
```

Estos son **flags de bits** que puedes combinar con `|`:

```c
// Un solo flag
SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

// Múltiples flags
SDL_CreateRenderer(window, -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
```

---

## 7. Ejemplo: Investigar SDL_Rect

### En el header:
```c
typedef struct SDL_Rect {
    int x, y;
    int w, h;
} SDL_Rect;
```

### En la documentación online:
https://wiki.libsdl.org/SDL2/SDL_Rect

### Uso:
```c
SDL_Rect rect = {100, 200, 50, 75};  // x, y, ancho, alto
// O también:
SDL_Rect rect;
rect.x = 100;
rect.y = 200;
rect.w = 50;
rect.h = 75;
```

---

## 8. Links útiles de SDL2

### Documentación oficial:
- **Wiki principal**: https://wiki.libsdl.org/SDL2/
- **Tutoriales**: https://lazyfoo.net/tutorials/SDL/
- **GitHub de SDL2**: https://github.com/libsdl-org/SDL

### Búsquedas específicas:
- SDL_Renderer: https://wiki.libsdl.org/SDL2/SDL_Renderer
- SDL_CreateRenderer: https://wiki.libsdl.org/SDL2/SDL_CreateRenderer
- SDL_RenderFillRect: https://wiki.libsdl.org/SDL2/SDL_RenderFillRect
- SDL_Rect: https://wiki.libsdl.org/SDL2/SDL_Rect
- SDL_Event: https://wiki.libsdl.org/SDL2/SDL_Event
- Todas las funciones: https://wiki.libsdl.org/SDL2/CategoryAPI

### Tutoriales en español:
- https://www.spaceinvaders.de/sdl/
- http://pysdl2.readthedocs.io/ (Python pero conceptos iguales)

---

## 9. Comandos útiles para explorar headers

```bash
# Ver todos los headers de SDL2
ls /opt/homebrew/opt/sdl2/include/SDL2/

# Buscar una función específica
grep -r "SDL_CreateRenderer" /opt/homebrew/opt/sdl2/include/SDL2/

# Ver definición de una estructura
grep -A 10 "typedef struct SDL_Rect" /opt/homebrew/opt/sdl2/include/SDL2/SDL_rect.h

# Buscar todos los enums
grep -r "typedef enum" /opt/homebrew/opt/sdl2/include/SDL2/ | head

# Ver comentarios de documentación
grep -B 5 "SDL_CreateRenderer" /opt/homebrew/opt/sdl2/include/SDL2/SDL_render.h
```

---

## 10. Resumen: Flujo de investigación

Cuando encuentres una función nueva:

1. **Busca en la wiki oficial**:
   - https://wiki.libsdl.org/SDL2/FunctionName

2. **Lee el header (.h)**:
   - Ve a `/opt/homebrew/opt/sdl2/include/SDL2/`
   - Busca la declaración de la función
   - Lee los comentarios

3. **Busca ejemplos**:
   - GitHub: "SDL_FunctionName example"
   - Lazy Foo tutorials
   - Stack Overflow

4. **Experimenta**:
   - Crea un pequeño programa de prueba
   - Compila y ejecuta
   - Lee los errores y aprende

---

## Ejercicio para ti:

Investiga estas funciones por tu cuenta:

1. **SDL_RenderDrawRect** (dibujar solo el borde)
   - https://wiki.libsdl.org/SDL2/SDL_RenderDrawRect
   - Diferencia con SDL_RenderFillRect

2. **SDL_SetRenderDrawBlendMode** (transparencias)
   - https://wiki.libsdl.org/SDL2/SDL_SetRenderDrawBlendMode

3. **SDL_RenderDrawLine** (dibujar líneas)
   - https://wiki.libsdl.org/SDL2/SDL_RenderDrawLine

Intenta usarlas en el programa!
