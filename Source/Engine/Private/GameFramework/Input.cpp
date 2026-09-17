#include "GameFramework/Input.h"
#include "Platform/Platform.h"
#include "Platform/Event.h"
#include "Core/Memory.h"

#define GENERATE_FKEY_LIST(Key) FKey{FName{#Key}, Key},

static FKey SKeyList[] = {XMACRO_KEYS(GENERATE_FKEY_LIST)};

FKey FInput::GetKeyByName(FName Name) {
  for(uint32 c = 0; c < KEY_MAX; c++) {
    if(SKeyList[c].keyName == Name) {
      return SKeyList[c];
    }
  }
  return SKeyList[0];
}

FKey FInput::GetKeyByCode(EKeyCode KeyCode) {
  UE_ASSERT(KeyCode < KEY_MAX);
  return SKeyList[KeyCode];
}

bool FInput::IsKeyPressed(EKeyCode KeyCode) {
  return (currentState.keys[KeyCode] && !previousState.keys[KeyCode]);
}

bool FInput::IsKeyReleased(EKeyCode KeyCode) {
  return (!currentState.keys[KeyCode] && previousState.keys[KeyCode]);
}

bool FInput::IsKeyRepeat(EKeyCode KeyCode) {
  return (currentState.keys[KeyCode] && previousState.keys[KeyCode]);
}

bool FInput::IsMouseCaptured() {
  return Platform::WindowIsMouseCaptured();
}

void FInput::CaptureMouse(bool bCapture) {
  Platform::WindowCaptureMouse(bCapture);
}

void FInput::ToggleCaptureMouse() {
  Platform::WindowCaptureMouse(!Platform::WindowIsMouseCaptured());
}

FVector2 FInput::GetMousePos() {
  return currentState.mousePos;
}

FVector2 FInput::GetMouseDelta() {
  return currentState.mouseDelta;
}

FVector2 FInput::GetMouseScroll() {
  return currentState.mouseScroll;
}

void FInput::Update() {
  FMemory::CopyAssign(&previousState, &currentState, 1);
  currentState.mouseDelta = FVector2{};
  currentState.mouseScroll = FVector2{};
}

bool FInput::ProcessEvent(const PEvent& InputEvent) {
  switch(InputEvent.type) {
    case PEventType::InputMap: {
      break;
    }
    case PEventType::InputKey: {
      currentState.keys[InputEvent.inputKey.keyCode] = InputEvent.inputKey.bState;
      break;
    }
    case PEventType::MouseScroll: {
      float scrollX = InputEvent.mouseScroll.scrollX;
      float scrollY = InputEvent.mouseScroll.scrollY;
      currentState.mouseDelta = FVector2{scrollX, scrollY};
      break;
    }
    case PEventType::MouseDelta: {
      float deltaX = InputEvent.mouseDelta.deltaX;
      float deltaY = InputEvent.mouseDelta.deltaY;
      currentState.mouseDelta = FVector2{deltaX, deltaY};
      break;
    }
    case PEventType::MousePos: {
      float posX = InputEvent.mousePos.posX;
      float posY = InputEvent.mousePos.posY;
      currentState.mouseDelta = FVector2{posX, posY};
      break;
    }
    default: return false;
  }
  return true;
}
