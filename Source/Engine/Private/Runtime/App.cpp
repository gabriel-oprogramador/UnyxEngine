#include "Platform/Platform.h"
#include "Runtime/Engine.h"
#include "Runtime/World.h"
#include "Runtime/GameInstance.h"

GAME_API void OnGameLoad(FGameInstance& Game);
GAME_API void OnGameUnload();

struct FApp {
  static inline FSchedule Schedule = {};
  static inline FGameInstance Game = {Schedule};
  static inline FWorld World = {};
  static void Initialize() {
    UE_ALERT("App Initialize");
    GEngine.Initialize();
    OnGameLoad(Game);
    Platform::WindowInit(800, 450, "Unyx Engine");
    Platform::WindowSetIcon("AppIcon.png");
    World.BeginPlay(Schedule);
  }

  static void Terminate() {
    World.EndPlay(Schedule);
    GEngine.Terminate();
    UE_ALERT("App Terminate");
  }

  static void Update() {
    GEngine.Update();
    World.Update(Schedule);
    GEngine.Render();
  }
};

void AppBootstrapInitialize() {
  FApp::Initialize();
}

void AppBootstrapTerminate() {
  FApp::Terminate();
}

void AppBootstrapUpdate() {
  FApp::Update();
}
