#pragma once

#include <SDL.h>
#include <iostream>

class Debug {
public:
    Debug();
    void startFPSCounter();
    void updateFPSCounter();
    float getFPS() const;
    float getMaxFPS() const;

private:
    Uint32 fpsStartTime;
    int fpsFrames;
    int maxFrames;
};