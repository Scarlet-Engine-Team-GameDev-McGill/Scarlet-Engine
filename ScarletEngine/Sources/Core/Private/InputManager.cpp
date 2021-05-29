#include "InputManager.h"

namespace ScarletEngine
{
    bool InputManager::IsKeyPressed(EKeyCode Code) const
    {
        return GetKeyState(Code) == EKeyState::Pressed;
    }

    bool InputManager::IsKeyHeld(EKeyCode Code) const
    {
        return GetKeyState(Code) == EKeyState::Held;
    }

    bool InputManager::IsKeyReleased(EKeyCode Code) const
    {
        return GetKeyState(Code) == EKeyState::Released;
    }

    EKeyState InputManager::GetKeyState(EKeyCode Code) const
    {
        const auto It = KeyMap.find(Code);
        return It != KeyMap.end() ? It->second : EKeyState::NotPressed;
    }

    void InputManager::OnKeyDownCallback(EKeyCode Code)
    {
        KeyMap[Code] = EKeyState::Pressed;
        OnKeyDown.Broadcast(Code);
    }

    void InputManager::OnKeyUpCallback(EKeyCode Code)
    {
        KeyMap[Code] = EKeyState::Released;
        OnKeyUp.Broadcast(Code);
    }

    bool InputManager::IsMouseButtonPressed(EMouseCode Code) const
    {
        return GetMouseButtonState(Code) == EKeyState::Pressed;
    }

    bool InputManager::IsMouseButtonHeld(EMouseCode Code) const
    {
        return GetMouseButtonState(Code) == EKeyState::Held;
    }

    bool InputManager::IsMouseButtonReleased(EMouseCode Code) const
    {
        return GetMouseButtonState(Code) == EKeyState::Released;
    }

    EKeyState InputManager::GetMouseButtonState(EMouseCode Code) const
    {
        const auto It = MouseMap.find(Code);
        return It != MouseMap.end() ? It->second : EKeyState::NotPressed;
    }

    void InputManager::OnMouseMoveCallback(glm::vec2 NewPos)
    {
        MouseDelta = MousePos - NewPos;
        MousePos = NewPos;
        OnMouseMove.Broadcast(NewPos);
    }

    void InputManager::OnMousePressCallback(EMouseCode Code)
    {
        MouseMap[Code] = EKeyState::Pressed;
        OnMouseButtonDown.Broadcast(Code);
    }

    void InputManager::OnMouseReleaseCallback(EMouseCode Code)
    {
        MouseMap[Code] = EKeyState::Released;
        OnMouseButtonUp.Broadcast(Code);
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
