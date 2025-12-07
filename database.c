#include "database.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

static sqlite3* db = NULL;

// Inicializar la base de datos
bool initDatabase() {
    int rc = sqlite3_open("tetris.db", &db);

    if (rc != SQLITE_OK) {
        printf("Error al abrir base de datos: %s\n", sqlite3_errmsg(db));
        return false;
    }

    // Crear tabla de usuarios si no existe
    const char* sqlUsers =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE NOT NULL,"
        "password TEXT NOT NULL"
        ");";

    char* errMsg = NULL;
    rc = sqlite3_exec(db, sqlUsers, NULL, NULL, &errMsg);

    if (rc != SQLITE_OK) {
        printf("Error al crear tabla users: %s\n", errMsg);
        sqlite3_free(errMsg);
        return false;
    }

    // Crear tabla de puntajes si no existe
    const char* sqlScores =
        "CREATE TABLE IF NOT EXISTS scores ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT NOT NULL,"
        "score INTEGER NOT NULL,"
        "lines INTEGER NOT NULL,"
        "date TEXT NOT NULL,"
        "FOREIGN KEY(username) REFERENCES users(username)"
        ");";

    rc = sqlite3_exec(db, sqlScores, NULL, NULL, &errMsg);

    if (rc != SQLITE_OK) {
        printf("Error al crear tabla scores: %s\n", errMsg);
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

// Cerrar la base de datos
void closeDatabase() {
    if (db != NULL) {
        sqlite3_close(db);
        db = NULL;
    }
}

// Registrar un nuevo usuario
bool registerUser(const char* username, const char* password) {
    if (db == NULL) return false;

    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO users (username, password) VALUES (?, ?);";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Error preparando statement: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        if (rc == SQLITE_CONSTRAINT) {
            printf("El usuario '%s' ya existe.\n", username);
        } else {
            printf("Error al registrar usuario: %s\n", sqlite3_errmsg(db));
        }
        return false;
    }

    return true;
}

// Hacer login de un usuario
bool loginUser(const char* username, const char* password) {
    if (db == NULL) return false;

    sqlite3_stmt* stmt;
    const char* sql = "SELECT password FROM users WHERE username = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Error preparando statement: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        const char* storedPassword = (const char*)sqlite3_column_text(stmt, 0);
        bool match = strcmp(password, storedPassword) == 0;
        sqlite3_finalize(stmt);
        return match;
    }

    sqlite3_finalize(stmt);
    printf("Usuario no encontrado.\n");
    return false;
}

// Guardar un puntaje
bool saveScore(const char* username, int score, int lines) {
    if (db == NULL) return false;

    // Obtener fecha actual
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    char date[20];
    strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm_info);

    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO scores (username, score, lines, date) VALUES (?, ?, ?, ?);";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Error preparando statement: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, score);
    sqlite3_bind_int(stmt, 3, lines);
    sqlite3_bind_text(stmt, 4, date, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        printf("Error al guardar puntaje: %s\n", sqlite3_errmsg(db));
        return false;
    }

    return true;
}

// Obtener los mejores puntajes
int getTopScores(Score* scores, int maxScores) {
    if (db == NULL || scores == NULL) return 0;

    sqlite3_stmt* stmt;
    const char* sql = "SELECT id, username, score, lines, date FROM scores ORDER BY score DESC LIMIT ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Error preparando statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, maxScores);

    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && count < maxScores) {
        scores[count].id = sqlite3_column_int(stmt, 0);
        strncpy(scores[count].username, (const char*)sqlite3_column_text(stmt, 1), 49);
        scores[count].username[49] = '\0';
        scores[count].score = sqlite3_column_int(stmt, 2);
        scores[count].lines = sqlite3_column_int(stmt, 3);
        strncpy(scores[count].date, (const char*)sqlite3_column_text(stmt, 4), 19);
        scores[count].date[19] = '\0';
        count++;
    }

    sqlite3_finalize(stmt);
    return count;
}

// Imprimir los mejores puntajes en consola
void printTopScores() {
    Score scores[10];
    int count = getTopScores(scores, 10);

    printf("\n========== TOP 10 PUNTAJES ==========\n");
    printf("%-4s %-15s %-10s %-8s %s\n", "Pos", "Usuario", "Puntos", "Líneas", "Fecha");
    printf("-----------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-4d %-15s %-10d %-8d %s\n",
               i + 1,
               scores[i].username,
               scores[i].score,
               scores[i].lines,
               scores[i].date);
    }

    if (count == 0) {
        printf("No hay puntajes registrados aún.\n");
    }

    printf("=====================================\n\n");
}
