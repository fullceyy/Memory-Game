#pragma once
#include "Math/Math.hpp"

namespace Telly
{

/*
 * Should my entity represent the fact that it's just an entity or hold other classes. Answer is no.
 */

class Entity
{
    private:
    Vec2f mPos{};

    public:
    Entity(Vec2f newPos = {0.f, 0.f}) : mPos(newPos) {}
    virtual ~Entity() {}

    Vec2f getPos() const { return mPos; }
};



};
