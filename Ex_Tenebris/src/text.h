/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

#pragma once

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

enum class TextType {
    NORMAL,
    FLASHING
};

class Text 
{
public:
    Text(SDL_Renderer* renderer, const std::string& fontPath, int fontSize, SDL_Color textColor);
    ~Text();

    void RenderConsoleText(const std::string&& text, int x, int y);
    void RenderGameText(const std::string&& text, int x, int y);
    void RenderDebugText(const std::string&& text, int x, int y);

    void RenderFlashingText(const std::string& text, int x, int y, Uint32 currentTime, Uint32 interval);
    void RenderText(const std::string& text, int x, int y, TextType type);
    void RenderPhasingText(const std::string& text, int x, int y, Uint32 currentTime, Uint32 period);

private:
    SDL_Renderer* renderer_;
    TTF_Font* font_;
    SDL_Color textColor_;
    SDL_Color modTextColor_;
};