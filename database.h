#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <stdbool.h>

// Estructura para representar un puntaje
typedef struct {
    int id;
    char username[50];
    int score;
    int lines;
    char date[20];
} Score;

// Funciones de base de datos
bool initDatabase();
void closeDatabase();
bool saveScore(const char* username, int score, int lines);
int getTopScores(Score* scores, int maxScores);
void printTopScores();

#endif
