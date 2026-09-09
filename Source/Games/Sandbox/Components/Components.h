#pragma once
#include <CoreMinimal.h>

DECLARE_STRUCT(FCenter)
struct FCenter {
  FVector3 center{};
  REFLECT(Type) {
    Type.Property<&FCenter::center>("LocalCenter");
  }
};

DECLARE_STRUCT(FSpringArm)
struct FSpringArm {
  FVector3 target{};
  FVector3 offset{};
  FCenter center{};
  REFLECT(Type) {
    Type.Property<&FSpringArm::target>("Target");
    Type.Property<&FSpringArm::offset>("Offset");
    Type.Property<&FSpringArm::center>("Center");
  }
};

DECLARE_COMPONENT(UCamera)
struct UCamera {
  float fov{60};
  uint32 layer{33};
  FVector3 target{};
  FName tag{"MainCamera"};
  bool bActivate{false};
  FSpringArm springArm{};

  REFLECT(Type) {
    Type.Property<&UCamera::fov>("Fov");
    Type.Property<&UCamera::layer>("Layer");
    Type.Property<&UCamera::target>("Target");
    Type.Property<&UCamera::tag>("Tag");
    Type.Property<&UCamera::bActivate>("bActivate");
    Type.Property<&UCamera::springArm>("SpringArm");
  }
};
