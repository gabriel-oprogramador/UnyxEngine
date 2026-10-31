#include <CoreMinimal.h>
#include <Renderer/RHI.h>
#include <Platform/Event.h>
#include <Platform/Platform.h>

struct FEditor {
  static uint32 WindowWidth();
  static uint32 WindowHeight();
  static FVector2 WindowSize();

  static FInput& GetInput() {
    return Input;
  }

  static FTime& GetTime() {
    return Time;
  }

private:
  friend struct FApp;
  static inline FInput Input;
  static inline FTime Time;
};

struct FApp {
  FWorld editorWorld{};
  FInput& editorInput = FEditor::GetInput();
  FInput& gameInput = FGame::GetInput();

  void Initialize() {
    Platform::WindowInitOpenGL();
    Platform::WindowInit(1280, 720, "Unyx " TARGET_NAME);
    RHI::ClearColor(FColors::SKY);
  }

  void Terminate() {
    Platform::WindowTerm();
  }

  void Update() {
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
        default: {
          break;
        }
      }
    }
  }
};

int main(int argc, const char** argv) {
  UE_INFO("Running Unyx Editor");

  for(uint32 c = 0; c < argc; c++) {
    UE_INFO("Arg:%d => %s", c, argv[c]);
  }

  FApp app = {};
  app.Initialize();
  while(!Platform::WindowShouldClose()) {
    app.Update();
  }
  app.Terminate();
  return 0;
}
