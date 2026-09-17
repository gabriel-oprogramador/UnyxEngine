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

struct FGameRegisterType {
  template<typename T>
  static bool Register(cstring TypeName) {
    static FType type = FTypeOf::MakeType<T>(TypeName, ETypeKind::Component);
    AllTypes.Add(&type);
    return true;
  }
  static inline TArray<FType*> AllTypes;
};

static inline bool bReg = FGameRegisterType::Register<struct UCamera>("UCamera");

struct UCamera {
  float fov;
  float nearPlane;
  float farPlane;
  FVector3 target;
};

GAME_API void OnGameLoad(FModule& Module) {
  Module.AddComponent<UPlayerState>();
}

GAME_API void OnGameUnload() {}
