#pragma once
#include "Core/Name.h"
#include "Core/Reflection.h"
#include "Runtime/Schedule.h"

struct FType;

struct ENGINE_API FModule {
  FModule(const FName& Name) : name(Name) {}

  template<typename T>
  void AddType() {
    FType* type = TStaticType<T>::GetType();
    switch(type->kind) {
      case ETypeKind::Struct: {
        types.Add(type);
        break;
      }
      case ETypeKind::Component: {
        components.Add(type);
        break;
      }
      case ETypeKind::Primitive: {
        types.Add(type);
        break;
      }
    }
  }

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

  FName GetName() {
    return name;
  }

private:
  friend struct FApp;
  FSchedule schedule{};
  FName name{};
  TArray<FType*> types{};
  TArray<FType*> components{};
};
