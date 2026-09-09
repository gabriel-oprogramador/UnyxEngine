#pragma once
#include <cstdint>
#include <cstddef>
#include <cstdio>

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;
using usize = size_t;
using intptr = intptr_t;
using uintptr = uintptr_t;
using cstring = const char*;

#if defined(PLATFORM_WINDOWS)

#if defined(UNYX_ENGINE_BUILD)
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
#define GAME_API extern "C" __declspec(dllexport)

#elif defined(PLATFORM_LINUX)

#if defined(UNYX_ENGINE_BUILD)
#define ENGINE_API __attribute__((visibility("default")))
#else
#define ENGINE_API
#endif
#define GAME_API extern "C" __attribute__((visibility("default")))

#else
#define ENGINE_API
#define GAME_API extern "C"
#endif

#define XMACRO_KEYS(T) \
  T(KEY_UNKNOWN)       \
  T(KEY_ZERO)          \
  T(KEY_ONE)           \
  T(KEY_TWO)           \
  T(KEY_THREE)         \
  T(KEY_FOUR)          \
  T(KEY_FIVE)          \
  T(KEY_SIX)           \
  T(KEY_SEVEN)         \
  T(KEY_EIGHT)         \
  T(KEY_NINE)          \
  T(KEY_A)             \
  T(KEY_B)             \
  T(KEY_C)             \
  T(KEY_D)             \
  T(KEY_E)             \
  T(KEY_F)             \
  T(KEY_G)             \
  T(KEY_H)             \
  T(KEY_I)             \
  T(KEY_J)             \
  T(KEY_K)             \
  T(KEY_L)             \
  T(KEY_M)             \
  T(KEY_N)             \
  T(KEY_O)             \
  T(KEY_P)             \
  T(KEY_Q)             \
  T(KEY_R)             \
  T(KEY_S)             \
  T(KEY_T)             \
  T(KEY_U)             \
  T(KEY_V)             \
  T(KEY_W)             \
  T(KEY_X)             \
  T(KEY_Y)             \
  T(KEY_Z)             \
  T(KEY_SPACE)         \
  T(KEY_ESCAPE)        \
  T(KEY_ENTER)         \
  T(KEY_TAB)           \
  T(KEY_BACKSPACE)     \
  T(KEY_INSERT)        \
  T(KEY_DELETE)        \
  T(KEY_RIGHT)         \
  T(KEY_LEFT)          \
  T(KEY_DOWN)          \
  T(KEY_UP)            \
  T(KEY_PAGE_UP)       \
  T(KEY_PAGE_DOWN)     \
  T(KEY_HOME)          \
  T(KEY_END)           \
  T(KEY_CAPS_LOCK)     \
  T(KEY_SCROLL_LOCK)   \
  T(KEY_NUM_LOCK)      \
  T(KEY_PRINT_SCREEN)  \
  T(KEY_F1)            \
  T(KEY_F2)            \
  T(KEY_F3)            \
  T(KEY_F4)            \
  T(KEY_F5)            \
  T(KEY_F6)            \
  T(KEY_F7)            \
  T(KEY_F8)            \
  T(KEY_F9)            \
  T(KEY_F10)           \
  T(KEY_F11)           \
  T(KEY_F12)           \
  T(KEY_LEFT_SHIFT)    \
  T(KEY_LEFT_CONTROL)  \
  T(KEY_LEFT_ALT)      \
  T(KEY_LEFT_SUPER)    \
  T(KEY_RIGHT_SHIFT)   \
  T(KEY_RIGHT_CONTROL) \
  T(KEY_RIGHT_ALT)     \
  T(KEY_RIGHT_SUPER)   \
  T(KEY_KB_MENU)       \
  T(KEY_KP_0)          \
  T(KEY_KP_1)          \
  T(KEY_KP_2)          \
  T(KEY_KP_3)          \
  T(KEY_KP_4)          \
  T(KEY_KP_5)          \
  T(KEY_KP_6)          \
  T(KEY_KP_7)          \
  T(KEY_KP_8)          \
  T(KEY_KP_9)          \
  T(KEY_KP_DECIMAL)    \
  T(KEY_KP_DIVIDE)     \
  T(KEY_KP_MULTIPLY)   \
  T(KEY_KP_SUBTRACT)   \
  T(KEY_KP_ADD)        \
  T(KEY_KP_ENTER)      \
  T(KEY_KP_EQUAL)      \
  T(KEY_MOUSE_LEFT)    \
  T(KEY_MOUSE_MIDDLE)  \
  T(KEY_MOUSE_RIGHT)

#define DECLARE_EKEYCODE(Key) Key,
enum EKeyCode : uint8 { XMACRO_KEYS(DECLARE_EKEYCODE) KEY_MAX };
