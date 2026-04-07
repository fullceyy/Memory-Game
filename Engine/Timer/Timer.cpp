#include "Timer.hpp"

Timer::Timer()
: m_StartTicks(0), m_PausedTicks(0), m_Paused(false), m_Started(false)
{
}

void Timer::start()
{
    m_Started = true;
    m_Paused = false;
    m_StartTicks = SDL_GetTicksNS();
    m_PausedTicks = 0;
}

void Timer::stop()
{
    m_Started = false;
    m_Paused = false;
    m_StartTicks = 0;
    m_PausedTicks = 0;
}

void Timer::pause()
{
    if(m_Started && !m_Paused)
    {
        m_Paused = true;

        m_PausedTicks = SDL_GetTicksNS() - m_StartTicks;
        m_StartTicks = 0;
    }
}

void Timer::unpause()
{
    if(m_Started && m_Paused)
    {
        m_Paused = false;
        m_StartTicks = SDL_GetTicksNS() - m_PausedTicks;
        m_PausedTicks = 0;
    }
}

uint64_t Timer::getNS()
{
    uint64_t time = 0;
    if(m_Started)
    {
        if(m_Paused)
        {
            time = m_PausedTicks;
        } else {
            time = SDL_GetTicksNS() - m_StartTicks;
        }
    }
    return time;
}

bool Timer::isPaused()
{
    return m_Paused;
}

bool Timer::isStarted()
{
    return m_Started;
}