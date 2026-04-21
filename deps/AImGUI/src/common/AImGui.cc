#include "AImGui.h"
#include "Global.h"
#include "ANativeWindowCreator.h"
#include "ATouchEvent.h"

size_t android::anative_window_creator::detail::compat::SystemVersion = 0;

namespace android
{
    static ImGuiKey KeyCodeToImGuiKey(int32_t keyCode)
    {
        switch (keyCode)
        {
        case AKEYCODE_TAB: return ImGuiKey_Tab;
        case AKEYCODE_DPAD_LEFT: return ImGuiKey_LeftArrow;
        case AKEYCODE_DPAD_RIGHT: return ImGuiKey_RightArrow;
        case AKEYCODE_DPAD_UP: return ImGuiKey_UpArrow;
        case AKEYCODE_DPAD_DOWN: return ImGuiKey_DownArrow;
        case AKEYCODE_PAGE_UP: return ImGuiKey_PageUp;
        case AKEYCODE_PAGE_DOWN: return ImGuiKey_PageDown;
        case AKEYCODE_MOVE_HOME: return ImGuiKey_Home;
        case AKEYCODE_MOVE_END: return ImGuiKey_End;
        case AKEYCODE_INSERT: return ImGuiKey_Insert;
        case AKEYCODE_FORWARD_DEL: return ImGuiKey_Delete;
        case AKEYCODE_DEL: return ImGuiKey_Backspace;
        case AKEYCODE_APOSTROPHE: return ImGuiKey_Apostrophe;
        case AKEYCODE_COMMA: return ImGuiKey_Comma;
        case AKEYCODE_MINUS: return ImGuiKey_Minus;
        case AKEYCODE_PERIOD: return ImGuiKey_Period;
        case AKEYCODE_SLASH: return ImGuiKey_Slash;
        case AKEYCODE_SEMICOLON: return ImGuiKey_Semicolon;
        case AKEYCODE_EQUALS: return ImGuiKey_Equal;
        case AKEYCODE_LEFT_BRACKET: return ImGuiKey_LeftBracket;
        case AKEYCODE_BACKSLASH: return ImGuiKey_Backslash;
        case AKEYCODE_RIGHT_BRACKET: return ImGuiKey_RightBracket;
        case AKEYCODE_GRAVE: return ImGuiKey_GraveAccent;
        case AKEYCODE_CAPS_LOCK: return ImGuiKey_CapsLock;
        case AKEYCODE_SCROLL_LOCK: return ImGuiKey_ScrollLock;
        case AKEYCODE_NUM_LOCK: return ImGuiKey_NumLock;
        case AKEYCODE_SYSRQ: return ImGuiKey_PrintScreen;
        case AKEYCODE_BREAK: return ImGuiKey_Pause;
        case AKEYCODE_SPACE: return ImGuiKey_Space;
        case AKEYCODE_ENTER: return ImGuiKey_Enter;
        case AKEYCODE_ESCAPE: return ImGuiKey_Escape;
        case AKEYCODE_NUMPAD_0: return ImGuiKey_Keypad0;
        case AKEYCODE_NUMPAD_1: return ImGuiKey_Keypad1;
        case AKEYCODE_NUMPAD_2: return ImGuiKey_Keypad2;
        case AKEYCODE_NUMPAD_3: return ImGuiKey_Keypad3;
        case AKEYCODE_NUMPAD_4: return ImGuiKey_Keypad4;
        case AKEYCODE_NUMPAD_5: return ImGuiKey_Keypad5;
        case AKEYCODE_NUMPAD_6: return ImGuiKey_Keypad6;
        case AKEYCODE_NUMPAD_7: return ImGuiKey_Keypad7;
        case AKEYCODE_NUMPAD_8: return ImGuiKey_Keypad8;
        case AKEYCODE_NUMPAD_9: return ImGuiKey_Keypad9;
        case AKEYCODE_NUMPAD_DOT: return ImGuiKey_KeypadDecimal;
        case AKEYCODE_NUMPAD_DIVIDE: return ImGuiKey_KeypadDivide;
        case AKEYCODE_NUMPAD_MULTIPLY: return ImGuiKey_KeypadMultiply;
        case AKEYCODE_NUMPAD_SUBTRACT: return ImGuiKey_KeypadSubtract;
        case AKEYCODE_NUMPAD_ADD: return ImGuiKey_KeypadAdd;
        case AKEYCODE_NUMPAD_ENTER: return ImGuiKey_KeypadEnter;
        case AKEYCODE_NUMPAD_EQUALS: return ImGuiKey_KeypadEqual;
        case AKEYCODE_CTRL_LEFT: return ImGuiKey_LeftCtrl;
        case AKEYCODE_SHIFT_LEFT: return ImGuiKey_LeftShift;
        case AKEYCODE_ALT_LEFT: return ImGuiKey_LeftAlt;
        case AKEYCODE_META_LEFT: return ImGuiKey_LeftSuper;
        case AKEYCODE_CTRL_RIGHT: return ImGuiKey_RightCtrl;
        case AKEYCODE_SHIFT_RIGHT: return ImGuiKey_RightShift;
        case AKEYCODE_ALT_RIGHT: return ImGuiKey_RightAlt;
        case AKEYCODE_META_RIGHT: return ImGuiKey_RightSuper;
        case AKEYCODE_MENU: return ImGuiKey_Menu;
        case AKEYCODE_0: return ImGuiKey_0;
        case AKEYCODE_1: return ImGuiKey_1;
        case AKEYCODE_2: return ImGuiKey_2;
        case AKEYCODE_3: return ImGuiKey_3;
        case AKEYCODE_4: return ImGuiKey_4;
        case AKEYCODE_5: return ImGuiKey_5;
        case AKEYCODE_6: return ImGuiKey_6;
        case AKEYCODE_7: return ImGuiKey_7;
        case AKEYCODE_8: return ImGuiKey_8;
        case AKEYCODE_9: return ImGuiKey_9;
        case AKEYCODE_A: return ImGuiKey_A;
        case AKEYCODE_B: return ImGuiKey_B;
        case AKEYCODE_C: return ImGuiKey_C;
        case AKEYCODE_D: return ImGuiKey_D;
        case AKEYCODE_E: return ImGuiKey_E;
        case AKEYCODE_F: return ImGuiKey_F;
        case AKEYCODE_G: return ImGuiKey_G;
        case AKEYCODE_H: return ImGuiKey_H;
        case AKEYCODE_I: return ImGuiKey_I;
        case AKEYCODE_J: return ImGuiKey_J;
        case AKEYCODE_K: return ImGuiKey_K;
        case AKEYCODE_L: return ImGuiKey_L;
        case AKEYCODE_M: return ImGuiKey_M;
        case AKEYCODE_N: return ImGuiKey_N;
        case AKEYCODE_O: return ImGuiKey_O;
        case AKEYCODE_P: return ImGuiKey_P;
        case AKEYCODE_Q: return ImGuiKey_Q;
        case AKEYCODE_R: return ImGuiKey_R;
        case AKEYCODE_S: return ImGuiKey_S;
        case AKEYCODE_T: return ImGuiKey_T;
        case AKEYCODE_U: return ImGuiKey_U;
        case AKEYCODE_V: return ImGuiKey_V;
        case AKEYCODE_W: return ImGuiKey_W;
        case AKEYCODE_X: return ImGuiKey_X;
        case AKEYCODE_Y: return ImGuiKey_Y;
        case AKEYCODE_Z: return ImGuiKey_Z;
        case AKEYCODE_F1: return ImGuiKey_F1;
        case AKEYCODE_F2: return ImGuiKey_F2;
        case AKEYCODE_F3: return ImGuiKey_F3;
        case AKEYCODE_F4: return ImGuiKey_F4;
        case AKEYCODE_F5: return ImGuiKey_F5;
        case AKEYCODE_F6: return ImGuiKey_F6;
        case AKEYCODE_F7: return ImGuiKey_F7;
        case AKEYCODE_F8: return ImGuiKey_F8;
        case AKEYCODE_F9: return ImGuiKey_F9;
        case AKEYCODE_F10: return ImGuiKey_F10;
        case AKEYCODE_F11: return ImGuiKey_F11;
        case AKEYCODE_F12: return ImGuiKey_F12;
        default: return ImGuiKey_None;
        }
    }

