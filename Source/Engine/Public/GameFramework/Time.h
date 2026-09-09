#pragma once
#include "Core/Core.h"

struct ENGINE_API FTime {
  static double Time();

  static float DeltaTime() {
    return deltaTime;
  }
  static uint32 Framerate() {
    return framerate;
  }

private:
  friend struct FApp;
  static inline float deltaTime = {0.f};
  static inline uint32 framerate = 60;

  static void Update();
};
