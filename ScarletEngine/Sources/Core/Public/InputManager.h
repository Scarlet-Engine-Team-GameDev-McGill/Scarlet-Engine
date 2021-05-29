#pragma once

#include "Core.h"

namespace ScarletEngine
{
    enum class EKeyCode
    {
        KeyUnknown,
        KeySpace, 
        KeyApostrophe, /* ' */ 
        KeyComma, /* , */ 
        KeyMinus, /* - */ 
        KeyPeriod, /* . */ 
        KeySlash, /* / */ 
        Key0, 
        Key1, 
        Key2, 
        Key3, 
        Key4, 
        Key5, 
        Key6, 
        Key7, 
        Key8, 
        Key9, 
        KeySemicolon, /* ; */ 
        KeyEqual, /* = */ 
        KeyA, 
        KeyB, 
        KeyC, 
        KeyD, 
        KeyE, 
        KeyF, 
        KeyG, 
        KeyH, 
        KeyI, 
        KeyJ, 
        KeyK, 
        KeyL, 
        KeyM, 
        KeyN, 
        KeyO, 
        KeyP, 
        KeyQ, 
        KeyR, 
        KeyS, 
        KeyT, 
        KeyU, 
        KeyV, 
        KeyW, 
        KeyX, 
        KeyY, 
        KeyZ, 
        KeyLeftBracket, /* [ */ 
        KeyBackslash, /* \ */ 
        KeyRightBracket, /* ] */ 
        KeyGraveAccent, /* ` */ 
        KeyWorld1, /* non-US #1 */ 
        KeyWorld2, /* non-US #2 */ 
        KeyEscape, 
        KeyEnter, 
        KeyTab, 
        KeyBackspace, 
        KeyInsert, 
        KeyDelete, 
        KeyRight, 
        KeyLeft, 
        KeyDown, 
        KeyUp, 
        KeyPageUp, 
        KeyPageDown, 
        KeyHome, 
        KeyEnd, 
        KeyCapsLock, 
        KeyScrollLock, 
        KeyNumLock, 
        KeyPrintScreen, 
        KeyPause, 
        KeyF1, 
        KeyF2, 
        KeyF3, 
        KeyF4, 
        KeyF5, 
        KeyF6, 
        KeyF7, 
        KeyF8, 
        KeyF9, 
        KeyF10, 
        KeyF11, 
        KeyF12, 
        KeyF13, 
        KeyF14, 
        KeyF15, 
        KeyF16, 
        KeyF17, 
        KeyF18, 
        KeyF19, 
        KeyF20, 
        KeyF21, 
        KeyF22, 
        KeyF23, 
        KeyF24, 
        KeyF25, 
        KeyNum0, 
        KeyNum1, 
        KeyNum2, 
        KeyNum3, 
        KeyNum4, 
        KeyNum5, 
        KeyNum6, 
        KeyNum7, 
        KeyNum8, 
        KeyNum9, 
        KeyNumDecimal, 
        KeyNumDivide, 
        KeyNumMultiply, 
        KeyNumSubtract, 
        KeyNumAdd, 
        KeyNumEnter, 
        KeyNumEqual, 
        KeyLeftShift, 
        KeyLeftControl, 
        KeyLeftAlt, 
        KeyLeftSuper, 
        KeyRightShift, 
        KeyRightControl, 
        KeyRightAlt, 
        KeyRightSuper, 
        KeyMenu,
    };

    //------------------------------------------------------------------------------------------------------------------

    enum class EMouseCode
    {
        MouseUnknown,
        MouseButton1,
        MouseButton2,
        MouseButton3,
        MouseButton4,
        MouseButton5,
        MouseButton6,
        MouseButton7,
        MouseButton8,
        MouseButtonLast = MouseButton8,
        MouseButtonLeft = MouseButton1,
        MouseButtonRight = MouseButton2,
        MouseButtonMiddle = MouseButton3,
    };

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
    using OnKeyDownEvent = Event<EKeyCode>;
    using OnKeyUpEvent = Event<EKeyCode>;
    using OnMouseMoveEvent = Event<EMouseCode>;
    using OnMouseButtonDownEvent = Event<EMouseCode>;
    using OnMouseButtonUpEvent = Event<EMouseCode>;

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