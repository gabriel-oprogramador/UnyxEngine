#pragma once
#include <cmath>
#include "Core/Core.h"

struct FMath {
  static constexpr float Pi = 3.14159265358979323846f;
  static constexpr float TwoPi = Pi * 2.0f;
  static constexpr float HalfPi = Pi * 0.5f;
  static constexpr float DegToRad = Pi / 180.0f;
  static constexpr float RadToDeg = 180.0f / Pi;
  static constexpr float Epsilon = 1e-6f;

  static constexpr float Degrees(float Degrees) {
    return Degrees * DegToRad;
  }

  static constexpr float Radians(float Radians) {
    return Radians * RadToDeg;
  }

  static constexpr float Min(float A, float B) {
    return A < B ? A : B;
  }

  static constexpr float Max(float A, float B) {
    return A > B ? A : B;
  }

  static constexpr float Clamp(float X, float Min, float Max) {
    return X < Min ? Min : (X > Max ? Max : X);
  }

  static constexpr float Abs(float X) {
    return X < 0.0f ? -X : X;
  }

  static constexpr float Lerp(float A, float B, float Alpha) {
    return A + (B - A) * Alpha;
  }
  static float LerpAngle(float A, float B, float Alpha) {
    float Delta = B - A;
    if(Delta > 180.0f) {
      Delta -= 360.0f;
    }
    if(Delta < -180.0f) {
      Delta += 360.0f;
    }
    float Result = A + Delta * Alpha;
    return FMath::Wrap(Result, 0.0f, 360.0f);
  }

  static constexpr bool IsZero(float Value) {
    return Value == 0.0f;
  }

  static constexpr bool IsEqual(float A, float B) {
    return A == B;
  }

  static constexpr bool IsNearlyZero(float Value, float Tolerance = Epsilon) {
    return Abs(Value) <= Tolerance;
  }

  static constexpr bool IsNearlyEqual(float A, float B, float Tolerance = Epsilon) {
    return Abs(A - B) <= Tolerance;
  }

  static inline float Sqrt(float Value) {
    return std::sqrt(Value);
  }

  static float Exp(float Value) {
    return std::exp(Value);
  }

  static inline float Sin(float Radians) {
    return std::sin(Radians);
  }

  static inline float Cos(float Radians) {
    return std::cos(Radians);
  }

  static inline float Tan(float Radians) {
    return std::tan(Radians);
  }

  static inline float Asin(float Value) {
    return std::asin(Value);
  }

  static inline float Acos(float Value) {
    return std::acos(Value);
  }

  static inline float Atan(float Value) {
    return std::atan(Value);
  }

  static inline float Atan2(float Y, float X) {
    return std::atan2(Y, X);
  }

  static inline float Floor(float Value) {
    return std::floor(Value);
  }

  static inline float Ceil(float Value) {
    return std::ceil(Value);
  }

  static inline float Round(float Value) {
    return std::round(Value);
  }

  static inline float Frac(float Value) {
    return Value - std::floor(Value);
  }

  static constexpr float Mod(float A, float B) {
    return fmodf(A, B);
  }

  static constexpr float Wrap(float Value, float Min, float Max) {
    return Mod(Mod(Value - Min, Max - Min) + (Max - Min), Max - Min) + Min;
  }
};
