#include "debug.h"
#include <iostream>

Debug::Debug() : fpsStartTime(0), fpsFrames(0) {}

void Debug::startFPSCounter() 
{
    fpsStartTime = SDL_GetTicks();
}

void Debug::updateFPSCounter() 
{
    fpsFrames++;
    Uint32 currentTime = SDL_GetTicks();
    Uint32 elapsedTime = currentTime - fpsStartTime;
    if (elapsedTime >= 1000) {
        float fps = fpsFrames / (elapsedTime / 1000.0f);
        maxFrames = std::max(maxFrames, static_cast<int>(fps));
        fpsStartTime = currentTime;
        fpsFrames = 0;
    }
}

float Debug::getFPS() const 
{
    return fpsFrames;
}

float Debug::getMaxFPS() const 
{
    return maxFrames;
}