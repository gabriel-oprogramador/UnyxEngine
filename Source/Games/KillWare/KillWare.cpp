#include <CoreMinimal.h>

DECLARE_COMPONENT(UPlayerState)
struct UPlayerState {
  float health{100.f};
  uint32 score{0};
  FVector3 location{};
  REFLECT(Type) {
    Type.Property<&UPlayerState::health>("Health");
    Type.Property<&UPlayerState::score>("Score");
    Type.Property<&UPlayerState::location>("Location");
  }
};

GAME_API void OnGameLoad(FModule& Module) {
  Module.AddComponent<UPlayerState>();
}

GAME_API void OnGameUnload() {}
