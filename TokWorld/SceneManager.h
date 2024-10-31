#pragma once
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <iostream>
class Scene {
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void on_enter() {}
    virtual void on_update() {}
    virtual void on_draw() {}
    virtual void on_input(GLFWwindow* window) {}
    virtual void on_exit() {}
};

class MenuScene : public Scene {
public:
    MenuScene() = default;
    ~MenuScene() = default;

    void on_enter() override {
        std::cout << "进入主菜单" << std::endl;
    }

    void on_update() override {
        std::cout << "主菜单正在运行..." << std::endl;
    }
    void on_draw();

    void on_input(GLFWwindow* window) override {

    }

    void on_exit() override {
        std::cout << "主菜单退出" << std::endl;
    }
};

class GameScene : public Scene {
private:

public:
    GameScene() {
    }

    ~GameScene() override = default;

    void on_enter() override {
        std::cout << "进入游戏场景" << std::endl;
        GameTimeManager::instance()->start();
        MapManager* map_manager = MapManager::instance();
        map_manager->load_map(1, "MapA", "testmap.tmj", 1.5f);



    }

    void on_update() override {
    }

    void on_draw() override {
        std::cout << "当前游戏时间: " << GameTimeManager::instance()->get_current_date_time() << std::endl;
		std::cout << "绘制当前地图：" << std::endl;
        MapManager::instance()->draw_map(1);



        }
    

    void on_input(GLFWwindow* window) override {

        }
    

    void on_exit() override {
        std::cout << "退出游戏场景" << std::endl;
        GameTimeManager::instance()->stop();
    }



};


enum class SceneType {
    Menu,
    Game,
    Achievements
};

class SceneManager {
public:
    static SceneManager* instance() {
        if (!manager) {
            manager = new SceneManager();
            manager->initialize_scenes();
        }
        return manager;
    }

    void switch_to(SceneType new_scene) {
        if (current_scene) {
            current_scene->on_exit(); // 退出当前场景
        }

        switch (new_scene) {
        case SceneType::Menu:
            if (!menu_scene) {
                menu_scene = std::make_unique<MenuScene>();
            }
            current_scene = menu_scene.get();
            break;
        case SceneType::Game:
            if (!game_scene) {
                game_scene = std::make_unique<GameScene>();
            }
            current_scene = game_scene.get();
            break;
            // 你可以在这里添加更多场景
        }

        if (current_scene) {
            current_scene->on_enter(); // 进入新场景
        }
    }
        void on_update() {
            if (current_scene) {
                current_scene->on_update();
            }
        }

        void on_draw() {
            if (current_scene) {
                current_scene->on_draw();
            }
        }

        void on_input(GLFWwindow * window) {
            if (current_scene) {
                current_scene->on_input(window);
            }
        }
    

    void initialize_scenes() {
        // 初始化主菜单场景
        menu_scene = std::make_unique<MenuScene>();
    }

private:
    SceneManager() = default;
    ~SceneManager() = default;

    static SceneManager* manager;
    std::unique_ptr<MenuScene> menu_scene;
    std::unique_ptr<GameScene> game_scene;
    Scene* current_scene = nullptr;
};

void MenuScene::on_draw()
{
        // 设置窗口的大小和位置
        ImGui::SetNextWindowSize(ImVec2(1200, 720));  // 设置窗口大小为 300x200
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once); // 设置窗口位置，确保首次渲染到指定位置

        // 开始绘制主菜单窗口
        ImGui::Begin("主菜单");

        // 添加“进入游戏”按钮
        ImGui::SetCursorPos(ImVec2(300, 480)); // 设置第一个按钮的相对位置 (相对于窗口左上角)
        if (ImGui::Button("进入游戏", ImVec2(150, 50))) {  // 设定按钮大小为 150x50
            // 当按钮被按下时，切换到游戏场景
            std::cout << "进入游戏场景按钮被点击" << std::endl;
            SceneManager::instance()->switch_to(SceneType::Game);
        }
        ImGui::SetCursorPos(ImVec2(600, 480)); // 设置下一个按钮的位置
        if (ImGui::Button("退出游戏", ImVec2(150, 50))) {  // 添加一个退出游戏按钮
            std::cout << "退出游戏按钮被点击" << std::endl;
            glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE); // 退出游戏
        }

        // 结束窗口
        ImGui::End();

}
