#ifndef UI_H
#define UI_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>

// Colores para la UI
#define UI_COLOR_TEXT_R 255
#define UI_COLOR_TEXT_G 255
#define UI_COLOR_TEXT_B 255

#define UI_COLOR_INPUT_BG_R 40
#define UI_COLOR_INPUT_BG_G 40
#define UI_COLOR_INPUT_BG_B 40

#define UI_COLOR_INPUT_ACTIVE_R 60
#define UI_COLOR_INPUT_ACTIVE_G 60
#define UI_COLOR_INPUT_ACTIVE_B 60

#define UI_COLOR_BUTTON_R 0
#define UI_COLOR_BUTTON_G 120
#define UI_COLOR_BUTTON_B 215

#define UI_COLOR_BUTTON_HOVER_R 30
#define UI_COLOR_BUTTON_HOVER_G 144
#define UI_COLOR_BUTTON_HOVER_B 255

#define MAX_INPUT_LENGTH 49

// Estructuras
typedef struct {
    SDL_Rect rect;
    char text[MAX_INPUT_LENGTH + 1];
    int cursorPos;
    bool isActive;
    bool isPassword;
} TextField;

typedef struct {
    SDL_Rect rect;
    const char* text;
    bool isHovered;
} Button;

// Funciones de inicialización
bool initUI();
void closeUI();

// Funciones de renderizado
void renderText(SDL_Renderer* renderer, const char* text, int x, int y, SDL_Color color);
void renderTextCentered(SDL_Renderer* renderer, const char* text, int x, int y, SDL_Color color);
void renderTextField(SDL_Renderer* renderer, TextField* field);
void renderButton(SDL_Renderer* renderer, Button* button);

// Funciones de input
void handleTextFieldInput(TextField* field, SDL_Event* event);
bool isButtonClicked(Button* button, int mouseX, int mouseY);
void updateButtonHover(Button* button, int mouseX, int mouseY);

// Funciones de creación
TextField createTextField(int x, int y, int width, int height, bool isPassword);
Button createButton(int x, int y, int width, int height, const char* text);

#endif // UI_H
