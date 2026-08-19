#pragma once
#include "Math/Utils.h"

struct FQuat;

struct FRotator {
  float pitch;
  float yaw;
  float roll;

  FRotator() = default;

  constexpr explicit FRotator(float Value) : pitch(Value), yaw(Value), roll(Value) {}

  constexpr FRotator(float Pitch, float Yaw, float Roll) : pitch(Pitch), yaw(Yaw), roll(Roll) {}

  explicit FRotator(const FQuat& Quaternion);

  constexpr bool operator==(const FRotator& Other) const {
    return pitch == Other.pitch && yaw == Other.yaw && roll == Other.roll;
  }

  constexpr bool operator!=(const FRotator& Other) const {
    return !(*this == Other);
  }

  constexpr FRotator operator-() const {
    return FRotator(-pitch, -yaw, -roll);
  }

  constexpr FRotator operator+(const FRotator& Other) const {
    return FRotator(pitch + Other.pitch, yaw + Other.yaw, roll + Other.roll);
  }

  constexpr FRotator operator-(const FRotator& Other) const {
    return FRotator(pitch - Other.pitch, yaw - Other.yaw, roll - Other.roll);
  }

  constexpr FRotator operator*(float Value) const {
    return FRotator(pitch * Value, yaw * Value, roll * Value);
  }

  constexpr FRotator operator/(float Value) const {
    return FRotator(pitch / Value, yaw / Value, roll / Value);
  }

  constexpr FRotator& operator+=(const FRotator& Other) {
    pitch += Other.pitch;
    yaw += Other.yaw;
    roll += Other.roll;
    return *this;
  }

  constexpr FRotator& operator-=(const FRotator& Other) {
    pitch -= Other.pitch;
    yaw -= Other.yaw;
    roll -= Other.roll;
    return *this;
  }

  constexpr FRotator& operator*=(float Value) {
    pitch *= Value;
    yaw *= Value;
    roll *= Value;
    return *this;
  }

  constexpr FRotator& operator/=(float Value) {
    pitch /= Value;
    yaw /= Value;
    roll /= Value;
    return *this;
  }

  friend constexpr FRotator operator*(float Value, const FRotator& Rotator) {
    return Rotator * Value;
  }

  bool NearlyEquals(const FRotator& Other, float Tolerance = FMath::Epsilon) const {
    return FMath::IsNearlyEqual(pitch, Other.pitch, Tolerance) && FMath::IsNearlyEqual(yaw, Other.yaw, Tolerance) && FMath::IsNearlyEqual(roll, Other.roll, Tolerance);
  }

  cstring Printf() {
    thread_local char Buffers[4][128];
    thread_local uint32 Index = 0;
    char* buffer = Buffers[Index];
    Index = (Index + 1) & 3;
    snprintf(buffer, sizeof(Buffers[0]), "Pitch:%.2f, Yaw:%.2f, Roll:%.2f", pitch, yaw, roll);
    return buffer;
  }

  static constexpr float NormalizeAxis(float Angle) {
    while(Angle > 180.0f) {
      Angle -= 360.0f;
    }
    while(Angle < -180.0f) {
      Angle += 360.0f;
    }
    return Angle;
  }

  constexpr FRotator GetNormalized() const {
    return FRotator(NormalizeAxis(pitch), NormalizeAxis(yaw), NormalizeAxis(roll));
  }

  FQuat ToQuat() const;
};
