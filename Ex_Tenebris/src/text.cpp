/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

#include "text.h"

Text::Text(SDL_Renderer* renderer, const std::string& fontPath, int fontSize, SDL_Color textColor) 
{
    if (TTF_Init() == -1) 
    {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
    }

    font_ = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font_) 
    {
        std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << std::endl;
    }

    renderer_ = renderer;
    textColor_ = textColor;
    modTextColor_ = textColor;
}

Text::~Text() 
{
    TTF_CloseFont(font_);
    TTF_Quit();
}

void Text::RenderConsoleText(const std::string&& text, int x, int y)
{
    int xOffset = 0;
    for (char ch : text) 
    {
        SDL_Surface* surface = TTF_RenderGlyph_Solid(font_, ch, textColor_);
        if (!surface) 
        {
            std::cerr << "TTF_RenderGlyph_Solid failed: " << TTF_GetError() << std::endl;
            continue;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
        if (!texture) 
        {
            std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            continue;
        }

        SDL_Rect destRect = { x + xOffset, y, surface->w, surface->h };
        SDL_RenderCopy(renderer_, texture, NULL, &destRect);

        xOffset += surface->w;

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}


void Text::RenderGameText(const std::string&& text, int x, int y)
{
    int xOffset = 0;
    for (char ch : text)
    {
        SDL_Surface* surface = TTF_RenderGlyph_Solid(font_, ch, textColor_);
        if (!surface)
        {
            std::cerr << "TTF_RenderGlyph_Solid failed: " << TTF_GetError() << std::endl;
            continue;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
        if (!texture)
        {
            std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            continue;
        }

        SDL_Rect destRect = { x + xOffset, y, surface->w, surface->h };
        SDL_RenderCopy(renderer_, texture, NULL, &destRect);

        xOffset += surface->w;

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}

void Text::RenderDebugText(const std::string&& text, int x, int y)
{
    int xOffset = 0;
    for (char ch : text)
    {
        SDL_Surface* surface = TTF_RenderGlyph_Solid(font_, ch, textColor_);
        if (!surface)
        {
            std::cerr << "TTF_RenderGlyph_Solid failed: " << TTF_GetError() << std::endl;
            continue;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
        if (!texture)
        {
            std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            continue;
        }

        SDL_Rect destRect = { x + xOffset, y, surface->w, surface->h };
        SDL_RenderCopy(renderer_, texture, NULL, &destRect);

        xOffset += surface->w;

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}

void Text::RenderText(const std::string& text, int x, int y, TextType type)
{
    int xOffset = 0;
    for (char ch : text)
    {
        SDL_Surface* surface = TTF_RenderGlyph_Solid(font_, ch, textColor_);
        if (!surface)
        {
            std::cerr << "TTF_RenderGlyph_Solid failed: " << TTF_GetError() << std::endl;
            continue;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
        if (!texture)
        {
            std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            continue;
        }

        SDL_Rect destRect = { x + xOffset, y, surface->w, surface->h };
        SDL_RenderCopy(renderer_, texture, NULL, &destRect);

        xOffset += surface->w;

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}

void Text::RenderFlashingText(const std::string& text, int x, int y, Uint32 currentTime, Uint32 interval)
{
    if ((currentTime / interval) % 2 == 0) // RJP - Toggle every 'interval' milliseconds
    {
        RenderText(text, x, y, TextType::NORMAL);
    }
}

/* 
void Text::RenderPhasingText(const std::string& text, int x, int y, Uint32 currentTime, Uint32 period)
{
    // Calculate alpha based on sine wave
    float alpha = sin(currentTime * 2 * M_PI / period); // M_PI is the constant for PI

    // Map the sine wave from [-1, 1] to [0, 255] for alpha blending
    Uint8 alphaValue = static_cast<Uint8>((alpha + 1.0) * 127.5);

    // Set alpha value for the text color
    SDL_Color originalColor = textColor_;
    textColor_.a = alphaValue;

    // Render the text with modified alpha
    RenderText(text, x, y, TextType::NORMAL);

    // Reset alpha back to original value for subsequent rendering
    textColor_ = originalColor;
}
*/

void Text::RenderPhasingText(const std::string& text, int x, int y, Uint32 currentTime, Uint32 period)
{
    Uint8 minAlpha = 1;

    // Calculate alpha based on sine wave
    float alpha = sin(currentTime * 2 * M_PI / period); // M_PI is the constant for PI

    // Map the sine wave from [-1, 1] to [0, 255] for alpha blending
    Uint8 alphaValue = static_cast<Uint8>((alpha + 1.0) * 127.5);

    // Ensure alpha doesn't fall below the minimum threshold
    if (alphaValue < minAlpha)
        return;

    // Set alpha value for the text color
    SDL_Color originalColor = textColor_;
    textColor_.a = alphaValue;

    // Render the text with modified alpha
    RenderText(text, x, y, TextType::NORMAL);

    // Reset alpha back to original value for subsequent rendering
    textColor_ = originalColor;
}
