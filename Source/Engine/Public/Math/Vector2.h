#pragma once
#include "Math/Utils.h"

struct FVector2 {
  float x;
  float y;

  FVector2() = default;
  constexpr explicit FVector2(float Value) : x(Value), y(Value) {}
  constexpr FVector2(float X, float Y) : x(X), y(Y) {}

  constexpr bool operator==(const FVector2& Other) const {
    return (x == Other.x && y == Other.y);
  }
  constexpr bool operator!=(const FVector2& Other) const {
    return !(*this == Other);
  }

  constexpr FVector2 operator-() const {
    return FVector2(-x, -y);
  }

  constexpr FVector2 operator+(const FVector2& Other) const {
    return FVector2(x + Other.x, y + Other.y);
  }
  constexpr FVector2 operator-(const FVector2& Other) const {
    return FVector2(x - Other.x, y - Other.y);
  }
  constexpr FVector2 operator*(const FVector2& Other) const {
    return FVector2(x * Other.x, y * Other.y);
  }
  constexpr FVector2 operator/(const FVector2& Other) const {
    return FVector2(x / Other.x, y / Other.y);
  }
  constexpr FVector2 operator*(float Value) const {
    return FVector2(x * Value, y * Value);
  }
  constexpr FVector2 operator/(float Value) const {
    return FVector2(x / Value, y / Value);
  }

  constexpr FVector2& operator+=(const FVector2& Other) {
    x += Other.x;
    y += Other.y;
    return *this;
  }
  constexpr FVector2& operator-=(const FVector2& Other) {
    x -= Other.x;
    y -= Other.y;
    return *this;
  }
  constexpr FVector2& operator*=(const FVector2& Other) {
    x *= Other.x;
    y *= Other.y;
    return *this;
  }
  constexpr FVector2& operator/=(const FVector2& Other) {
    x /= Other.x;
    y /= Other.y;
    return *this;
  }

  constexpr FVector2& operator*=(float Value) {
    x *= Value;
    y *= Value;
    return *this;
  }
  constexpr FVector2& operator/=(float Value) {
    x /= Value;
    y /= Value;
    return *this;
  }

  friend constexpr FVector2 operator*(float Value, const FVector2& Vector) {
    return Vector * Value;
  }

  bool NearlyEquals(const FVector2& Other, float Tolerance = FMath::Epsilon) const {
    return (FMath::IsNearlyEqual(x, Other.x, Tolerance) && FMath::IsNearlyEqual(y, Other.y, Tolerance));
  }

  constexpr float LengthSquared() const {
    return x * x + y * y;
  }

  float Length() const {
    return FMath::Sqrt(LengthSquared());
  }

  constexpr float Dot(const FVector2& Other) const {
    return x * Other.x + y * Other.y;
  }

  void Normalize() {
    float Len = Length();
    if(Len > FMath::Epsilon) {
      x /= Len;
      y /= Len;
    } else {
      x = 0.0f;
      y = 0.0f;
    }
  }

  FVector2 GetNormalized() const {
    float Len = Length();
    if(Len > FMath::Epsilon) {
      return FVector2(x / Len, y / Len);
    }
    return FVector2(0.0f);
  }

  float Distance(const FVector2& Other) const {
    return (*this - Other).Length();
  }

  cstring Printf() {
    thread_local char Buffers[4][128];
    thread_local uint32 Index = 0;
    char* buffer = Buffers[Index];
    Index = (Index + 1) & 3;
    snprintf(buffer, sizeof(Buffers[0]), "X:%.2f, Y:%.2f", x, y);
    return buffer;
  }

  constexpr float DistanceSquared(const FVector2& Other) const {
    return (*this - Other).LengthSquared();
  }

  static constexpr FVector2 GetRight() {
    return FVector2(1.f, 0.f);
  }
  static constexpr FVector2 GetUp() {
    return FVector2(0.f, 1.f);
  }
};
