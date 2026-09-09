#pragma once
#include "Core/Core.h"

#include <type_traits>

struct FWorld;

struct FSystem {
  void (*OnExecute)(FWorld&);
};

namespace ARC {
  template<typename T>
  struct TFunctionTraits;
  template<typename R, typename Arg>
  struct TFunctionTraits<R (*)(Arg)> {
    using ReturnType = R;
    using ArgType = Arg;
    static constexpr bool IsVoid = std::is_same_v<R, void>;
    static constexpr bool HasWorld = std::is_same_v<std::remove_cv_t<std::remove_reference_t<Arg>>, FWorld>;
    static constexpr bool IsReference = std::is_lvalue_reference_v<Arg>;
    static constexpr bool Valid = IsVoid && HasWorld && IsReference;
  };

  template<typename T, typename = void>
  struct THasBeginPlay : std::false_type {};
  template<typename T>
  struct THasBeginPlay<T, std::void_t<decltype(&T::BeginPlay)>> : std::true_type {};
  template<typename T, typename = void>
  struct THasEndPlay : std::false_type {};
  template<typename T>
  struct THasEndPlay<T, std::void_t<decltype(&T::EndPlay)>> : std::true_type {};
  template<typename T, typename = void>
  struct THasUpdate : std::false_type {};
  template<typename T>
  struct THasUpdate<T, std::void_t<decltype(&T::Update)>> : std::true_type {};
}  // namespace ARC

struct ENGINE_API FSchedule {
  enum class EStage {
    BeginPlay,  //
    EndPlay,    //
    Update,     //
  };

  void RegisterSystem(EStage Stage, FSystem System);
  void RunStage(EStage Stage, FWorld& World) const;

private:
  TArray<FSystem> sysBeginPlay;
  TArray<FSystem> sysEndPlay;
  TArray<FSystem> sysUpdate;
};
