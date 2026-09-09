#pragma once
#include "Math/Utils.h"

struct FTransform;
struct FRotator;
struct FVector3;
struct FQuat;

// My matrix is e[column][row];
// I use column vectors (v' = M * v)
// right-multiplication, Right +X, Up +Y, and Forward -Z.
// Standard matrix
// [][][][tx];
// [][][][ty];
// [][][][tz];
// [][][][tw];
// Transposed View
// [rx][ry][rz][rd];
// [ux][uy][uy][ud];
// [fx][fy][fz][fd];
// [00][00][00][01];

struct ENGINE_API FMatrix4 {
  float e[4][4];

  static FMatrix4 MakeIdentity();
  static FMatrix4 MakePerspective(float Fov, float Aspect, float ZNear, float ZFar);
  static FMatrix4 MakeTransform(const FTransform& Transform);
  static FMatrix4 MakeTranslation(const FVector3& Location);
  static FMatrix4 MakeRotation(const FQuat& Rotation);
  static FMatrix4 MakeScale(const FVector3& Scale);
  static FMatrix4 LookAt(const FVector3& Eye, const FVector3& Target, const FVector3& Up);
  static FMatrix4 Mul(const FMatrix4& M1, const FMatrix4& M2);
};