    static unsigned int KeyCodeToCharacter(int32_t keyCode, bool upperCase)
    {
        switch (keyCode)
        {
        case AKEYCODE_SPACE: return ' ';
        case AKEYCODE_APOSTROPHE: return upperCase ? '"' : '\'';
        case AKEYCODE_COMMA: return upperCase ? '<' : ',';
        case AKEYCODE_MINUS: return upperCase ? '_' : '-';
        case AKEYCODE_PERIOD: return upperCase ? '>' : '.';
        case AKEYCODE_SLASH: return upperCase ? '?' : '/';
        case AKEYCODE_SEMICOLON: return upperCase ? ':' : ';';
        case AKEYCODE_EQUALS: return upperCase ? '+' : '=';
        case AKEYCODE_LEFT_BRACKET: return upperCase ? '{' : '[';
        case AKEYCODE_BACKSLASH: return upperCase ? '|' : '\\';
        case AKEYCODE_RIGHT_BRACKET: return upperCase ? '}' : ']';
        case AKEYCODE_GRAVE: return upperCase ? '~' : '`';
        case AKEYCODE_NUMPAD_0: return '0';
        case AKEYCODE_NUMPAD_1: return '1';
        case AKEYCODE_NUMPAD_2: return '2';
        case AKEYCODE_NUMPAD_3: return '3';
        case AKEYCODE_NUMPAD_4: return '4';
        case AKEYCODE_NUMPAD_5: return '5';
        case AKEYCODE_NUMPAD_6: return '6';
        case AKEYCODE_NUMPAD_7: return '7';
        case AKEYCODE_NUMPAD_8: return '8';
        case AKEYCODE_NUMPAD_9: return '9';
        case AKEYCODE_NUMPAD_DOT: return '.';
        case AKEYCODE_NUMPAD_DIVIDE: return '/';
        case AKEYCODE_NUMPAD_MULTIPLY: return '*';
        case AKEYCODE_NUMPAD_SUBTRACT: return '-';
        case AKEYCODE_NUMPAD_ADD: return '+';
        case AKEYCODE_NUMPAD_EQUALS: return '=';
        case AKEYCODE_0: return upperCase ? ')' : '0';
        case AKEYCODE_1: return upperCase ? '!' : '1';
        case AKEYCODE_2: return upperCase ? '@' : '2';
        case AKEYCODE_3: return upperCase ? '#' : '3';
        case AKEYCODE_4: return upperCase ? '$' : '4';
        case AKEYCODE_5: return upperCase ? '%' : '5';
        case AKEYCODE_6: return upperCase ? '^' : '6';
        case AKEYCODE_7: return upperCase ? '&' : '7';
        case AKEYCODE_8: return upperCase ? '*' : '8';
        case AKEYCODE_9: return upperCase ? '(' : '9';
        case AKEYCODE_A: return upperCase ? 'A' : 'a';
        case AKEYCODE_B: return upperCase ? 'B' : 'b';
        case AKEYCODE_C: return upperCase ? 'C' : 'c';
        case AKEYCODE_D: return upperCase ? 'D' : 'd';
        case AKEYCODE_E: return upperCase ? 'E' : 'e';
        case AKEYCODE_F: return upperCase ? 'F' : 'f';
        case AKEYCODE_G: return upperCase ? 'G' : 'g';
        case AKEYCODE_H: return upperCase ? 'H' : 'h';
        case AKEYCODE_I: return upperCase ? 'I' : 'i';
        case AKEYCODE_J: return upperCase ? 'J' : 'j';
        case AKEYCODE_K: return upperCase ? 'K' : 'k';
        case AKEYCODE_L: return upperCase ? 'L' : 'l';
        case AKEYCODE_M: return upperCase ? 'M' : 'm';
        case AKEYCODE_N: return upperCase ? 'N' : 'n';
        case AKEYCODE_O: return upperCase ? 'O' : 'o';
        case AKEYCODE_P: return upperCase ? 'P' : 'p';
        case AKEYCODE_Q: return upperCase ? 'Q' : 'q';
        case AKEYCODE_R: return upperCase ? 'R' : 'r';
        case AKEYCODE_S: return upperCase ? 'S' : 's';
        case AKEYCODE_T: return upperCase ? 'T' : 't';
        case AKEYCODE_U: return upperCase ? 'U' : 'u';
        case AKEYCODE_V: return upperCase ? 'V' : 'v';
        case AKEYCODE_W: return upperCase ? 'W' : 'w';
        case AKEYCODE_X: return upperCase ? 'X' : 'x';
        case AKEYCODE_Y: return upperCase ? 'Y' : 'y';
        case AKEYCODE_Z: return upperCase ? 'Z' : 'z';
        default: return 0;
        }
    }

