# Configuración del compilador
CC = gcc
CFLAGS = -Wall -I/opt/homebrew/opt/sdl2/include/SDL2 -I/opt/homebrew/opt/sdl2_ttf/include/SDL2 -I/opt/homebrew/opt/sqlite/include
LDFLAGS = -L/opt/homebrew/opt/sdl2/lib -lSDL2 -L/opt/homebrew/opt/sdl2_ttf/lib -lSDL2_ttf -L/opt/homebrew/opt/sqlite/lib -lsqlite3

# Nombre del ejecutable
TARGET = game

# Archivos fuente
SOURCES = main.c database.c ui.c

# Regla principal
all: $(TARGET)

# Compilar el ejecutable
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

# Compilar y ejecutar
run: $(TARGET)
	./$(TARGET)

# Limpiar archivos compilados
clean:
	rm -f $(TARGET)

.PHONY: all run clean
