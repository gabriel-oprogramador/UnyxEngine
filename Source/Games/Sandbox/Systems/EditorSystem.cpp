#include "Systems.h"
#include <GameFramework/Input.h>
#include <GameFramework/Time.h>

void FEditorSystem::BeginPlay(FWorld& World) {
  UE_ALERT("FEditorSystem::BeginPlay");
}

void FEditorSystem::EndPlay(FWorld& World) {
  UE_ALERT("FEditorSystem::EndPlay");
}

void FEditorSystem::Update(FWorld& World) {
  if(FInput::IsKeyRepeat(EKeyCode::KEY_ENTER)) {
    FVector2 windowSize = World.GetWindowSize();
    UE_ALERT("Window Size:(%s)", windowSize.Printf());
  }

  if(FInput::IsKeyPressed(EKeyCode::KEY_F)) {
    World.SetFullscreen(!World.IsFullscreen());
  }
  if(FInput::IsKeyPressed(KEY_ESCAPE)) {
    World.QuitGame();
  }
}
