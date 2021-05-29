#include "InputManager.h"

namespace ScarletEngine
{
    bool InputManager::IsKeyPressed(KeyCode Code) const
    {
        return GetKeyState(Code) == EKeyState::Pressed;
    }

    bool InputManager::IsKeyHeld(KeyCode Code) const
    {
        return GetKeyState(Code) == EKeyState::Held;
    }

    bool InputManager::IsKeyReleased(KeyCode Code) const
    {
        return GetKeyState(Code) == EKeyState::Released;
    }

    EKeyState InputManager::GetKeyState(KeyCode Code) const
    {
        const auto It = KeyMap.find(Code);
        return It != KeyMap.end() ? It->second : EKeyState::NotPressed;
    }

    void InputManager::OnKeyDownCallback(KeyCode Code)
    {
        KeyMap[Code] = EKeyState::Pressed;
    }

    void InputManager::OnKeyUpCallback(KeyCode Code)
    {
        KeyMap[Code] = EKeyState::Released;
    }

    bool InputManager::IsMouseButtonPressed(MouseCode Code) const
    {
        return GetMouseButtonState(Code) == EKeyState::Pressed;
    }

    bool InputManager::IsMouseButtonHeld(MouseCode Code) const
    {
        return GetMouseButtonState(Code) == EKeyState::Held;
    }

    bool InputManager::IsMouseButtonReleased(MouseCode Code) const
    {
        return GetMouseButtonState(Code) == EKeyState::Released;
    }

    EKeyState InputManager::GetMouseButtonState(MouseCode Code) const
    {
        const auto It = MouseMap.find(Code);
        return It != MouseMap.end() ? It->second : EKeyState::NotPressed;
    }

    void InputManager::OnMouseMoveCallback(glm::vec2 NewPos)
    {
        MouseDelta = MousePos - NewPos;
        MousePos = NewPos;
    }

    void InputManager::OnMousePressCallback(MouseCode Code)
    {
        MouseMap[Code] = EKeyState::Pressed;
    }

    void InputManager::OnMouseReleaseCallback(MouseCode Code)
    {
        MouseMap[Code] = EKeyState::Released;
    }

    void InputManager::PostUpdate()
    {
        for (auto& [Code, State] : KeyMap)
        {
            // Set all keys that were released this frame to NotPressed for the next frame 
            if (State == EKeyState::Released)
            {
                State = EKeyState::NotPressed;
            }
            // Set all pressed keys to held if they haven't been released this frame.
            else if (State == EKeyState::Pressed)
            {
                State = EKeyState::Held;
            }
        }

        for (auto& [Code, State] : MouseMap)
        {
            // Set all buttons that were released this frame to NotPressed for the next frame 
            if (State == EKeyState::Released)
            {
                State = EKeyState::NotPressed;
            }
            // Set all pressed buttons to held if they haven't been released this frame.
            else if (State == EKeyState::Pressed)
            {
                State = EKeyState::Held;
            }
        }
    }
}
