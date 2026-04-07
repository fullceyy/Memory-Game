#pragma once
#include "SDL3/SDL.h"
#include <cstdint>

class Timer
{
    private:
        uint64_t m_StartTicks;
        uint64_t m_PausedTicks;
        
        bool m_Paused;
        bool m_Started;
    public:
        Timer();
        ~Timer() = default;

        void start();
        void stop();
        void pause();
        void unpause();

        uint64_t getNS();
        bool isStarted();
        bool isPaused();
};