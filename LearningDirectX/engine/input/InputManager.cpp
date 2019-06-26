#include "InputManager.h"

namespace core
{


    InputManager::InputManager()
    {
        m_keyActionCurrent = 0;
        m_mouseAction = 0;
        m_delta = { 0 };
    }

    InputManager::~InputManager()
    {
    }
}