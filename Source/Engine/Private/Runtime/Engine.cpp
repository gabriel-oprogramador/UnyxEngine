#include "Runtime/Engine.h"
#include "Renderer/Renderer.h"
#include "Renderer/RHI.h"
#include "Platform/Platform.h"
#include "Platform/Event.h"
#include "Core/Memory.h"

FEngine GEngine;

static void InternalProcessEvents();

void FEngine::Initialize() {
  renderer = new FRenderer;
}

void FEngine::Terminate() {
  delete renderer;
}

void FEngine::Update() {
  static double lastTime = Platform::TimeGetNow();
  static double smoothFPS = 60.0;
  constexpr double Alpha = 0.1;
  const double current = Platform::TimeGetNow();
  const double deltaTime = current - lastTime;

  lastTime = current;
  if(deltaTime > 0.0) {
    const double fps = 1.0 / deltaTime;
    smoothFPS = Alpha * fps + (1.0 - Alpha) * smoothFPS;
  }
  GEngine.timeContext.deltaTime = deltaTime;
  GEngine.timeContext.framerate = static_cast<uint32>(smoothFPS);

  FMemory::CopyBytes(&GEngine.inputContext.previousState, &GEngine.inputContext.currentState, sizeof(FInputState));
  GEngine.inputContext.currentState.mouseDelta = FVector2{0};
  GEngine.inputContext.currentState.mouseScroll = FVector2{0};

  InternalProcessEvents();
  renderer->BeginFrame();
}

void FEngine::Render() {
  renderer->EndFrame();
}

void InternalProcessEvents() {
  PEvent event{};
  while(Platform::NextEvent(event)) {
    switch(event.type) {
      case PEventType::WindowFocus: {
        break;
      }
      case PEventType::InputMap: {
        break;
      }
      case PEventType::InputKey: {
        FInputContext& ctx = GEngine.GetInputContext();
        ctx.currentState.keys[event.inputKey.keyCode] = event.inputKey.bState;
        break;
      }
      case PEventType::MousePos: {
        FInputContext& ctx = GEngine.GetInputContext();
        ctx.currentState.mousePos = FVector2{(float)event.mousePos.posX, (float)event.mousePos.posY};
        break;
      }
      case PEventType::MouseDelta: {
        FInputContext& ctx = GEngine.GetInputContext();
        ctx.currentState.mouseDelta += FVector2{(float)event.mouseDelta.deltaX, (float)event.mouseDelta.deltaY};
        break;
      }
      case PEventType::MouseScroll: {
        FInputContext& ctx = GEngine.GetInputContext();
        ctx.currentState.mouseScroll += FVector2{(float)event.mouseScroll.scrollX, (float)event.mouseScroll.scrollY};
        break;
      }
      case PEventType::WindowResize: {
        uint32 width = event.windowResize.width;
        uint32 heght = event.windowResize.height;
        RHI::SetViewport(FViewport{0, 0, width, heght});
        UE_ALERT("Window Resize(%ux%u)", width, heght);
        break;
      }
    }
  }
}
