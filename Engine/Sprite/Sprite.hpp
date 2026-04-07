#pragma once
#include "Math/Math.hpp"
#include "SDL3/SDL_render.h"
#include <string>
#include <unordered_map>

namespace Telly
{

class Sprite
{
    private:
    SDL_Texture* mTexture = nullptr;
    SDL_FRect mRect;
    std::string mAssetPath;

    public:
    Sprite(const std::string& assetPath);
    ~Sprite();

    void render(const Vec4f& data);
    void render();

    void setFRect(const Vec4f& data);
    void setDimensions(float w, float h);
    void setPosition(float x, float y);

    void setAlpha(uint8_t alpha);

    SDL_Texture* getTexture() const { return mTexture; }
};

};
