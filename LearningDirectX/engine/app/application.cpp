#include "application.h"

#include <string>
#include <vector>

#include "renderer/Renderer.h"
#include "engine/Model/Model.h"
#include "renderer/ShaderManager.h"
#include "engine/input/InputManager.h"
#include "engine/Logger.h"
#include "framework/Singleton.h"
#include "engine/timer/GameTimer.h"
#include "engine/Util.h"
#include "engine/core/Core_SceneGraph.h"
#include "engine/core/Nodes/Node_Model.h"

#define MAX_LOADSTRING 100
#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

const double g_frameRate = 60.0;
const double g_frameTime = 1000.0 / g_frameRate;
const double g_refreshRateFPS = 0.1;



namespace core {

    // Global Variables:
    HINSTANCE hInst;                                        // current instance
    TCHAR szTitle[MAX_LOADSTRING];                         // The title bar text
    TCHAR szWindowClass[MAX_LOADSTRING];               // the main window class name
    HWND hWnd;

    Application* Application::s_instance = 0;
   
    bool Application::init(HINSTANCE hInstance)
    {

        hInst = hInstance;

        //m_World = new World();

        m_shaderManager  = new ShaderManager();

        //new Logger();

        if (!createWindow())
            return FALSE;

        if (!initRenderer())
               return FALSE;

        INPUTMANAGER->create();
        GAMETIMER->create();
                
        return TRUE;
    }

    void Application::run()
    {
        MSG msg;
        unsigned long timeSinceLastOutput = 0;
        unsigned long timeLastOutputed = 0;
        // Main message loop:

        GAMETIMER->Start();
        m_stop = false;
        float slept = 0.0f;

        int counter = 0;
        int fpsInterval = 10;
        while (!m_stop)
        { 
            GAMETIMER->Tick();
            INPUTMANAGER->Update();
            if (PeekMessage(&msg, NULL, 0, 0, 1))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            if (INPUTMANAGER->isMouseAction(InputManager::MOUSE_L_PRESSED))
            {
                OutputDebugString("Left Mouse");
            }

            //m_World->Step(GAMETIMER->DeltaTime());

            RENDERER->beginFrame();

            //prepare scene graph;
            m_CurrentScene->update();

            RENDERER->drawFrame();
            RENDERER->endFrame();

            float dt = GAMETIMER->DeltaTime();

            slept = max(0, 0.0016 - dt);

            Sleep(1);

            if (++counter >= fpsInterval)
            {
                SetWindowText(hWnd, Util::getStr(1 / (dt)).c_str());
                counter = 0;
            }
        }
    }

    void Application::runWithScene(SceneGraph* next_scene)
    {
        m_CurrentScene = next_scene;
    }

    void Application::handleKeyPressed(WPARAM wParam)
    {
        INPUTMANAGER->handleKeyPressed(wParam);
    }

    void Application::handleKeyReleased(WPARAM wParam)
    {
        INPUTMANAGER->handleKeyReleased(wParam);
    }

    void Application::handleMouse(UINT msg, LPARAM lParam, WPARAM wParam)
    {
        INPUTMANAGER->handleMouse(msg, lParam, wParam);
    }


    void Application::exit()
    {
        if (RENDERER)
        {
            delete RENDERER;
        }
    }

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        int wmId, wmEvent;
        PAINTSTRUCT ps;
        HDC hdc;

        switch (message)
        {
        case WM_COMMAND:
            wmId = LOWORD(wParam);
            wmEvent = HIWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case WM_CLOSE:
                DestroyWindow(hWnd);
                exit(0);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;

        case WM_MBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK:
        case WM_MOUSEMOVE:
        case WM_MOUSEWHEEL:
            Application::sharedApplication()->handleMouse(message, lParam, wParam);
        case WM_KEYDOWN:
            Application::sharedApplication()->handleKeyPressed(wParam);
            break;
        case WM_KEYUP:
            Application::sharedApplication()->handleKeyReleased(wParam);
            break;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code here...
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            Application::sharedApplication()->m_stop = true;
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }


    ATOM MyRegisterClass(HINSTANCE hInstance)
    {
        WNDCLASSEX wcex = { 0 };

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.hInstance = hInstance;
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
        wcex.lpszClassName = "wclass";

        return RegisterClassEx(&wcex);
    }

    bool Application::createWindow()
    {
        // TODO: Place code here.
        // Initialize global strings

        MyRegisterClass(hInst);

        hWnd = CreateWindow("wclass",
            "asdasd",
            WS_OVERLAPPEDWINDOW,
            300, 300,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            NULL,
            NULL,
            hInst,
            NULL);

        if (!hWnd)
        {
            return FALSE;
        }

        ShowWindow(hWnd, 1);
        UpdateWindow(hWnd);
        SetFocus(hWnd);

        return TRUE;
    }

    bool Application::initRenderer()
    {
        RENDERER->create();
        RENDERER->init(hWnd);
        return true;
    }
}