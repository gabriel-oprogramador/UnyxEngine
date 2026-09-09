#pragma once
#include "Math/Utils.h"
#include "Core/Reflection.h"

DECLARE_STRUCT(FVector3)
struct ENGINE_API FVector3 {
  float x;
  float y;
  float z;

  REFLECT(Type) {
    Type.Property<&FVector3::x>("X");
    Type.Property<&FVector3::y>("Y");
    Type.Property<&FVector3::z>("Z");
  }

  FVector3() = default;
  constexpr explicit FVector3(float Value) : x(Value), y(Value), z(Value) {}
  constexpr FVector3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}

  constexpr bool operator==(const FVector3& Other) const {
    return x == Other.x && y == Other.y && z == Other.z;
  }

  constexpr bool operator!=(const FVector3& Other) const {
    return !(*this == Other);
  }

  constexpr FVector3 operator-() const {
    return FVector3(-x, -y, -z);
  }

  constexpr FVector3 operator+(const FVector3& Other) const {
    return FVector3(x + Other.x, y + Other.y, z + Other.z);
  }

  constexpr FVector3 operator-(const FVector3& Other) const {
    return FVector3(x - Other.x, y - Other.y, z - Other.z);
  }

  constexpr FVector3 operator*(const FVector3& Other) const {
    return FVector3(x * Other.x, y * Other.y, z * Other.z);
  }

  constexpr FVector3 operator/(const FVector3& Other) const {
    return FVector3(x / Other.x, y / Other.y, z / Other.z);
  }

  constexpr FVector3 operator*(float Value) const {
    return FVector3(x * Value, y * Value, z * Value);
  }

  constexpr FVector3 operator/(float Value) const {
    return FVector3(x / Value, y / Value, z / Value);
  }

  constexpr FVector3& operator+=(const FVector3& Other) {
    x += Other.x;
    y += Other.y;
    z += Other.z;
    return *this;
  }

  constexpr FVector3& operator-=(const FVector3& Other) {
    x -= Other.x;
    y -= Other.y;
    z -= Other.z;
    return *this;
  }

  constexpr FVector3& operator*=(const FVector3& Other) {
    x *= Other.x;
    y *= Other.y;
    z *= Other.z;
    return *this;
  }

  constexpr FVector3& operator/=(const FVector3& Other) {
    x /= Other.x;
    y /= Other.y;
    z /= Other.z;
    return *this;
  }

  constexpr FVector3& operator*=(float Value) {
    x *= Value;
    y *= Value;
    z *= Value;
    return *this;
  }

  constexpr FVector3& operator/=(float Value) {
    x /= Value;
    y /= Value;
    z /= Value;
    return *this;
  }

  friend constexpr FVector3 operator*(float Value, const FVector3& Vector) {
    return Vector * Value;
  }

  bool NearlyEquals(const FVector3& Other, float Tolerance = FMath::Epsilon) const {
    return FMath::IsNearlyEqual(x, Other.x, Tolerance) && FMath::IsNearlyEqual(y, Other.y, Tolerance) && FMath::IsNearlyEqual(z, Other.z, Tolerance);
  }

  constexpr float LengthSquared() const {
    return x * x + y * y + z * z;
  }

  float Length() const {
    return FMath::Sqrt(LengthSquared());
  }

  constexpr float Dot(const FVector3& Other) const {
    return x * Other.x + y * Other.y + z * Other.z;
  }

  constexpr FVector3 Cross(const FVector3& Other) const {
    return FVector3(y * Other.z - z * Other.y, z * Other.x - x * Other.z, x * Other.y - y * Other.x);
  }

  void Normalize() {
    float Len = Length();
    if(Len > FMath::Epsilon) {
      x /= Len;
      y /= Len;
      z /= Len;
    } else {
      x = 0.0f;
      y = 0.0f;
      z = 0.0f;
    }
  }

  FVector3 GetNormalized() const {
    float Len = Length();
    if(Len > FMath::Epsilon) {
      return FVector3(x / Len, y / Len, z / Len);
    }
    return FVector3(0.0f);
  }

  float Distance(const FVector3& Other) const {
    return (*this - Other).Length();
  }

  constexpr float DistanceSquared(const FVector3& Other) const {
    return (*this - Other).LengthSquared();
  }

  cstring Printf() {
    thread_local char Buffers[4][128];
    thread_local uint32 Index = 0;
    char* buffer = Buffers[Index];
    Index = (Index + 1) & 3;
    snprintf(buffer, sizeof(Buffers[0]), "X:%.2f, Y:%.2f, Z:%.2f", x, y, z);
    return buffer;
  }

  static constexpr FVector3 GetRight() {
    return FVector3(1.f, 0.f, 0.f);
  }
  static constexpr FVector3 GetForward() {
    return FVector3(0.f, 0.f, -1.f);
  }
  static constexpr FVector3 GetUp() {
    return FVector3(0.f, 1.f, 0.f);
  }
};
