#pragma once
#include "engine/XPlatform.h"
#include "renderer/Renderer.h"
#include "framework/Singleton.h"

namespace core
{
    class InputManager : public Singleton<InputManager>
    {
        friend class Application;

    public:
        InputManager();
        ~InputManager();

        enum KeyAction
        {
            KEY_LEFT = 1,
            KEY_RIGHT = KEY_LEFT << 1,
            KEY_UP = KEY_RIGHT << 1,
            KEY_DOWN = KEY_UP << 1,
            KEY_FIRE = KEY_DOWN << 1,
            KEY_JUMP = KEY_FIRE << 1,
            KEY_SHIFT = KEY_JUMP << 1,
            KEY_ALT = KEY_SHIFT << 1,
        };

        enum MouseAction
        {
            MOUSE_L_PRESSED = 1,
            MOUSE_L_RELEASED = 1 << 2,
            MOUSE_L_DRAGGED = 1 << 3,
            MOUSE_L_DOUBLECLK = 1 << 4,

            MOUSE_R_PRESSED = 1 << 5,
            MOUSE_R_RELEASED = 1 << 6,
            MOUSE_R_DRAGGED = 1 << 7,
            MOUSE_R_DOUBLECLK = 1 << 8,

            MOUSE_M_PRESSED = 1 << 9,
            MOUSE_M_RELEASED = 1 << 10,
            MOUSE_M_DRAGGED = 1 << 11,
            MOUSE_M_DOUBLECLK = 1 << 12,

            MOUSE_WHEEL_SCROLL_UP = 1 << 13,
            MOUSE_WHEEL_SCROLL_DOWN = 1 << 14,
        };

        bool isKeyPressed(KeyAction _key) const
        {
            return (_key & m_keyActionCurrent) != 0;
        }

        struct uint2d
        {
            unsigned int x;
            unsigned int y;
        };

        struct int2d
        {
            int x;
            int y;
        };

        typedef uint2d MousePos;
        typedef int2d MouseDelta;

        bool isMouseAction(MouseAction _action)
        {
            return (m_mouseAction & _action)==0?false:true ;
        }

        const MousePos& getMousePosition() const
        {
            return m_currentMousePos;
        }

        const MouseDelta& getMouseDelta() const
        {
            return m_delta;
        }

    private:

        unsigned int m_keyActionCurrent;

        void handleKeyPressed(WPARAM wParam)
        {
            switch (wParam)
            {
            case VK_LEFT:
                m_keyActionCurrent |= KEY_LEFT;
                break;

            case VK_RIGHT:
                m_keyActionCurrent |= KEY_RIGHT;
                break;

            case VK_UP:
                m_keyActionCurrent |= KEY_UP;
                break;

            case VK_DOWN:
                m_keyActionCurrent |= KEY_DOWN;
                break;

            case VK_HOME:break;
            case VK_END:break;
            case VK_INSERT:break;
            case VK_DELETE:break;
            case VK_F2:break;

            }
        }

        void handleKeyReleased(WPARAM wParam)
        {
            switch (wParam)
            {
            case VK_LEFT:
                m_keyActionCurrent &= ~KEY_LEFT;
                break;

            case VK_RIGHT:
                m_keyActionCurrent &= ~KEY_RIGHT;
                break;

            case VK_UP:
                m_keyActionCurrent &= ~KEY_UP;
                break;

            case VK_DOWN:
                m_keyActionCurrent &= ~KEY_DOWN;
                break;

            case VK_HOME:break;
            case VK_END:break;
            case VK_INSERT:break;
            case VK_DELETE:break;
            case VK_F2:break;

            }
        }

        unsigned int m_mouseAction;
        MousePos m_currentMousePos;
        MousePos m_lastMousePos;
        MouseDelta m_delta;


        void handleMouse(UINT msg, LPARAM lParam,WPARAM wParam)
        {
            short scroll;

            switch (msg)
            {
            case WM_LBUTTONDOWN:
                m_mouseAction |= MOUSE_L_PRESSED;
                break;
            case WM_LBUTTONUP:
                m_mouseAction &= !MOUSE_L_PRESSED;
                break;
            case WM_LBUTTONDBLCLK:
                m_mouseAction |= MOUSE_L_DOUBLECLK;
                break; 
            case WM_MOUSEWHEEL:
                scroll = HIWORD(wParam);
                break;
            case WM_MOUSEMOVE:
                m_lastMousePos = m_currentMousePos;
               
                m_currentMousePos.x = LOWORD(lParam);
                m_currentMousePos.y = RENDERER->GetScreenHeight() - HIWORD(lParam);

                m_delta.x = m_currentMousePos.x - m_lastMousePos.x;
                m_delta.y = m_currentMousePos.y - m_lastMousePos.y;

                break;
            default:
                break;
            }
        }

        void resetInput()
        {
            m_keyActionCurrent = m_mouseAction = 0;
        }
        void Update() {
            //resetInput();
            m_delta = { 0 };
        }
    };
    #define INPUTMANAGER InputManager::getPtr()
}