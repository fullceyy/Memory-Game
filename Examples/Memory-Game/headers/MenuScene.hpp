#pragma once
#include "Math/Math.hpp"
#include "Scene/IScene.hpp"
#include "SDL3/SDL_render.h"
#include <vector>
#include <string>

class MenuScene : public Telly::IScene
{
    private:
    enum class MenuPhase { SPLASH, FADE_IN, ACTIVE };
    MenuPhase mPhase     = MenuPhase::SPLASH;
    float     mFadeAlpha  = 0.f;
    float     mSplashAlpha = 255.f;
    static constexpr float kFadeSpeed    = 120.f;
    static constexpr float kFadeOutSpeed = 150.f;

    struct MenuOption
    {
        SDL_Texture* mTexture    = nullptr;
        std::string  mText;
        bool         mHovered    = false;
        Vec2f        mDimensions {};
        Vec2f        mPos        {};

        bool contains(float px, float py) const
        {
            return px >= mPos.x && px <= mPos.x + mDimensions.x &&
                    py >= mPos.y && py <= mPos.y + mDimensions.y;
        }
    };
    std::vector<MenuOption> mOptions;
    bool active = false;

    struct SplashLetter
    {
        SDL_Texture* mTexture  = nullptr;
        float        mWidth    = 0.f;
        float        mHeight   = 0.f;
        float        mCurrentX = 0.f;
        float        mTargetX  = 0.f;
        float        mBaseY    = 0.f;
        bool         mArrived  = false;
    };

    SDL_Color mWhite  = { .r = 255, .g = 255, .b = 255, .a = SDL_ALPHA_OPAQUE };
    SDL_Color mYellow = { .r = 255, .g = 220, .b = 50,  .a = SDL_ALPHA_OPAQUE };

    std::vector<SplashLetter> mSplashLetters;
    float                     mSplashTimer    = 0.f;
    int                       mLettersVisible = 0;
    static constexpr float    kSlideSpeed     = 600.f;
    static constexpr float    kLetterDelay    = 0.07f;
    static constexpr float    kFloatAmp       = 4.f;
    static constexpr float    kFloatSpeed     = 2.5f;

    public:
    MenuScene();
    ~MenuScene();

    void update(float dt) override;
    void render() override;
    void handle_event(SDL_Event& event) override;
};
