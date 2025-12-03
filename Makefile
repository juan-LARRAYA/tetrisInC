# Configuración del compilador
CC = gcc
CFLAGS = -Wall -I/opt/homebrew/opt/sdl2/include
LDFLAGS = -L/opt/homebrew/opt/sdl2/lib -lSDL2

# Nombre del ejecutable
TARGET = game

# Archivos fuente
SOURCES = main.c

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
