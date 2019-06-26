#pragma once

#include "engine/XPlatform.h"
#include "Physics.h"

namespace core {

    class InputManager;
    class Renderer;
    class ShaderManager;
    class Scene;
    class SceneGraph;

    class Application
    {
    public:
        Application() 
        {
            srand(GetTickCount());
            s_instance = this;
        }
        ~Application() {}

        bool init(HINSTANCE hInstance);
        void run();
        void exit();

        static Application* sharedApplication() { return s_instance; }

        void runWithScene(SceneGraph*);

        void handleKeyPressed(WPARAM wp);
        void handleKeyReleased(WPARAM wp);
        void handleMouse(UINT msg, LPARAM lParam, WPARAM wParam);
        bool m_stop;
    protected:
        World *m_World;
    private:

        static Application* s_instance;

        SceneGraph* m_CurrentScene;

        InputManager* m_inputManager;
        ShaderManager* m_shaderManager;

        unsigned long m_lastTime;

        bool createWindow();
        bool destroyWindow();

        bool initRenderer();
    };
}