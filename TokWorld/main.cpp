#include <GLFW/glfw3.h>                      // 引入GLFW库，用于创建窗口、管理上下文和处理输入
#include <imgui.h>                           // 引入ImGui库，用于UI绘制
#include <imgui_impl_glfw.h>                 // ImGui的GLFW绑定，实现与GLFW的集成
#include <imgui_impl_opengl3.h>              // ImGui的OpenGL绑定，实现与OpenGL的集成

#include <chrono>
#include <thread>
#include "GlobalManagers.h"


void setup_imgui_fonts() {
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig config;
    config.MergeMode = true;
    config.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF(R"(c:\Windows\Fonts\msyh.ttc)", 18.0f, nullptr,
        io.Fonts->GetGlyphRangesChineseFull());
}
// 设置ImGui的初始化函数
void setup_imgui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();                    // 检查ImGui版本
    ImGui::CreateContext();                  // 创建ImGui上下文
    ImGui_ImplGlfw_InitForOpenGL(window, true);  // 初始化ImGui与GLFW的绑定
    ImGui_ImplOpenGL3_Init("#version 330");  // 初始化ImGui与OpenGL的绑定，指定GLSL版本
    ImGui::StyleColorsDark();                // 设置ImGui的样式为暗色主题
    setup_imgui_fonts();
}

// 清理ImGui资源的函数
void cleanup_imgui() {
    ImGui_ImplOpenGL3_Shutdown();            // 关闭ImGui的OpenGL绑定
    ImGui_ImplGlfw_Shutdown();               // 关闭ImGui的GLFW绑定
    ImGui::DestroyContext();                 // 销毁ImGui上下文，释放资源
}

// 主函数，程序从这里开始执行
int main() {
    using namespace std::chrono;
    // 初始化GLFW库
    if (!glfwInit()) {
        return -1;
    }

    // 创建一个GLFW窗口，分辨率为1280x720，标题为"Tok"
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Tok", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    setup_imgui(window);

    // 设置初始场景为主菜单
    SceneManager::instance()->switch_to(SceneType::Menu);

    const nanoseconds frame_duration(1000000000 / 60);
    steady_clock::time_point last_tick = steady_clock::now();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        steady_clock::time_point frame_start = steady_clock::now();
        duration<float> real_delta = duration<float>(frame_start - last_tick);
        last_tick = frame_start;

        // 更新游戏时间管理器
        GameTimeManager::instance()->update(real_delta.count());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 更新场景和输入
        SceneManager::instance()->on_input(window);
        SceneManager::instance()->on_update();

        glClear(GL_COLOR_BUFFER_BIT);

        // 绘制场景
        SceneManager::instance()->on_draw();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);

        last_tick = frame_start;
        nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);
        if (sleep_duration > nanoseconds(0)) {
            std::this_thread::sleep_for(sleep_duration);
        }
    }

    cleanup_imgui();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

