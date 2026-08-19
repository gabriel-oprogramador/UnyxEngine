#include "Runtime/World.h"
#include "Runtime/Schedule.h"
#include "Platform/Platform.h"
#include "Math/Math.h"

FVector2 FWorld::GetWindowSize() {
  uint32 width = 0, height = 0;
  Platform::WindowGetSize(width, height);
  return FVector2{static_cast<float>(width), static_cast<float>(height)};
}

bool FWorld::IsFullscreen() {
  return Platform::WindowIsFullcreen();
}

void FWorld::SetFullscreen(bool bFullscreen) {
  Platform::WindowSetFullscreen(bFullscreen);
}

void FWorld::QuitGame() {
  Platform::WindowClose();
}

void FWorld::Initialize() {}

void FWorld::Terminate() {}

void FWorld::BeginPlay(const FSchedule& Schedule) {
  Schedule.RunStage(FSchedule::EStage::BeginPlay, *this);
}

void FWorld::EndPlay(const FSchedule& Schedule) {
  Schedule.RunStage(FSchedule::EStage::EndPlay, *this);
}

void FWorld::Update(const FSchedule& Schedule) {
  Schedule.RunStage(FSchedule::EStage::Update, *this);
}