    AImGui::AImGui(const Options &options)
        : m_options(options)
    {
        InitEnvironment();
    }

    AImGui::~AImGui()
    {
        UnInitEnvironment();
    }

    void AImGui::BeginFrame()
    {
        if (!m_state)
            return;

        if (m_options.autoUpdateOrientation)
        {
            auto displayInfo = ANativeWindowCreator::GetDisplayInfo();
            if (m_rotateTheta != displayInfo.theta)
            {
                UnInitEnvironment();
                InitEnvironment();
            }
        }

        ANativeWindowCreator::ProcessMirrorDisplay();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplAndroid_NewFrame();
        ImGui::NewFrame();
    }

    void AImGui::EndFrame()
    {
        if (!m_state)
            return;

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        eglSwapBuffers(m_defaultDisplay, m_eglSurface);
    }

    void AImGui::ProcessInputEvent()
    {
        if (!m_state)
            return;

        auto &imguiIO = ImGui::GetIO();
        static ATouchEvent touchEvent;
        ATouchEvent::TouchEvent event{};

        while (touchEvent.GetTouchEvent(&event))
        {
            switch (event.type)
            {
            case ATouchEvent::EventType::Move:
            {
                event.TransformToScreen(m_screenWidth, m_screenHeight, m_rotateTheta);
                imguiIO.AddMousePosEvent((float)event.x, (float)event.y);
                break;
            }
            case ATouchEvent::EventType::TouchDown:
            case ATouchEvent::EventType::TouchUp:
            {
                event.TransformToScreen(m_screenWidth, m_screenHeight, m_rotateTheta);
                imguiIO.AddMousePosEvent((float)event.x, (float)event.y);
                imguiIO.AddMouseButtonEvent(0, ATouchEvent::EventType::TouchDown == event.type);
                break;
            }
            case ATouchEvent::EventType::KeyDown:
            case ATouchEvent::EventType::KeyUp:
            {
                auto imguiKey = KeyCodeToImGuiKey(event.keyCode);
                if (ImGuiKey_None == imguiKey)
                    break;

                switch (imguiKey)
                {
                case ImGuiKey_LeftCtrl:
                case ImGuiKey_RightCtrl:
                    imguiIO.AddKeyEvent(ImGuiMod_Ctrl, ATouchEvent::EventType::KeyDown == event.type);
                    break;
                case ImGuiKey_LeftShift:
                case ImGuiKey_RightShift:
                    imguiIO.AddKeyEvent(ImGuiMod_Shift, ATouchEvent::EventType::KeyDown == event.type);
                    break;
                case ImGuiKey_LeftAlt:
                case ImGuiKey_RightAlt:
                    imguiIO.AddKeyEvent(ImGuiMod_Alt, ATouchEvent::EventType::KeyDown == event.type);
                    break;
                default:
                    break;
                }
                imguiIO.AddKeyEvent(imguiKey, ATouchEvent::EventType::KeyDown == event.type);
                imguiIO.SetKeyEventNativeData(imguiKey, event.keyCode, event.scanCode);

                if (ATouchEvent::EventType::KeyDown != event.type)
                    break;
                unsigned int character = KeyCodeToCharacter(event.keyCode, ImGui::IsKeyDown(ImGuiMod_Shift));
                if (imguiIO.WantTextInput && 0 != character)
                    imguiIO.AddInputCharacter(character);
                break;
            }
            case ATouchEvent::EventType::Wheel:
            {
                imguiIO.AddMousePosEvent((float)std::abs(event.x), (float)event.y);
                imguiIO.AddMouseWheelEvent(0, 0 > event.x ? -1.0f : 1.0f);
                break;
            }
            default:
                break;
            }
        }
    }

