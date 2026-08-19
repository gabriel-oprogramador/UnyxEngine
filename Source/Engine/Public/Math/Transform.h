#pragma once
#include "Math/Utils.h"
#include "Math/Vector3.h"
#include "Math/Quaternion.h"

struct FTransform {
  FVector3 location;
  FVector3 scale;
  FQuat rotation;

  FTransform() : location(0.0f), rotation(FQuat::GetIdentity()), scale(1.0f) {}
  FTransform(const FVector3& Location, const FQuat& Rotation, const FVector3& Scale) : location(Location), rotation(Rotation), scale(Scale) {}
};
