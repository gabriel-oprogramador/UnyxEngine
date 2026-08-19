#include <CoreMinimal.h>
#include "Systems.h"

GAME_API void OnGameLoad(FGameInstance& Game) {
  Game.AddSystem<FSetupSystem>();
  Game.AddSystem<FEditorSystem>();
}

GAME_API void OnGameUnload() {}