    void AImGui::SetupWindowInfo(void *windowInfo)
    {
        ANativeWindowCreator::UpdateWindowInfo(m_nativeWindow, windowInfo);
    }

    bool AImGui::InitEnvironment()
    {
        auto displayInfo = ANativeWindowCreator::GetDisplayInfo();
        LogInfo("[=] Display angle:%d width:%d height:%d", displayInfo.theta, displayInfo.width, displayInfo.height);

        m_nativeWindow = ANativeWindowCreator::Create({.name = "AImGui", .skipScreenshot = false});
        if (nullptr == m_nativeWindow)
        {
            LogDebug("[-] ANativeWindow create failed");
            return false;
        }

        LogInfo("[=] Acquiring native window");
        ANativeWindow_acquire(m_nativeWindow);
        LogInfo("[+] Native window acquired");

        m_defaultDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (EGL_NO_DISPLAY == m_defaultDisplay) return false;

        if (EGL_TRUE != eglInitialize(m_defaultDisplay, 0, 0)) return false;

        EGLint numEglConfig = 0;
        EGLConfig eglConfig{};
        EGLint eglConfigAttributeList[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 24, EGL_STENCIL_SIZE, 8,
            EGL_SAMPLE_BUFFERS, 0,
            EGL_NONE
        };
        if (EGL_TRUE != eglChooseConfig(m_defaultDisplay, eglConfigAttributeList, &eglConfig, 1, &numEglConfig)) return false;

        EGLint eglBufferFormat;
        eglGetConfigAttrib(m_defaultDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &eglBufferFormat);
        ANativeWindow_setBuffersGeometry(m_nativeWindow, 0, 0, eglBufferFormat);
        m_eglSurface = eglCreateWindowSurface(m_defaultDisplay, eglConfig, m_nativeWindow, nullptr);
        if (EGL_NO_SURFACE == m_eglSurface) return false;

        EGLint eglContextAttribList[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
        m_eglContext = eglCreateContext(m_defaultDisplay, eglConfig, EGL_NO_CONTEXT, eglContextAttribList);
        if (EGL_NO_CONTEXT == m_eglContext) return false;

        if (EGL_TRUE != eglMakeCurrent(m_defaultDisplay, m_eglSurface, m_eglSurface, m_eglContext)) return false;

        IMGUI_CHECKVERSION();
        m_imguiContext = ImGui::CreateContext();
        if (nullptr == m_imguiContext) return false;

        auto &imguiIO = ImGui::GetIO();
        imguiIO.IniFilename = nullptr;
        ImGui::StyleColorsDark();
        ImGui::GetStyle().ScaleAllSizes(3.f);

        ImFontConfig fontConfig;
        fontConfig.SizePixels = 22.f;
        imguiIO.Fonts->AddFontDefault(&fontConfig);

        if (!ImGui_ImplAndroid_Init(m_nativeWindow)) return false;
        if (!ImGui_ImplOpenGL3_Init("#version 300 es")) return false;

        glViewport(0, 0, displayInfo.width, displayInfo.height);
        glClearColor(0.f, 0.f, 0.f, 0.f);

        m_rotateTheta = displayInfo.theta;
        m_screenWidth = displayInfo.width;
        m_screenHeight = displayInfo.height;

        return (m_state = true);
    }

    void AImGui::UnInitEnvironment()
    {
        m_state = false;
        if (nullptr != m_imguiContext)
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplAndroid_Shutdown();
            ImGui::DestroyContext(m_imguiContext);
        }
        if (EGL_NO_DISPLAY != m_defaultDisplay)
        {
            eglMakeCurrent(m_defaultDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            if (EGL_NO_CONTEXT != m_eglContext) eglDestroyContext(m_defaultDisplay, m_eglContext);
            if (EGL_NO_SURFACE != m_eglSurface) eglDestroySurface(m_defaultDisplay, m_eglSurface);
            eglTerminate(m_defaultDisplay);
        }
        if (nullptr != m_nativeWindow)
        {
            ANativeWindow_release(m_nativeWindow);
            android::ANativeWindowCreator::Destroy(m_nativeWindow);
        }
        m_imguiContext = nullptr;
        m_eglContext = EGL_NO_CONTEXT;
        m_eglSurface = EGL_NO_SURFACE;
        m_defaultDisplay = EGL_NO_DISPLAY;
        m_nativeWindow = nullptr;
    }
} // namespace android