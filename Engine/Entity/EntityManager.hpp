#pragma once
#include <memory>
#include <vector>
#include "Entity.hpp"

namespace Telly
{

class EntityManager
{
    private:
    static EntityManager* EntityManager_;
    std::vector<std::unique_ptr<Entity>> mEntities;
    EntityManager() = default;
    ~EntityManager() = default;

    public:
    EntityManager(EntityManager&) = delete;
    void operator=(const EntityManager&) = delete;
    EntityManager(EntityManager&&) = delete;
    void operator=(const EntityManager&&) = delete;

    static EntityManager* getInstance();

    void loadEntity(std::unique_ptr<Entity> entity);
    std::vector<std::unique_ptr<Entity>>& getEntities();
};


};
