#include "Math/Matrix4.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Rotator.h"
#include "Math/Transform.h"
#include "Math/Quaternion.h"

FMatrix4 FMatrix4::MakeIdentity() {
  FMatrix4 m{};
  m.e[0][0] = m.e[1][1] = m.e[2][2] = m.e[3][3] = 1.f;
  return m;
}

FMatrix4 FMatrix4::MakePerspective(float Fov, float Aspect, float ZNear, float ZFar) {
  Fov = (float)(Fov * (M_PI / 180.0));
  const float tanHalfFov = (float)tan(Fov / 2);
  FMatrix4 m = FMatrix4::MakeIdentity();
  m.e[0][0] = 1 / (Aspect * tanHalfFov);
  m.e[1][1] = 1 / tanHalfFov;
  m.e[2][2] = -(ZFar + ZNear) / (ZFar - ZNear);
  m.e[2][3] = -1;
  m.e[3][2] = -(2 * ZFar * ZNear) / (ZFar - ZNear);
  m.e[3][3] = 0.0f;
  return m;
}

FMatrix4 FMatrix4::MakeTransform(const FTransform& Transform) {
  FMatrix4 mTranslation = FMatrix4::MakeTranslation(Transform.location);
  FMatrix4 mRotation = FMatrix4::MakeRotation(Transform.rotation);
  FMatrix4 mScale = FMatrix4::MakeScale(Transform.scale);
  return FMatrix4::Mul(FMatrix4::Mul(mTranslation, mRotation), mScale);
}

FMatrix4 FMatrix4::MakeTranslation(const FVector3& Location) {
  FMatrix4 m = FMatrix4::MakeIdentity();
  m.e[3][0] = Location.x;
  m.e[3][1] = Location.y;
  m.e[3][2] = Location.z;
  return m;
}

FMatrix4 FMatrix4::MakeRotation(const FQuat& Rotation) {
  const float xx = Rotation.x * Rotation.x;
  const float yy = Rotation.y * Rotation.y;
  const float zz = Rotation.z * Rotation.z;

  const float xy = Rotation.x * Rotation.y;
  const float xz = Rotation.x * Rotation.z;
  const float yz = Rotation.y * Rotation.z;

  const float wx = Rotation.w * Rotation.x;
  const float wy = Rotation.w * Rotation.y;
  const float wz = Rotation.w * Rotation.z;

  FMatrix4 m = FMatrix4::MakeIdentity();

  m.e[0][0] = 1.0f - 2.0f * (yy + zz);
  m.e[0][1] = 2.0f * (xy - wz);
  m.e[0][2] = 2.0f * (xz + wy);

  m.e[1][0] = 2.0f * (xy + wz);
  m.e[1][1] = 1.0f - 2.0f * (xx + zz);
  m.e[1][2] = 2.0f * (yz - wx);

  m.e[2][0] = 2.0f * (xz - wy);
  m.e[2][1] = 2.0f * (yz + wx);
  m.e[2][2] = 1.0f - 2.0f * (xx + yy);

  return m;
}

FMatrix4 FMatrix4::MakeScale(const FVector3& Scale) {
  FMatrix4 m = FMatrix4::MakeIdentity();
  m.e[0][0] = Scale.x;
  m.e[1][1] = Scale.y;
  m.e[2][2] = Scale.z;
  return m;
}

FMatrix4 FMatrix4::Mul(const FMatrix4& M1, const FMatrix4& M2) {
  FMatrix4 m{};
  for(int line = 0; line < 4; line++) {
    for(int column = 0; column < 4; column++) {
      m.e[column][line] =                    //
          M1.e[0][line] * M2.e[column][0] +  //
          M1.e[1][line] * M2.e[column][1] +  //
          M1.e[2][line] * M2.e[column][2] +  //
          M1.e[3][line] * M2.e[column][3];   //
    }
  }
  return m;
}

FMatrix4 FMatrix4::LookAt(const FVector3& Eye, const FVector3& Target, const FVector3& Up) {
  FVector3 forward = (Target - Eye).GetNormalized();
  FVector3 right = forward.Cross(Up).GetNormalized();
  FVector3 up = right.Cross(forward).GetNormalized();
  FMatrix4 m = FMatrix4::MakeIdentity();

  // Row 0
  m.e[0][0] = right.x;
  m.e[1][0] = right.y;
  m.e[2][0] = right.z;

  // Row 1
  m.e[0][1] = up.x;
  m.e[1][1] = up.y;
  m.e[2][1] = up.z;

  // Row 2
  m.e[0][2] = -forward.x;
  m.e[1][2] = -forward.y;
  m.e[2][2] = -forward.z;

  // Translation
  m.e[3][0] = -right.Dot(Eye);
  m.e[3][1] = -up.Dot(Eye);
  m.e[3][2] = forward.Dot(Eye);
  m.e[3][3] = 1.0f;
  return m;
}
