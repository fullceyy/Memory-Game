#include "Engine.hpp"
#include "SDL3/SDL_render.h"
#include <format>
#include <memory>

TellyEngine* TellyEngine::TellyEngine_ = nullptr;

TellyEngine* TellyEngine::getInstance()
{
    if(!TellyEngine_)
        TellyEngine_ = new TellyEngine();

    return TellyEngine_;
}

void TellyEngine::setWindowSize(const Vec2f&& vec)
{
    mWindowSize = vec;
}

void TellyEngine::setViewport(const Vec2f&& vec)
{
    mViewport = vec;
}

void TellyEngine::initialize()
{
    if(mWindow = SDL_CreateWindow("Telly", mWindowSize.x, mWindowSize.y, 0); !mWindow)
        throw std::runtime_error(std::format("Failed to create SDLWindow! {}", SDL_GetError()));
    if(mRenderer = SDL_CreateRenderer(mWindow, 0); !mRenderer)
        throw std::runtime_error(std::format("Failed to create SDLRenderer! {}", SDL_GetError()));

    SDL_SetRenderLogicalPresentation(mRenderer, mViewport.x, mViewport.y, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    if(!TTF_Init())
        throw std::runtime_error(std::format("Failed to initialize TTF!"));

    // TODO:
    // Resolve this hardcoded absolute path somehow.
    std::string fontPath = std::string("../../Engine/Assets/Fonts/PressStart2P-Regular.ttf");
    if(mFont = TTF_OpenFont(fontPath.c_str(), 16); !mFont)
        SDL_Log("%s", fontPath.c_str());
        // throw std::runtime_error(std::format("Failed to open TTF_Font!"));

    SMInstance_ = SceneManager::getInstance();

    mClear = { .r = 0, .g = 0, .b = 0, .a = SDL_ALPHA_OPAQUE };
    isRunning = true;
}

void TellyEngine::run()
{
    const auto perfFrequency = SDL_GetPerformanceFrequency();
    auto lastFrameTime = SDL_GetPerformanceCounter();
    SDL_Event e;
    auto& vector = SMInstance_->getScenes();
    while(isRunning)
    {
        auto frameStartTime = SDL_GetPerformanceCounter();
        double deltaTime    = static_cast<double>(frameStartTime - lastFrameTime) / perfFrequency;
        lastFrameTime       = frameStartTime;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                isRunning = false;

            for(auto& scene : vector)
            {
                scene->handle_event(e);
            }
        }

        for(auto& scene : vector)
        {
            scene->update(deltaTime);
            scene->render();
        }

        SDL_RenderPresent(mRenderer);
    }
}

void TellyEngine::windowCloseRequest()
{
    isRunning = false;
}
