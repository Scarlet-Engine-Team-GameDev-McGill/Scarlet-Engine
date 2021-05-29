#pragma once

#include "Core.h"

namespace ScarletEngine
{
    using KeyCode = uint32_t;
    using MouseCode = uint32_t;

    //------------------------------------------------------------------------------------------------------------------

    enum class EKeyState
    {
        NotPressed,
        Pressed,
        Held,
        Released,
    };

    //------------------------------------------------------------------------------------------------------------------

    /* External input events */
    using OnKeyDownEvent = Event<KeyCode>;
    using OnKeyUpEvent = Event<KeyCode>;
    using OnMouseMoveEvent = Event<MouseCode>;
    using OnMouseButtonDownEvent = Event<MouseCode>;
    using OnMouseButtonUpEvent = Event<MouseCode>;

    //------------------------------------------------------------------------------------------------------------------

    /** Manages the state of HID devices such as keyboard, mouse, and controllers */
    class InputManager
    {
    public:
        void PostUpdate();
        static InputManager& Get() { static InputManager Instance; return Instance; }

        /* Keyboard events */
        
        /** @returns true if a given key was just pressed */
        bool IsKeyPressed(KeyCode Code) const;
        /** @returns true if a given key is being held down */
        bool IsKeyHeld(KeyCode Code) const;
        /** @returns true if a given key has just been released */
        bool IsKeyReleased(KeyCode Code) const;

        /** @returns the state of a given key */
        EKeyState GetKeyState(KeyCode Code) const;

        /** Called by the OS when a key is pressed */
        void OnKeyDownCallback(KeyCode Code);
        /** Called by the OS when a key is released */
        void OnKeyUpCallback(KeyCode Code);

        /* Mouse Events */

        /** @returns true if a given mouse button was just pressed */
        bool IsMouseButtonPressed(MouseCode Code) const;
        /** @returns true if a given mouse button is being held down */
        bool IsMouseButtonHeld(MouseCode Code) const;
        /** @returns true if a given mouse button has just been released */
        bool IsMouseButtonReleased(MouseCode Code) const;
        /** @returns the current mouse position */
        glm::vec2 GetMousePos() const { return MousePos; };
        /** @returns the mouse delta mouse position from the last frame */
        glm::vec2 GetMouseDelta() const { return MouseDelta; }

        /** @returns the state of a given mouse button */
        EKeyState GetMouseButtonState(MouseCode Code) const;

        /** Called by the OS when the mouse moves */
        void OnMouseMoveCallback(glm::vec2 NewPos);
        /** Called by the OS when a mouse button is pressed */
        void OnMousePressCallback(MouseCode Code);
        /** Called by the OS when a mouse button is released */
        void OnMouseReleaseCallback(MouseCode Code);

        /* Public subscribable events */

        OnKeyDownEvent OnKeyDown;
        OnKeyUpEvent OnKeyUp;
        OnMouseMoveEvent OnMouseMove;
        OnMouseButtonDownEvent OnMouseButtonDown;
        OnMouseButtonUpEvent OnMouseButtonUp;
    private:
        /** Map of all keyboard buttons to their current states */
        UnorderedMap<KeyCode, EKeyState> KeyMap;
        /** Map of all mouse buttons to their current states */
        UnorderedMap<MouseCode, EKeyState> MouseMap;
        /** Current mouse position */
        glm::vec2 MousePos{};
        /** Delta mouse pos from last frame */
        glm::vec2 MouseDelta{};
    };
}