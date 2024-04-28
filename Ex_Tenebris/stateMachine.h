/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

#pragma once

enum class GameState 
{
    MENU,
    PLAY,
    PAUSE,
    GAMEOVER,
    DEADEND,
    CONTINUE,
    INTRO,
    ESCAPE,
};

enum class TransitionState {
    NONE,
    FADE_IN,
    FADE_OUT,
    SLIDE_LEFT,
    SLIDE_RIGHT,
    CUSTOM // You can define your custom transition states
};

class StateMachine 
{
public:
    StateMachine() : gsCurrentState(GameState::MENU) {}

    inline void setState(GameState newState) { gsCurrentState = newState; }
    inline GameState getState() const { return gsCurrentState; }

private:
    GameState gsCurrentState;
};

class TransitionStateMachine {
public:
    TransitionStateMachine() : tsCurrentTransition(TransitionState::NONE) {}

    inline void setTransition(TransitionState newTransition) { tsCurrentTransition = newTransition; }
    inline TransitionState getTransition() const { return tsCurrentTransition; }

private:
    TransitionState tsCurrentTransition;
};