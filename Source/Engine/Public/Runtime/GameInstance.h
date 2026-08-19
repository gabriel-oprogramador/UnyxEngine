#pragma once
#include "Runtime/Schedule.h"

struct FGameInstance {
  template<typename TSystem>
  void AddSystem() {
    if constexpr(ARC::THasBeginPlay<TSystem>::value) {
      using TFunction = decltype(&TSystem::BeginPlay);
      static_assert(ARC::TFunctionTraits<TFunction>::Valid, "Expected: static void BeginPlay(FWorld&)");
      FSystem sys{};
      sys.OnExecute = [](FWorld& World) { TSystem::BeginPlay(World); };
      schedule.RegisterSystem(FSchedule::EStage::BeginPlay, sys);
    }
    if constexpr(ARC::THasEndPlay<TSystem>::value) {
      using TFunction = decltype(&TSystem::EndPlay);
      static_assert(ARC::TFunctionTraits<TFunction>::Valid, "Expected: static void EndPlay(FWorld&)");
      FSystem sys{};
      sys.OnExecute = [](FWorld& World) { TSystem::EndPlay(World); };
      schedule.RegisterSystem(FSchedule::EStage::EndPlay, sys);
    }
    if constexpr(ARC::THasUpdate<TSystem>::value) {
      using TFunction = decltype(&TSystem::Update);
      static_assert(ARC::TFunctionTraits<TFunction>::Valid, "Expected: static void Update(FWorld&)");
      FSystem sys{};
      sys.OnExecute = [](FWorld& World) { TSystem::Update(World); };
      schedule.RegisterSystem(FSchedule::EStage::Update, sys);
    }
  }

private:
  friend class FApp;
  FGameInstance(FSchedule& Schedule) : schedule(Schedule) {}
  FSchedule& schedule;
  uint32 width{800};
  uint32 height{450};
};
