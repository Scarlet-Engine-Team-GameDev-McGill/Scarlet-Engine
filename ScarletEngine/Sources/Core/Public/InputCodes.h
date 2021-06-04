#pragma once

namespace ScarletEngine
{
    // -----------------------------------------------------------------------------------------------------------------

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
}