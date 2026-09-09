#include <CoreMinimal.h>
#include <Renderer/RHI.h>
#include <Platform/Event.h>
#include <Platform/Platform.h>

struct FApp {
  FWorld editorWorld{};

  void Initialize() {
    Platform::WindowInitOpenGL();
    Platform::WindowInit(1280, 720, "Unyx " TARGET_NAME);
    RHI::ClearColor(FColors::SKY);
  }

  void Terminate() {
    Platform::WindowTerm();
  }

  void Update() {
    FTime::Update();
    RHI::Clear();
    Platform::WindowPollEvent();
    ProcessEvents();
    Platform::WindowSwapBuffers();
  }
  void ProcessEvents() {
    PEvent event = {};
    while(Platform::NextEvent(event)) {
      switch(event.type) {
        case PEventType::WindowResize: {
          uint32 width = event.windowResize.width;
          uint32 height = event.windowResize.height;
          RHI::SetViewport(FViewport{0, 0, width, height});
          break;
        }
        default: return;
      }
    }
  }
};

int main() {
  UE_INFO("Running Unyx Editor");
  FApp app = {};
  app.Initialize();
  while(!Platform::WindowShouldClose()) {
    app.Update();
  }
  app.Terminate();
  return 0;
}
