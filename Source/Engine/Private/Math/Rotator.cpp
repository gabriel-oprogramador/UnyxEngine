#include "Math/Rotator.h"
#include "Math/Quaternion.h"
#include "Math/Vector3.h"

FRotator::FRotator(const FQuat& Quaternion) {
  *this = Quaternion.ToRotator();
}

FQuat FRotator::ToQuat() const {
  FQuat qYaw = FQuat::FromAxisAngle(FVector3::GetUp(), yaw);
  FQuat qPitch = FQuat::FromLocalAxis(qYaw, FVector3::GetRight(), pitch);
  FQuat qRoll = FQuat::FromLocalAxis(qYaw * qPitch, FVector3::GetForward(), -roll);
  return qYaw * qPitch * qRoll;
}
