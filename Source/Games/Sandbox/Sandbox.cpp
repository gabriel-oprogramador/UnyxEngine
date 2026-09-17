#include <CoreMinimal.h>
#include "Components/Components.h"
#include "Systems/Systems.h"

DECLARE_STRUCT(FWeaponState)
struct FWeaponState {
  float range{512.f};
  float damage{1.4};
  uint32 ammo{10};
  uint32 clips{2};
  REFLECT(Type) {
    Type.Property<&FWeaponState::ammo>("Ammo");
    Type.Property<&FWeaponState::clips>("Clips");
  }
};

DECLARE_COMPONENT(UPlayerState)
struct UPlayerState {
  float health{100.f};
  uint32 score{0};
  FVector3 velocity{};
  FWeaponState weapon{};

  REFLECT(Type) {
    Type.Property<&UPlayerState::health>("Health");
    Type.Property<&UPlayerState::score>("Score");
    Type.Property<&UPlayerState::velocity>("Velocity");
    Type.Property<&UPlayerState::weapon>("Weapon");
  }
};

// Example of the future API — still under development
//
// System declaration
// A system may or may not define stage functions.
// Stage functions must be static, return void,
// and take FWorld& as a parameter.
// These functions are optional; a system does not need to implement all of them.
//
// Systems are executed in the order they are registered in the Module.
struct FExampleSystem {
  static void BeginPlay(FWorld& World) {
    // Called once at the beginning, after the World has loaded a level.
    //World.Spawn(UCamera{}, UPlayerState{});
    //World.Spawn(UEnemyAI{}, UEnemyState{});
  }

  static void EndPlay(FWorld& World) {
    // Called once at the end, before the World unloads the level.
  }

  static void Update(FWorld& World) {
    // Called once per tick.
    //auto Query = World.Query<UTransform, UCamera>();
    //Query.Each([](UTransform& Transform, UCamera& Camera) {
    // Instructions executed for each Entity in this Archetype.
    //});
  }
};

struct FTestMap {
  static void BeginPlay(FWorld& World) {
    TMap<FName, FString> players = {
        {FName{"PlayerOne"}, "Gabriel"},  //
        {FName{"PlayerTwo"}, "Raphael"}   //
    };
    UE_INFO("Player: %u/%u", players.Count(), players.Capacity());
    UE_INFO("PlayerOne: %s", players.Find(FName{"PlayerOne"})->ToStr());
    UE_INFO("PlayerTwo: %s", players.Find(FName{"PlayerTwo"})->ToStr());

    players.Add(FName{"PlayerThree"}, "Kauan");
    players.Add(FName{"PlayerFour"}, "Miguel");
    players.Add(FName{"PlayerFive"}, "Mateus");

    UE_INFO("Player: %u/%u", players.Count(), players.Capacity());
    UE_INFO("PlayerOne: %s", players.Find(FName{"PlayerOne"})->ToStr());
    UE_INFO("PlayerTwo: %s", players.Find(FName{"PlayerTwo"})->ToStr());
    UE_INFO("PlayerThree: %s", players.Find(FName{"PlayerThree"})->ToStr());
    UE_INFO("PlayerFour: %s", players.Find(FName{"PlayerFour"})->ToStr());
    UE_INFO("PlayerFive: %s", players.Find(FName{"PlayerFive"})->ToStr());
  }
};

GAME_API void OnGameLoad(FModule& Module) {
  Module.AddSystem<FSetupSystem>();
  Module.AddSystem<FExampleSystem>();
  Module.AddComponent<UPlayerState>();
  Module.AddComponent<UCamera>();
  Module.AddSystem<FTestMap>();
}

GAME_API void OnGameUnload() {}
