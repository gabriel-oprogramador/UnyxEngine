#include "Systems.h"
#include <Renderer/RHI.h>

void FSetupSystem::BeginPlay(FWorld& World) {
  UE_ALERT("FSetupSystem::BeginPlay()");
}

void FSetupSystem::Update(FWorld& World) {
  float time = FTime::Time();
  float speed = 0.5f;
  float angle = time * speed;
  float r = (sinf(angle) + 1.0f) / 2.0f;
  float g = (sinf(angle + 2.094f) + 1.0f) / 2.0f;
  float b = (sinf(angle + 4.188f) + 1.0f) / 2.0f;
  RHI::ClearColor(FColor::Make(r, g, b, 1.f));

  if(FInput::IsKeyPressed(EKeyCode::KEY_ESCAPE)) {
    FGame::QuitGame();
  }

  if(FInput::IsKeyPressed(EKeyCode::KEY_F)) {
    FGame::ToggleFullscreen();
  }

  if(FInput::IsKeyPressed(EKeyCode::KEY_P)) {
    FInput::ToggleCaptureMouse();
  }
}
