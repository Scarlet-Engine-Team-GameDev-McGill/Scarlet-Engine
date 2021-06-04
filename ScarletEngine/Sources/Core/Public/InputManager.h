#pragma once

#include "Core.h"

namespace ScarletEngine
{
    //------------------------------------------------------------------------------------------------------------------

    /* External input events */
    using OnKeyDownEvent = Event<EKeyCode>;
    using OnKeyUpEvent = Event<EKeyCode>;
    using OnMouseButtonDownEvent = Event<EMouseCode>;
    using OnMouseButtonUpEvent = Event<EMouseCode>;
    using OnMouseMoveEvent = Event<glm::vec2>;

    //------------------------------------------------------------------------------------------------------------------

    /** Manages the state of HID devices such as keyboard, mouse, and controllers */
    class InputManager
    {
    public:
        void PostUpdate();
        static InputManager& Get() { static InputManager Instance; return Instance; }

        /* Keyboard events */
        
        /** @returns true if a given key was just pressed */
        bool IsKeyPressed(EKeyCode Code) const;
        /** @returns true if a given key is being held down */
        bool IsKeyHeld(EKeyCode Code) const;
        /** @returns true if a given key has just been released */
        bool IsKeyReleased(EKeyCode Code) const;

        /** @returns the state of a given key */
        EKeyState GetKeyState(EKeyCode Code) const;

        /** Called by the OS when a key is pressed */
        void OnKeyDownCallback(EKeyCode Code);
        /** Called by the OS when a key is released */
        void OnKeyUpCallback(EKeyCode Code);

        /* Mouse Events */

        /** @returns true if a given mouse button was just pressed */
        bool IsMouseButtonPressed(EMouseCode Code) const;
        /** @returns true if a given mouse button is being held down */
        bool IsMouseButtonHeld(EMouseCode Code) const;
        /** @returns true if a given mouse button has just been released */
        bool IsMouseButtonReleased(EMouseCode Code) const;
        /** @returns the current mouse position */
        glm::vec2 GetMousePos() const { return MousePos; };
        /** @returns the mouse delta mouse position from the last frame */
        glm::vec2 GetMouseDelta() const { return MouseDelta; }

        /** @returns the state of a given mouse button */
        EKeyState GetMouseButtonState(EMouseCode Code) const;

        /** Called by the OS when the mouse moves */
        void OnMouseMoveCallback(glm::vec2 NewPos);
        /** Called by the OS when a mouse button is pressed */
        void OnMousePressCallback(EMouseCode Code);
        /** Called by the OS when a mouse button is released */
        void OnMouseReleaseCallback(EMouseCode Code);

        /* Public subscribable events */

        OnKeyDownEvent OnKeyDown;
        OnKeyUpEvent OnKeyUp;
        OnMouseMoveEvent OnMouseMove;
        OnMouseButtonDownEvent OnMouseButtonDown;
        OnMouseButtonUpEvent OnMouseButtonUp;
    private:
        /** Map of all keyboard buttons to their current states */
        UnorderedMap<EKeyCode, EKeyState> KeyMap;
        /** Map of all mouse buttons to their current states */
        UnorderedMap<EMouseCode, EKeyState> MouseMap;
        /** Current mouse position */
        glm::vec2 MousePos{};
        /** Delta mouse pos from last frame */
        glm::vec2 MouseDelta{};
    };
}