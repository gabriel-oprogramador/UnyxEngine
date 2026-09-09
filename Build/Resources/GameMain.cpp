#include <CoreMinimal.h>
#include <Renderer/RHI.h>
#include <Platform/Event.h>
#include <Platform/Platform.h>

GAME_API void OnGameLoad(FModule& Module);
GAME_API void OnGameUnload();

struct FApp final {
  FModule game = {};
  FWorld world = {};

  void Initialize() {
    Platform::WindowInitOpenGL();
    Platform::WindowInit(800, 450, TARGET_NAME);
    OnGameLoad(game);
    game.RegisterModule(FName{"Game"});
    world.Initialize();
    world.BeginPlay(game.schedule);
  }

  void Terminate() {
    world.EndPlay(game.schedule);
    world.Terminate();
    OnGameUnload();
    Platform::WindowTerm();
  }

  void Update() {
    FTime::Update();
    FInput::Update();
    RHI::Clear();
    Platform::WindowPollEvent();
    ProcessEvents();
    world.Update(game.schedule);
    Platform::WindowSwapBuffers();
  }

  void ProcessEvents() {
    PEvent event = {};
    while(Platform::NextEvent(event)) {
      switch(event.type) {
        case PEventType::InputMap:
        case PEventType::InputKey:
        case PEventType::MousePos:
        case PEventType::MouseDelta:
        case PEventType::MouseScroll: {
          FInput::ProcessEvent(event);
          break;
        }
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

#if defined(PLATFORM_WINDOWS) || defined(PLATFORM_LINUX)

int main() {
  UE_INFO("Running Game Standalone");
  static FApp app = {};
  app.Initialize();
  while(!Platform::WindowShouldClose()) {
    app.Update();
  }
  app.Terminate();
  return 0;
}

#elif defined(PLATFORM_WEB)
#include <emscripten.h>

int main() {
  UE_INFO("Running Game Standalone");
  static FApp app = {};
  app.Initialize();
  auto MainLoop = []() {
    if(Platform::WindowShouldClose()) {
      app.Terminate();
      emscripten_cancel_main_loop();
    }
    app.Update();
  };
  emscripten_set_main_loop(MainLoop, 0, true);
  return 0;
}
#endif
