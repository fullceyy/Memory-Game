#include "SceneManager.hpp"
#include "Scene/IScene.hpp"
#include <memory>

using namespace Telly;

SceneManager* SceneManager::SceneManager_ = nullptr;

SceneManager* SceneManager::getInstance()
{
    if(!SceneManager_)
        SceneManager_ = new SceneManager();

    return SceneManager_;
}

void SceneManager::loadScene(std::unique_ptr<IScene> scene)
{
    mScenes.emplace_back(std::move(scene));
}

std::vector<std::unique_ptr<IScene>>& SceneManager::getScenes()
{
    return mScenes;
}
