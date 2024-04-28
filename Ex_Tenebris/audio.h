/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

#pragma once

#include <SDL.h>
#include <SDL_mixer.h>

class AudioPlayer 
{
public:
    AudioPlayer();
    ~AudioPlayer();

    void play(const char* filePath);
    void pause();
    void resume();
    void stop();
    void fadeOut(int milliseconds);
    void fadeIn(const char* filePath, int milliseconds);

private:
    Mix_Music* music;
};