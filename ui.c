#include "ui.h"
#include <string.h>

static TTF_Font* font = NULL;

bool initUI()
{
    if (TTF_Init() == -1) {
        printf("Error al inicializar SDL_ttf: %s\n", TTF_GetError());
        return false;
    }

    // Intentar cargar una fuente del sistema
    // macOS tiene fuentes en /System/Library/Fonts/
    const char* fontPaths[] = {
        "/System/Library/Fonts/Helvetica.ttc",
        "/System/Library/Fonts/SFNSText.ttf",
        "/Library/Fonts/Arial.ttf",
        NULL
    };

    for (int i = 0; fontPaths[i] != NULL; i++) {
        font = TTF_OpenFont(fontPaths[i], 24);
        if (font != NULL) {
            printf("Fuente cargada: %s\n", fontPaths[i]);
            break;
        }
    }

    if (font == NULL) {
        printf("Error al cargar fuente: %s\n", TTF_GetError());
        printf("Intentando con fuente por defecto...\n");
        return false;
    }

    return true;
}

void closeUI()
{
    if (font != NULL) {
        TTF_CloseFont(font);
        font = NULL;
    }
    TTF_Quit();
}

void renderText(SDL_Renderer* renderer, const char* text, int x, int y, SDL_Color color)
{
    if (font == NULL || text == NULL || strlen(text) == 0) {
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    if (surface == NULL) {
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect destRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &destRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void renderTextCentered(SDL_Renderer* renderer, const char* text, int x, int y, SDL_Color color)
{
    if (font == NULL || text == NULL || strlen(text) == 0) {
        return;
    }

    int textWidth, textHeight;
    TTF_SizeText(font, text, &textWidth, &textHeight);
    renderText(renderer, text, x - textWidth / 2, y - textHeight / 2, color);
}

void renderTextField(SDL_Renderer* renderer, TextField* field)
{
    // Fondo del campo
    if (field->isActive) {
        SDL_SetRenderDrawColor(renderer, UI_COLOR_INPUT_ACTIVE_R, UI_COLOR_INPUT_ACTIVE_G, UI_COLOR_INPUT_ACTIVE_B, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, UI_COLOR_INPUT_BG_R, UI_COLOR_INPUT_BG_G, UI_COLOR_INPUT_BG_B, 255);
    }
    SDL_RenderFillRect(renderer, &field->rect);

    // Borde
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawRect(renderer, &field->rect);

    // Texto
    SDL_Color textColor = {UI_COLOR_TEXT_R, UI_COLOR_TEXT_G, UI_COLOR_TEXT_B, 255};

    if (field->isPassword && strlen(field->text) > 0) {
        // Mostrar asteriscos para password
        char masked[MAX_INPUT_LENGTH + 1];
        int len = strlen(field->text);
        for (int i = 0; i < len && i < MAX_INPUT_LENGTH; i++) {
            masked[i] = '*';
        }
        masked[len] = '\0';
        renderText(renderer, masked, field->rect.x + 10, field->rect.y + 10, textColor);
    } else if (strlen(field->text) > 0) {
        renderText(renderer, field->text, field->rect.x + 10, field->rect.y + 10, textColor);
    }

    // Cursor parpadeante (si está activo)
    if (field->isActive) {
        Uint32 ticks = SDL_GetTicks();
        if ((ticks / 500) % 2 == 0) {  // Parpadear cada 500ms
            int cursorX = field->rect.x + 10;
            if (strlen(field->text) > 0) {
                int textWidth;
                if (field->isPassword) {
                    char masked[MAX_INPUT_LENGTH + 1];
                    int len = strlen(field->text);
                    for (int i = 0; i < len && i < MAX_INPUT_LENGTH; i++) {
                        masked[i] = '*';
                    }
                    masked[len] = '\0';
                    TTF_SizeText(font, masked, &textWidth, NULL);
                } else {
                    TTF_SizeText(font, field->text, &textWidth, NULL);
                }
                cursorX += textWidth;
            }
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawLine(renderer, cursorX, field->rect.y + 8, cursorX, field->rect.y + field->rect.h - 8);
        }
    }
}

void renderButton(SDL_Renderer* renderer, Button* button)
{
    // Fondo del botón
    if (button->isHovered) {
        SDL_SetRenderDrawColor(renderer, UI_COLOR_BUTTON_HOVER_R, UI_COLOR_BUTTON_HOVER_G, UI_COLOR_BUTTON_HOVER_B, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, UI_COLOR_BUTTON_R, UI_COLOR_BUTTON_G, UI_COLOR_BUTTON_B, 255);
    }
    SDL_RenderFillRect(renderer, &button->rect);

    // Borde
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderDrawRect(renderer, &button->rect);

    // Texto centrado
    SDL_Color textColor = {255, 255, 255, 255};
    renderTextCentered(renderer, button->text,
                      button->rect.x + button->rect.w / 2,
                      button->rect.y + button->rect.h / 2,
                      textColor);
}

void handleTextFieldInput(TextField* field, SDL_Event* event)
{
    if (!field->isActive) {
        return;
    }

    if (event->type == SDL_TEXTINPUT) {
        int currentLen = strlen(field->text);
        if (currentLen < MAX_INPUT_LENGTH) {
            strcat(field->text, event->text.text);
        }
    } else if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_BACKSPACE && strlen(field->text) > 0) {
            field->text[strlen(field->text) - 1] = '\0';
        }
    }
}

bool isButtonClicked(Button* button, int mouseX, int mouseY)
{
    return mouseX >= button->rect.x && mouseX <= button->rect.x + button->rect.w &&
           mouseY >= button->rect.y && mouseY <= button->rect.y + button->rect.h;
}

void updateButtonHover(Button* button, int mouseX, int mouseY)
{
    button->isHovered = isButtonClicked(button, mouseX, mouseY);
}

TextField createTextField(int x, int y, int width, int height, bool isPassword)
{
    TextField field;
    field.rect.x = x;
    field.rect.y = y;
    field.rect.w = width;
    field.rect.h = height;
    field.text[0] = '\0';
    field.cursorPos = 0;
    field.isActive = false;
    field.isPassword = isPassword;
    return field;
}

Button createButton(int x, int y, int width, int height, const char* text)
{
    Button button;
    button.rect.x = x;
    button.rect.y = y;
    button.rect.w = width;
    button.rect.h = height;
    button.text = text;
    button.isHovered = false;
    return button;
}
