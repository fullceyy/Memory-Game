#include "MenuScene.hpp"
#include "Engine/Engine.hpp"
#include <cmath>

MenuScene::MenuScene()
: active(true)
{
    TellyEngine* Engine_ = TellyEngine::getInstance();
    int width, height;
    SDL_GetCurrentRenderOutputSize(Engine_->PTRgetRenderer(), &width, &height);
    SDL_SetRenderDrawBlendMode(Engine_->PTRgetRenderer(), SDL_BLENDMODE_BLEND);

    // ── Splash letters ────────────────────────────────────────────
    const char* splashText = "Press SPACE to continue...";
    float totalW = 0.f;
    float maxH   = 0.f;

    for(int i = 0; splashText[i] != '\0'; i++)
    {
        char buf[2] = { splashText[i], '\0' };
        SDL_Surface* surface;
        SDL_Texture* texture;

        if(i >= 6 && i <= 10)
        {
            surface = TTF_RenderText_Blended(Engine_->PTRgetFont(), buf, 0, mYellow);
            texture = SDL_CreateTextureFromSurface(Engine_->PTRgetRenderer(), surface);
        }
        else
        {
            surface = TTF_RenderText_Blended(Engine_->PTRgetFont(), buf, 0, mWhite);
            texture = SDL_CreateTextureFromSurface(Engine_->PTRgetRenderer(), surface);
        }

        SDL_DestroySurface(surface);

        float tw, th;
        SDL_GetTextureSize(texture, &tw, &th);
        totalW += tw;
        if(th > maxH) maxH = th;

        mSplashLetters.push_back({ texture, tw, th });
    }

    float baseY   = height - maxH - 30.f;
    float startX  = (width - totalW) / 2.f;
    float cursorX = startX;

    for(auto& letter : mSplashLetters)
    {
        letter.mTargetX  = cursorX;
        letter.mCurrentX = -letter.mWidth;
        letter.mBaseY    = baseY;
        letter.mArrived  = false;
        cursorX += letter.mWidth;
    }

    // ── Menu options ──────────────────────────────────────────────
    const int padding = 35;

    mOptions.push_back({ .mText = "Play" });
    mOptions.push_back({ .mText = "Exit" });

    for(size_t q = 0; q < 2; q++)
    {
        SDL_Surface* surface = TTF_RenderText_Blended(Engine_->PTRgetFont(),
            mOptions[q].mText.c_str(), 0,
            { .r = 255, .g = 255, .b = 255, .a = SDL_ALPHA_OPAQUE });

        mOptions[q].mTexture = SDL_CreateTextureFromSurface(Engine_->PTRgetRenderer(), surface);
        SDL_DestroySurface(surface);

        SDL_GetTextureSize(mOptions[q].mTexture, &mOptions[q].mDimensions.x, &mOptions[q].mDimensions.y);
    }

    float total_height = 0.f;
    for(auto& o : mOptions)
        total_height += o.mDimensions.y + padding;
    total_height -= padding;

    float cursorY = (height - total_height) / 2.f;
    for(auto& o : mOptions)
    {
        o.mPos.x = (width - o.mDimensions.x) / 2.f;
        o.mPos.y = cursorY;
        cursorY += o.mDimensions.y + padding;
    }

}

MenuScene::~MenuScene()
{
    for(auto& letter : mSplashLetters)
        if(letter.mTexture)
            SDL_DestroyTexture(letter.mTexture);

    for(auto& opt : mOptions)
        if(opt.mTexture)
            SDL_DestroyTexture(opt.mTexture);
}

void MenuScene::handle_event(SDL_Event& e)
{
    if(e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_SPACE)
        if(mPhase == MenuPhase::SPLASH)
            mPhase = MenuPhase::FADE_IN;

    if(e.type == SDL_EVENT_MOUSE_MOTION)
        for(auto& option : mOptions)
            option.mHovered = option.contains(e.motion.x, e.motion.y);
}

void MenuScene::update(float dt)
{
    mSplashTimer += dt;

    int shouldBeVisible = static_cast<int>(mSplashTimer / kLetterDelay);
    mLettersVisible     = std::min(shouldBeVisible, (int)mSplashLetters.size());

    for(int i = 0; i < mLettersVisible; i++)
    {
        auto& letter = mSplashLetters[i];
        if(!letter.mArrived)
        {
            letter.mCurrentX += kSlideSpeed * dt;
            if(letter.mCurrentX >= letter.mTargetX)
            {
                letter.mCurrentX = letter.mTargetX;
                letter.mArrived  = true;
            }
        }
    }

    if(mPhase == MenuPhase::FADE_IN || mPhase == MenuPhase::ACTIVE)
    {
        mSplashAlpha -= kFadeOutSpeed * dt;
        if(mSplashAlpha < 0.f) mSplashAlpha = 0.f;
    }

    if(mPhase == MenuPhase::FADE_IN)
    {
        mFadeAlpha += kFadeSpeed * dt;
        if(mFadeAlpha >= 255.f)
        {
            mFadeAlpha = 255.f;
            mPhase     = MenuPhase::ACTIVE;
        }
    }
}

void MenuScene::render()
{
    TellyEngine* Engine_ = TellyEngine::getInstance();
    SDL_SetRenderDrawColor(Engine_->PTRgetRenderer(), 10, 10, 20, 255);
    SDL_RenderClear(Engine_->PTRgetRenderer());

    Uint8 optAlpha    = static_cast<Uint8>(mFadeAlpha);
    Uint8 splashAlpha = static_cast<Uint8>(mSplashAlpha);

    // ── Menu options ──────────────────────────────────────────────
    for(auto& o : mOptions)
    {
        SDL_SetTextureAlphaMod(o.mTexture, optAlpha);
        SDL_SetTextureColorMod(o.mTexture,
            o.mHovered ? 255 : 180,
            o.mHovered ? 220 : 180,
            o.mHovered ? 50  : 180
        );

        SDL_FRect dst { o.mPos.x, o.mPos.y, o.mDimensions.x, o.mDimensions.y };
        SDL_RenderTexture(Engine_->PTRgetRenderer(), o.mTexture, nullptr, &dst);
    }

    // ── Splash letters ────────────────────────────────────────────
    float time = SDL_GetTicks() / 1000.f;

    for(int i = 0; i < mLettersVisible; i++)
    {
        auto& letter = mSplashLetters[i];

        float floatY = letter.mBaseY;
        if(letter.mArrived)
            floatY += std::sin(time * kFloatSpeed + i * 0.35f) * kFloatAmp;

        SDL_FRect dst { letter.mCurrentX, floatY, letter.mWidth, letter.mHeight };
        SDL_SetTextureAlphaMod(letter.mTexture, splashAlpha);
        SDL_RenderTexture(Engine_->PTRgetRenderer(), letter.mTexture, nullptr, &dst);
    }
}
