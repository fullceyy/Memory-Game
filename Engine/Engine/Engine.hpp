#pragma once
#include "SDL3/SDL_render.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "Scene/SceneManager.hpp"
#include "Entity/EntityManager.hpp"
#include "Math/Math.hpp"
/*
 * Layer Manager ->  implies simultaneous rendering of multiple things stacked on top of each other.
 * The mental model is a painter - layer 0 is the background, layer 1 is the game world, layer 2 is the HUD, layer 3 is a particle effect.
 * All layers are alive at the same time, all of them update and render every frame.
 * Order matters for drawing, not for logic ownership.
 *
 * Current situation with Layer Manager ->  returns layer stack via vector, is a singleton, since it's a central layer manager.
 *
 * Scene Manager ->  implies exclusive game states where only one thing is in charge at a time.
 * The mental model is a state machine — you are either in the menu, or in the game, or on the game over screen.
 * When you transition, the old state hands off to the new one.
 * A stack variant lets you push a pause screen on top without destroying the game underneath, but even then the top scene is the active owner.
 *
 *
 * The core difference is ownership and exclusivity.
 * A LayerManager says "all of these run together."
 * A SceneManager says "one of these is in charge, the others are suspended or gone."
 *
 *
 */
using namespace Telly;

class TellyEngine
{
    private:
        TellyEngine() = default;
        ~TellyEngine() = default;
        static TellyEngine* TellyEngine_;

        struct
        {
            SDL_Window* mWindow = nullptr;
            Vec2f mWindowSize {};
        };

        struct
        {
            SDL_Renderer* mRenderer = nullptr;
            TTF_Font* mFont         = nullptr;
            Vec2f mViewport {};
            SDL_Color mClear {};
        };

        // does it even have to be a singleton at this point?
        SceneManager* SMInstance_ = nullptr;
        EntityManager* EMInstance_ = nullptr;

        bool isRunning = false;
    public:
        TellyEngine(TellyEngine&) = delete;
        void operator=(const TellyEngine&) = delete;
        TellyEngine(TellyEngine&&) = delete;
        void operator=(const TellyEngine&&) = delete;

        static TellyEngine* getInstance();

        void setWindowSize(const Vec2f&& vec);
        void setViewport(const Vec2f&& vec);

        void initialize();
        void run();
        void shutdown();

        void windowCloseRequest();

        SDL_Window* PTRgetWindow() const { return mWindow; }
        SDL_Renderer* PTRgetRenderer() const { return mRenderer; }
        TTF_Font* PTRgetFont() const { return mFont; }

        SceneManager* PTRgetSceneManager() const { return SMInstance_; }
};
