#ifndef A_IMGUI_H // !A_IMGUI_H
#define A_IMGUI_H

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_android.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <android/keycodes.h>

#include <memory>
#include <vector>
#include <string>

namespace android
{
    class AImGui
    {
    public:
        struct Options
        {
            bool autoUpdateOrientation = false;
        };

    public:
        AImGui() : AImGui(Options{})
        {
        }
        AImGui(const Options &options);
        ~AImGui();

        void BeginFrame();
        void EndFrame();

        void ProcessInputEvent();

        void SetupWindowInfo(void *windowInfo);

        constexpr operator bool() const
        {
            return m_state;
        }

    private:
        bool InitEnvironment();
        void UnInitEnvironment();

    private:
        bool m_state = false;

        int m_rotateTheta = 0;
        int m_screenWidth = -1, m_screenHeight = -1;
        double m_lastTime = 0.0;

        Options m_options;

        ANativeWindow *m_nativeWindow = nullptr;
        EGLDisplay m_defaultDisplay = EGL_NO_DISPLAY;
        EGLSurface m_eglSurface = EGL_NO_SURFACE;
        EGLContext m_eglContext = EGL_NO_CONTEXT;
        ImGuiContext *m_imguiContext = nullptr;
    };
} // namespace android

#endif // !A_IMGUI_H