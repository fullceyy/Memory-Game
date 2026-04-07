#include "EntityManager.hpp"
#include "Entity/Entity.hpp"
#include <memory>

using namespace Telly;

EntityManager* EntityManager::EntityManager_ = nullptr;

EntityManager* EntityManager::getInstance()
{
    if(!EntityManager_)
        EntityManager_ = new EntityManager();

    return EntityManager_;
}

void EntityManager::loadEntity(std::unique_ptr<Entity> entity)
{
    mEntities.emplace_back(std::move(entity));
}

std::vector<std::unique_ptr<Entity>>& EntityManager::getEntities()
{
    return mEntities;
}
