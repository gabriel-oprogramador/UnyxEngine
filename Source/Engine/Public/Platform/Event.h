#pragma once
#include "Core/BaseTypes.h"
#include "Platform.h"

struct PWindow;

enum class PEventType {
  WindowResize,  //
  WindowFocus,   //
  InputKey,      //
  InputMap,      //
  MousePos,      //
  MouseDelta,    //
  MouseScroll,   //
};

struct ENGINE_API PEvent {
  PEventType type;
  PWindow* target;

  union {
    struct {
      bool bFocused;
    } windowFocus;
    struct {
      uint32 width;
      uint32 height;
    } windowResize;
    struct {
      EKeyCode keyCode;
      bool bState;
    } inputKey;
    struct {
      PKey* map;
      uint8 count;
    } inputMap;
    struct {
      int32 posX;
      int32 posY;
    } mousePos;
    struct {
      float deltaX;
      float deltaY;
    } mouseDelta;
    struct {
      float scrollX;
      float scrollY;
    } mouseScroll;
  };
};

namespace Platform {
  bool ENGINE_API PushEvent(const PEvent& Event);
  bool ENGINE_API NextEvent(PEvent& OutEvent);
}  // namespace Platform
