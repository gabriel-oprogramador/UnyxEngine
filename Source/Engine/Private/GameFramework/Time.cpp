#include "GameFramework/Time.h"
#include "Platform/Platform.h"

double FTime::Time() {
  return Platform::TimeGetNow();
}

void FTime::Update() {
  static double lastTime = Platform::TimeGetNow();
  static double smoothFPS = 60.0;
  constexpr double Alpha = 0.1;
  const double current = Platform::TimeGetNow();
  const double delta = current - lastTime;

  lastTime = current;
  if(delta > 0.0) {
    const double fps = 1.0 / delta;
    smoothFPS = Alpha * fps + (1.0 - Alpha) * smoothFPS;
  }
  deltaTime = delta;
  framerate = static_cast<uint32>(smoothFPS);
}
