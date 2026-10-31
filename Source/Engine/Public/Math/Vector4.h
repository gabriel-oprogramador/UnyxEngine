#pragma once
#include "Math/Utils.h"

struct ENGINE_API FVector4 {
  float x;
  float y;
  float z;
  float w;

  FVector4() = default;

  constexpr explicit FVector4(float Value) : x(Value), y(Value), z(Value), w(Value) {}

  constexpr FVector4(float X, float Y, float Z, float W) : x(X), y(Y), z(Z), w(W) {}

  constexpr bool operator==(const FVector4& Other) const {
    return x == Other.x && y == Other.y && z == Other.z && w == Other.w;
  }

  constexpr bool operator!=(const FVector4& Other) const {
    return !(*this == Other);
  }

  constexpr FVector4 operator-() const {
    return FVector4(-x, -y, -z, -w);
  }

  constexpr FVector4 operator+(const FVector4& Other) const {
    return FVector4(x + Other.x, y + Other.y, z + Other.z, w + Other.w);
  }

  constexpr FVector4 operator-(const FVector4& Other) const {
    return FVector4(x - Other.x, y - Other.y, z - Other.z, w - Other.w);
  }

  constexpr FVector4 operator*(float Value) const {
    return FVector4(x * Value, y * Value, z * Value, w * Value);
  }

  constexpr FVector4 operator/(float Value) const {
    return FVector4(x / Value, y / Value, z / Value, w / Value);
  }

  constexpr float LengthSquared() const {
    return x * x + y * y + z * z + w * w;
  }

  float Length() const {
    return FMath::Sqrt(LengthSquared());
  }

  constexpr float Dot(const FVector4& Other) const {
    return x * Other.x + y * Other.y + z * Other.z + w * Other.w;
  }

  void Normalize() {
    float Len = Length();
    if(Len > FMath::Epsilon) {
      x /= Len;
      y /= Len;
      z /= Len;
      w /= Len;
    } else {
      x = y = z = w = 0.0f;
    }
  }

  FVector4 GetNormalized() const {
    float Len = Length();
    if(Len > FMath::Epsilon) {
      return FVector4(x / Len, y / Len, z / Len, w / Len);
    }
    return FVector4(0.0f);
  }

  cstring Printf() {
    thread_local char Buffers[4][128];
    thread_local uint32 Index = 0;
    char* buffer = Buffers[Index];
    Index = (Index + 1) & 3;
    snprintf(buffer, sizeof(Buffers[0]), "(X:%.2f, Y:%.2f, Z:%.2f, W:%.2f)", x, y, z, w);
    return buffer;
  }
};
