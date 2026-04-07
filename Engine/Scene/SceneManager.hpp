#pragma once
#include "IScene.hpp"
#include <vector>
#include <memory>

namespace Telly
{

class SceneManager
{
    private:
        static SceneManager* SceneManager_;
        std::vector<std::unique_ptr<IScene>> mScenes;
        SceneManager() = default;
        ~SceneManager() = default;
    public:
        SceneManager(SceneManager&) = delete;
        void operator=(const SceneManager&) = delete;
        SceneManager(SceneManager&&) = delete;
        void operator=(const SceneManager&&) = delete;

        static SceneManager* getInstance();

        void loadScene(std::unique_ptr<IScene> scene);
        std::vector<std::unique_ptr<IScene>>& getScenes();
};


};
