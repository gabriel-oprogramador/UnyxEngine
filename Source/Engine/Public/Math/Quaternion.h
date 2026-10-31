#pragma once
#include "Math/Utils.h"

struct FVector3;
struct FRotator;

struct ENGINE_API FQuat {
  float x;
  float y;
  float z;
  float w;

  constexpr FQuat() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

  constexpr FQuat(float X, float Y, float Z, float W) : x(X), y(Y), z(Z), w(W) {}

  explicit FQuat(const FRotator& Rotator);

  constexpr bool operator==(const FQuat& Other) const {
    return x == Other.x && y == Other.y && z == Other.z && w == Other.w;
  }

  constexpr bool operator!=(const FQuat& Other) const {
    return !(*this == Other);
  }

  constexpr FQuat operator*(const FQuat& Other) const {
    float nx = w * Other.x + x * Other.w + y * Other.z - z * Other.y;
    float ny = w * Other.y - x * Other.z + y * Other.w + z * Other.x;
    float nz = w * Other.z + x * Other.y - y * Other.x + z * Other.w;
    float nw = w * Other.w - x * Other.x - y * Other.y - z * Other.z;
    return FQuat(nx, ny, nz, nw);
  }
  constexpr FQuat operator-() const {
    return FQuat(-x, -y, -z, -w);
  }

  constexpr float LengthSquared() const {
    return x * x + y * y + z * z + w * w;
  }

  float Length() const {
    return FMath::Sqrt(LengthSquared());
  }

  void Normalize() {
    float Len = Length();
    if(Len > FMath::Epsilon) {
      x /= Len;
      y /= Len;
      z /= Len;
      w /= Len;
      return;
    }
    w = 1.0f;
    x = y = z = 0.0f;
  }

  FQuat GetNormalized() const {
    FQuat Result = *this;
    Result.Normalize();
    return Result;
  }

  constexpr FQuat Conjugate() const {
    return FQuat(-x, -y, -z, w);
  }

  FRotator ToRotator() const;
  FVector3 RotateVector(const FVector3& Vector) const;
  FVector3 GetForward();
  FVector3 GetRight();
  FVector3 GetUp();

  static FQuat FromAxisAngle(const FVector3& Axis, float AngleDeg);
  static FQuat FromLocalAxis(const FQuat& Rotation, const FVector3& LocalAxis, float AngleDeg);

  static FQuat GetIdentity() {
    return FQuat(0, 0, 0, 1);
  }
};
