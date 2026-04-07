#pragma once
#include "SDL3/SDL_events.h"

namespace Telly
{

class IScene
{
    public:
        virtual ~IScene() = default;

        virtual void update(float dt) = 0;
        virtual void render() = 0;
        virtual void handle_event(SDL_Event&) = 0;
};


};
