# Paso 12: Sistema de Usuarios y Base de Datos

## Objetivo
Implementar un sistema completo de usuarios con login, registro y almacenamiento persistente de puntajes usando SQLite.

## ¿Qué implementamos?

### 1. Base de datos SQLite ([database.h](../database.h) y [database.c](../database.c))

**Creamos dos tablas:**

#### Tabla `users`
```sql
CREATE TABLE users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL,
    password TEXT NOT NULL
);
```

#### Tabla `scores`
```sql
CREATE TABLE scores (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL,
    score INTEGER NOT NULL,
    lines INTEGER NOT NULL,
    date TEXT NOT NULL,
    FOREIGN KEY(username) REFERENCES users(username)
);
```

### 2. Funciones principales de la base de datos

```c
bool initDatabase()                // Inicializa DB y crea tablas
void closeDatabase()               // Cierra la conexión
bool registerUser()                // Registra nuevo usuario
bool loginUser()                   // Verifica credenciales
bool saveScore()                   // Guarda un puntaje
int getTopScores()                 // Obtiene top N puntajes
void printTopScores()              // Imprime top 10 en consola
```

### 3. Menú de login/registro ([main.c:232-299](../main.c#L232-L299))

Al iniciar el juego, el usuario ve un menú con opciones:

```
========== TETRIS EN C ==========
1. Iniciar sesión
2. Registrar nuevo usuario
3. Ver top 10 puntajes
4. Salir
```

**Flujo:**
1. Usuario selecciona opción
2. Si es login (1): ingresa username y password
3. Si es registro (2): crea nuevo usuario
4. Si es ver scores (3): muestra top 10 y vuelve al menú
5. Si login exitoso: empieza el juego

### 4. Guardado automático de puntajes ([main.c:467-470](../main.c#L467-L470))

Cuando el juego termina (Game Over):

```c
if (saveScore(username, score, totalLinesCleared)) {
    printf("¡Puntaje guardado exitosamente!\n");
}
printTopScores();
```

El puntaje se guarda con:
- Username del jugador
- Puntos obtenidos
- Líneas eliminadas
- Fecha y hora actual

### 5. Actualización del Makefile

Agregamos SQLite a la compilación:

```makefile
CFLAGS = -Wall \
  -I/opt/homebrew/opt/sdl2/include \
  -I/opt/homebrew/opt/sqlite/include

LDFLAGS = -L/opt/homebrew/opt/sdl2/lib -lSDL2 \
  -L/opt/homebrew/opt/sqlite/lib -lsqlite3

SOURCES = main.c database.c
```

## Conceptos clave de SQLite

### 1. Prepared Statements (Statements preparados)
```c
sqlite3_stmt* stmt;
const char* sql = "INSERT INTO users (username, password) VALUES (?, ?);";
sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
sqlite3_step(stmt);
sqlite3_finalize(stmt);
```

**Por qué usar prepared statements:**
- Evitan SQL injection
- Son más eficientes
- Separan la lógica SQL de los datos

### 2. Foreign Keys (Llaves foráneas)
```sql
FOREIGN KEY(username) REFERENCES users(username)
```

Esto crea una relación entre las tablas:
- Cada score debe tener un username que existe en users
- Mantiene la integridad referencial

### 3. Manejo de errores
```c
int rc = sqlite3_exec(db, sql, NULL, NULL, &errMsg);
if (rc != SQLITE_OK) {
    printf("Error: %s\n", errMsg);
    sqlite3_free(errMsg);
    return false;
}
```

## Flujo completo del sistema

```
1. Inicio del programa
   ↓
2. initDatabase() → Crea/abre tetris.db
   ↓
3. Menú de login
   ↓
4. Usuario se registra o hace login
   ↓
5. Juega Tetris
   ↓
6. Game Over
   ↓
7. saveScore() → Guarda en DB
   ↓
8. printTopScores() → Muestra top 10
   ↓
9. closeDatabase() → Cierra conexión
```

## Archivos del proyecto

- **database.h**: Declaraciones de funciones y estructuras
- **database.c**: Implementación de todas las funciones de DB
- **main.c**: Juego con integración del sistema de usuarios
- **tetris.db**: Archivo de base de datos SQLite (creado automáticamente)

## Cómo probar el sistema

1. Compilar:
```bash
make clean
make
```

2. Ejecutar:
```bash
./game
```

3. Registrar un usuario nuevo (opción 2)
4. Hacer login (opción 1)
5. Jugar y obtener puntaje
6. Al terminar, verificar que se guardó el puntaje
7. Ver top 10 (opción 3 en el menú)

## Comandos útiles de SQLite

Ver usuarios registrados:
```bash
sqlite3 tetris.db "SELECT * FROM users;"
```

Ver todos los puntajes:
```bash
sqlite3 tetris.db "SELECT * FROM scores ORDER BY score DESC;"
```

Ver top 5:
```bash
sqlite3 tetris.db "SELECT username, score, lines, date FROM scores ORDER BY score DESC LIMIT 5;"
```

Eliminar todos los puntajes:
```bash
sqlite3 tetris.db "DELETE FROM scores;"
```

## Conceptos importantes de C

### 1. Uso de estructuras
```c
typedef struct {
    int id;
    char username[50];
    int score;
    int lines;
    char date[20];
} Score;
```

### 2. Punteros a funciones de SQLite
```c
static sqlite3* db = NULL;  // Variable global estática
```

### 3. Formateo de strings
```c
strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm_info);
```

### 4. Manejo de strings en C
```c
username[strcspn(username, "\n")] = 0;  // Remover newline
strncpy(dest, src, 49);                 // Copiar con límite
dest[49] = '\0';                        // Null terminator
```

## Mejoras futuras

1. **Seguridad:**
   - Hashear contraseñas (bcrypt, argon2)
   - Validar inputs del usuario
   - Prevenir ataques de fuerza bruta

2. **Features:**
   - Filtrar puntajes por usuario
   - Estadísticas (promedio, mejor racha, etc.)
   - Niveles de dificultad
   - Logros/achievements

3. **UI:**
   - Mostrar puntajes en la ventana del juego
   - Animaciones al eliminar líneas
   - Sonidos y música

## Advertencia de seguridad

**Este código almacena contraseñas en texto plano.**

Para un proyecto real necesitas:
```c
// NO HACER:
password = "micontraseña123"

// HACER:
password_hash = hash_bcrypt("micontraseña123")
```

## Resumen

Has aprendido:
- ✅ Integrar SQLite en C
- ✅ Crear y gestionar tablas
- ✅ Usar prepared statements
- ✅ Relaciones entre tablas (foreign keys)
- ✅ Manejo de errores en SQLite
- ✅ Formateo de fechas en C
- ✅ Input/output de consola
- ✅ Persistencia de datos

El juego ahora es completo y funcional con un sistema de usuarios robusto!
