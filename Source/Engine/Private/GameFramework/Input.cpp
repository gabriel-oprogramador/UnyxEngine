#include "GameFramework/Input.h"
#include "Platform/Platform.h"
#include "Runtime/Engine.h"

#define GENERATE_FKEY_LIST(Key) FKey{#Key, Key},

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
  FInputContext& ctx = GEngine.GetInputContext();
  return (ctx.currentState.keys[KeyCode] && !ctx.previousState.keys[KeyCode]);
}

bool FInput::IsKeyReleased(EKeyCode KeyCode) {
  FInputContext& ctx = GEngine.GetInputContext();
  return (!ctx.currentState.keys[KeyCode] && ctx.previousState.keys[KeyCode]);
}

bool FInput::IsKeyRepeat(EKeyCode KeyCode) {
  FInputContext& ctx = GEngine.GetInputContext();
  return (ctx.currentState.keys[KeyCode] && ctx.previousState.keys[KeyCode]);
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
  return GEngine.GetInputContext().currentState.mousePos;
}

FVector2 FInput::GetMouseDelta() {
  return GEngine.GetInputContext().currentState.mouseDelta;
}

FVector2 FInput::GetMouseScroll() {
  return GEngine.GetInputContext().currentState.mouseScroll;
}
