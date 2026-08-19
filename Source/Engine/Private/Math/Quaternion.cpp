#include "Math/Quaternion.h"
#include "Math/Rotator.h"
#include "Math/Vector3.h"

FQuat::FQuat(const FRotator& Rotator) {
  *this = Rotator.ToQuat();
}

// Converts quaternion to Euler angles using:
// Yaw (Y) -> Pitch (X) -> Roll (Z)
// Coordinate system:
// Right = +X
// Up    = +Y
// Forward = -Z
// Quaternion rotation convention:
// q^-1 * v * q
FRotator FQuat::ToRotator() const {
  FQuat quat = this->GetNormalized();
  float w = quat.w;
  float x = quat.x;
  float y = quat.y;
  float z = quat.z;
  float pitch, yaw, roll;
  float siny_cosp = 2.0f * (w * y - z * x);
  float cosy_cosp = 1.0f - 2.0f * (y * y + x * x);
  yaw = atan2f(siny_cosp, cosy_cosp);
  float sinp = 2.0f * (w * x + y * z);
  if(fabsf(sinp) >= 1.0f) {
    pitch = copysignf(M_PI / 2.0f, sinp);  // +-90° se sinp ≈ ±1
  } else {
    pitch = asinf(sinp);
  }
  float sinr_cosp = 2.0f * (w * z - x * y);
  float cosr_cosp = 1.0f - 2.0f * (x * x + z * z);
  roll = atan2f(sinr_cosp, cosr_cosp);
  const float RAD_TO_DEG = FMath::RadToDeg;
  return FRotator(pitch * RAD_TO_DEG, yaw * RAD_TO_DEG, roll * RAD_TO_DEG);
}

FVector3 FQuat::RotateVector(const FVector3& Vector) const {
  FQuat qNorm = GetNormalized();
  FQuat qVec(Vector.x, Vector.y, Vector.z, 0.0f);
  FQuat qInv = qNorm.Conjugate();
  FQuat Result = qInv * qVec * qNorm;
  return FVector3(Result.x, Result.y, Result.z);
}

FVector3 FQuat::GetForward() {
  return RotateVector(FVector3::GetForward());
}

FVector3 FQuat::GetRight() {
  return RotateVector(FVector3::GetRight());
}

FVector3 FQuat::GetUp() {
  return RotateVector(FVector3::GetUp());
}

FQuat FQuat::FromAxisAngle(const FVector3& Axis, float AngleDeg) {
  FVector3 vec = Axis.GetNormalized();
  float HalfAngle = FMath::Degrees(AngleDeg) * 0.5f;
  float SinHalf = FMath::Sin(HalfAngle);
  float CosHalf = FMath::Cos(HalfAngle);
  return FQuat(vec.x * SinHalf, vec.y * SinHalf, vec.z * SinHalf, CosHalf);
}

FQuat FQuat::FromLocalAxis(const FQuat& Rotation, const FVector3& LocalAxis, float AngleDeg) {
  FVector3 Axis = Rotation.RotateVector(LocalAxis);
  return FQuat::FromAxisAngle(Axis, AngleDeg);
}
