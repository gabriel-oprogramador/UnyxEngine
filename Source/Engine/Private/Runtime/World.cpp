#include "Runtime/World.h"
#include "Runtime/Schedule.h"

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
