# Guía de FLAGS del Compilador

## ¿Qué son los FLAGS?

Los **flags** son opciones que le pasas al compilador (gcc) para modificar su comportamiento.

---

## FLAGS en nuestro Makefile:

### **Línea 3: CFLAGS (Compilation Flags)**
```makefile
CFLAGS = -Wall -I/opt/homebrew/opt/sdl2/include
```

#### `-Wall` (Warning All)
- **Qué hace**: Activa TODAS las advertencias del compilador
- **Por qué es útil**: Te avisa de posibles errores (variables sin usar, conversiones peligrosas, etc.)
- **Ejemplo**:
  ```c
  int x;  // variable declarada pero nunca usada
  // Con -Wall te advertirá: "unused variable 'x'"
  ```

#### `-I/ruta` (Include directory)
- **Qué hace**: Le dice al compilador dónde buscar archivos `.h` (headers)
- **Por qué lo necesitamos**: SDL2 está instalado en `/opt/homebrew/opt/sdl2/include`
- **Sin esto**: El compilador no encontraría `#include <SDL2/SDL.h>`
- **Ejemplo de error sin -I**:
  ```
  fatal error: SDL2/SDL.h: No such file or directory
  ```

Puedes tener múltiples `-I`:
```makefile
CFLAGS = -Wall -I/ruta1 -I/ruta2 -I/ruta3
```

---

### **Línea 4: LDFLAGS (Linker/Loader Flags)**
```makefile
LDFLAGS = -L/opt/homebrew/opt/sdl2/lib -lSDL2
```

#### `-L/ruta` (Library directory)
- **Qué hace**: Le dice al **linker** dónde buscar librerías compiladas (`.dylib`, `.so`, `.a`)
- **Por qué lo necesitamos**: Las librerías de SDL2 están en `/opt/homebrew/opt/sdl2/lib`
- **Sin esto**: El linker no encontraría la librería SDL2

#### `-lSDL2` (link with SDL2)
- **Qué hace**: Enlaza tu programa con la librería SDL2
- **Nota**: Se escribe `-l` seguido del nombre de la librería **sin** el prefijo "lib" ni la extensión
  - Archivo real: `libSDL2.dylib` (en Mac) o `libSDL2.so` (en Linux)
  - Flag: `-lSDL2`

**Sin esto**: Tendrías errores como:
```
Undefined symbols for architecture arm64:
  "_SDL_Init", referenced from: _main in main.o
  "_SDL_CreateWindow", referenced from: _main in main.o
  ...
```

Puedes enlazar múltiples librerías:
```makefile
LDFLAGS = -L/ruta1 -L/ruta2 -lSDL2 -lSDL2_image -lSDL2_ttf
```

---

## Otros FLAGS útiles que podrías agregar:

### **Flags de Compilación (CFLAGS)**

#### `-g` (debugging info)
```makefile
CFLAGS = -Wall -g -I/opt/homebrew/opt/sdl2/include
```
- Incluye información de depuración para usar con `gdb` o `lldb`
- Permite ver números de línea en crashes

#### `-O2` o `-O3` (optimization)
```makefile
CFLAGS = -Wall -O2 -I/opt/homebrew/opt/sdl2/include
```
- `-O0`: Sin optimización (por defecto, bueno para debug)
- `-O1`: Optimización básica
- `-O2`: Optimización estándar (recomendado para release)
- `-O3`: Optimización agresiva (puede aumentar tamaño del ejecutable)

#### `-std=c11` (estándar de C)
```makefile
CFLAGS = -Wall -std=c11 -I/opt/homebrew/opt/sdl2/include
```
- Especifica qué versión del lenguaje C usar
- Opciones: `-std=c89`, `-std=c99`, `-std=c11`, `-std=c17`

---

## Cómo se usan en la compilación:

Cuando ejecutas `make`, la línea 17 se expande a:

```bash
gcc -Wall -I/opt/homebrew/opt/sdl2/include main.c -o game -L/opt/homebrew/opt/sdl2/lib -lSDL2
```

**Descomposición:**
```bash
gcc                                    # El compilador
-Wall                                  # Mostrar warnings
-I/opt/homebrew/opt/sdl2/include      # Dónde buscar .h
main.c                                 # Archivo a compilar
-o game                                # Nombre del ejecutable
-L/opt/homebrew/opt/sdl2/lib          # Dónde buscar librerías
-lSDL2                                 # Enlazar con SDL2
```

---

## El proceso de compilación en 2 fases:

### **Fase 1: Compilación (usa CFLAGS)**
```
main.c + SDL.h → [gcc con -Wall -I] → main.o (objeto)
```

### **Fase 2: Linkeo (usa LDFLAGS)**
```
main.o + libSDL2.dylib → [linker con -L -l] → game (ejecutable)
```

---

## Ejemplo práctico:

Si quisieras agregar más flags para debug:

```makefile
CFLAGS = -Wall -g -std=c11 -I/opt/homebrew/opt/sdl2/include
LDFLAGS = -L/opt/homebrew/opt/sdl2/lib -lSDL2
```

Para release (optimizado):

```makefile
CFLAGS = -Wall -O2 -std=c11 -I/opt/homebrew/opt/sdl2/include
LDFLAGS = -L/opt/homebrew/opt/sdl2/lib -lSDL2
```

---

## Resumen visual:

```
CFLAGS → Opciones para COMPILAR (.c → .o)
         ├─ -Wall       : warnings
         ├─ -g          : debug info
         ├─ -O2         : optimización
         ├─ -std=c11    : versión de C
         └─ -I/ruta     : dónde buscar .h

LDFLAGS → Opciones para ENLAZAR (.o → ejecutable)
          ├─ -L/ruta    : dónde buscar librerías
          └─ -lNOMBRE   : enlazar con librería
```
