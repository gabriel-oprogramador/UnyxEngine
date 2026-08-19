#pragma once
#include <CoreMinimal.h>

struct UTransform {
  FTransform world;
};

struct UCamera3D {
  float fov{60.f};
  FVector3 offset{};
  FVector3 target{};
  FMatrix4 viewMatrix{};
};
