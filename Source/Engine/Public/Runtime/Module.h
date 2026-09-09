#pragma once

#include "Runtime/Schedule.h"
#include "Core/Reflection.h"

struct ENGINE_API FModule {
  template<typename TSystem>
  void AddSystem() {
    if constexpr(ARC::THasBeginPlay<TSystem>::value) {
      using TFunction = decltype(&TSystem::BeginPlay);
      static_assert(ARC::TFunctionTraits<TFunction>::Valid, "Expected: static void BeginPlay(FWorld&)");
      FSystem sys{};
      sys.OnExecute = &TSystem::BeginPlay;
      schedule.RegisterSystem(FSchedule::EStage::BeginPlay, sys);
    }
    if constexpr(ARC::THasEndPlay<TSystem>::value) {
      using TFunction = decltype(&TSystem::EndPlay);
      static_assert(ARC::TFunctionTraits<TFunction>::Valid, "Expected: static void EndPlay(FWorld&)");
      FSystem sys{};
      sys.OnExecute = TSystem::EndPlay;
      schedule.RegisterSystem(FSchedule::EStage::EndPlay, sys);
    }
    if constexpr(ARC::THasUpdate<TSystem>::value) {
      using TFunction = decltype(&TSystem::Update);
      static_assert(ARC::TFunctionTraits<TFunction>::Valid, "Expected: static void Update(FWorld&)");
      FSystem sys{};
      sys.OnExecute = TSystem::Update;
      schedule.RegisterSystem(FSchedule::EStage::Update, sys);
    }
  }

  template<typename TComponent>
  void AddComponent() {
    FType* type = TStaticType<TComponent>::GetType();
    componentsList.Add(type);
  }

private:
  void RegisterModule(const FName& Name) {
    FTypeOf::RegisterComponents(FName{"Game"}, componentsList);
  }

  friend struct FApp;
  TArray<FType*> componentsList{};
  FSchedule schedule{};
};
