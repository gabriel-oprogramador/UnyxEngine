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
  void WindowInit(uint32 Width, uint32 Height, cstring Title);
  void WindowClose();
  bool WindowShouldClose();
  bool WindowIsFullcreen();
  void WindowSetFullscreen(bool bFullscreen);
  bool WindowIsMouseCaptured();
  void WindowCaptureMouse(bool bCapture);
  void WindowGetSize(uint32& OutWidth, uint32& OutHeight);
  void WindowSetIcon(cstring Path);

  float TimeGetNow();

  void* MemAlloc(uint64 Size, uint64 Align);
  void MemFree(void* Data);

  uint32 GraphicInitOpenGL();
  void LogPrint(ELogLevel Level, cstring FuncName, cstring Context, cstring Format, va_list Args);

}  // namespace Platform
