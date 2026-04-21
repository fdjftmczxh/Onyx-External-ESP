// 作者: feng_xingzhe_cn_.86

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <atomic>
#include <exception>

#include "AImGui.h"
#include "KittyMemoryMgr.hpp"
#include <onyx/utils/Memory.hpp>
#include <onyx/core/esp_manager.hpp>
#include <onyx/sdk/game/CameraSystem.hpp>
#include <onyx/render/renderer.hpp>

namespace {

    onyx::core::EspManager            g_esp_manager;
    onyx::sdk::game::CameraSystem     g_camera;
    onyx::render::Renderer            g_renderer;

    KittyMemoryMgr g_ktmgr;

    std::atomic<uintptr_t> s_il2cpp_base{0};
    std::atomic<bool>      s_is_ready{false};
    std::atomic<bool>      s_show_menu{true};
    std::atomic<bool>      s_running{true};

    const std::string k_package_name = "com.criticalforceentertainment.criticalops";

    void gui_loop() {

        try {

            android::AImGui imgui;

            if (!imgui) {

                std::cerr << "[-] AImGui initialization failed (Unknown error)" << std::endl;

                return;

            }

            g_renderer.initialize();

            std::cout << "[+] Rendering and Input loops starting on main thread." << std::endl;

            while (s_running) {

                imgui.ProcessInputEvent();

                imgui.BeginFrame();

                if (s_is_ready) {

                    g_renderer.render_frame(g_esp_manager, g_camera, s_il2cpp_base.load());

                } else {

                    ImGui::SetNextWindowPos({20, 20}, ImGuiCond_FirstUseEver);
                    ImGui::SetNextWindowBgAlpha(0.0f); // Make it almost transparent for a cleaner look if desired

                    if (ImGui::Begin("##SearchStatus", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings)) {

                        ImGui::TextColored({1.0f, 1.0f, 1.0f, 0.6f}, "Searching for game process...");

                    }

                    ImGui::End();

                }

                imgui.EndFrame();

                std::this_thread::sleep_for(std::chrono::milliseconds(1));

            }

        } catch (const std::exception& e) {

            std::cerr << "[-] AImGui Critical Error: " << e.what() << std::endl;

        }

    }

    void logic_worker() {

        while (s_running) {

            if (s_is_ready) {

                g_esp_manager.update_loop(s_il2cpp_base.load(), g_camera);

            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

        }

    }

    void discovery_worker() {

        while (s_running) {

            if (!s_is_ready) {

                pid_t pid = KittyMemoryEx::getProcessID(k_package_name);

                if (pid > 0) {

                    std::cout << "[+] Found process " << k_package_name << " with PID: " << pid << std::endl;

                    if (g_ktmgr.initialize(pid, EK_MEM_OP_SYSCALL, true)) {

                        onyx::utils::Memory::s_mgr = &g_ktmgr;

                        auto il2cpp_elf = g_ktmgr.elfScanner.findElf("libil2cpp.so");

                        if (il2cpp_elf.isValid()) {

                            onyx::utils::Memory::s_base_address = il2cpp_elf.base();

                            s_il2cpp_base.store(il2cpp_elf.base());
                            s_is_ready = true;

                        }

                    }

                }

            } else {

                if (KittyMemoryEx::getProcessID(k_package_name) == 0) {

                    std::cout << "[-] Game process terminated. Returning to search mode." << std::endl;

                    s_is_ready = false;
                    s_il2cpp_base.store(0);

                }

            }

            std::this_thread::sleep_for(std::chrono::seconds(2));

        }

    }

}

int main() {

    std::cout << "[+] Onyx External starting..." << std::endl;

    std::thread discovery_thread(discovery_worker);
    std::thread logic_thread(logic_worker);

    gui_loop();

    discovery_thread.join();
    logic_thread.join();

    return 0;
}
