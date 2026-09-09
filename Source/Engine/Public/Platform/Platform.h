#pragma once
#include "Core/BaseTypes.h"
#include "Core/Log.h"

#include <cstdarg>

typedef struct PKey {
  EKeyCode keyCode;
  cstring physicalName;
  cstring layoutName;
  bool bIsControlKey;
} PKey;

namespace Platform {
  ENGINE_API uint32 WindowInitOpenGL();
  ENGINE_API void WindowInit(uint32 Width, uint32 Height, cstring Title);
  ENGINE_API void WindowTerm();
  ENGINE_API void WindowPollEvent();
  ENGINE_API void WindowSwapBuffers();
  ENGINE_API void WindowClose();
  ENGINE_API bool WindowShouldClose();
  ENGINE_API bool WindowIsFullcreen();
  ENGINE_API void WindowSetFullscreen(bool bFullscreen);
  ENGINE_API bool WindowIsMouseCaptured();
  ENGINE_API void WindowCaptureMouse(bool bCapture);
  ENGINE_API void WindowGetSize(uint32& OutWidth, uint32& OutHeight);
  ENGINE_API void WindowSetIcon(cstring Path);

  ENGINE_API double TimeGetNow();

  ENGINE_API void* MemAlloc(uint64 Size, uint64 Align);
  ENGINE_API void MemFree(void* Data);

  ENGINE_API void LogPrint(ELogLevel Level, cstring FuncName, cstring Context, cstring Format, va_list Args);

}  // namespace Platform
