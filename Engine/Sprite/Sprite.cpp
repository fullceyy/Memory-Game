#include "Sprite.hpp"
#include "Engine/Engine.hpp"
#include "Math/Math.hpp"
#include "SDL3/SDL_render.h"
#include "SDL3_image/SDL_image.h"

using namespace Telly;

Sprite::Sprite(const std::string& assetPath)
: mAssetPath(assetPath)
{
    // TODO: FIX THE FUCKING PATHING IN THIS ENGINE.
    std::string pathcfg = "../../Engine/Assets/";
    mTexture = IMG_LoadTexture(TellyEngine::getInstance()->PTRgetRenderer(), (pathcfg + mAssetPath).c_str());
}

Sprite::~Sprite()
{
    if(mTexture)
        SDL_DestroyTexture(mTexture);
}

void Sprite::setFRect(const Vec4f& data)
{
    mRect = { .x = data.x, .y = data.y, .w = data.z, .h = data.w };
}

void Sprite::setDimensions(float w, float h)
{
    mRect.w = w;
    mRect.h = h;
}

void Sprite::setPosition(float x, float y)
{
    mRect.x = x;
    mRect.y = y;
}

void Sprite::render(const Vec4f& data)
{
    SDL_FRect dst { .x = data.x, .y = data.y, .w = data.z, .h = data.w };
    SDL_RenderTexture(TellyEngine::getInstance()->PTRgetRenderer(), mTexture, nullptr, &dst);
}

void Sprite::render()
{
    SDL_RenderTexture(TellyEngine::getInstance()->PTRgetRenderer(), mTexture, nullptr, &mRect);
}

void Sprite::setAlpha(uint8_t alpha)
{
    SDL_SetTextureAlphaMod(mTexture, alpha);
}
