#include "GameFramework/Time.h"
#include "Runtime/Engine.h"

float FTime::DeltaTime() {
  return GEngine.GetTimeContext().deltaTime;
}

uint32 FTime::Framerate() {
  return GEngine.GetTimeContext().framerate;
}
