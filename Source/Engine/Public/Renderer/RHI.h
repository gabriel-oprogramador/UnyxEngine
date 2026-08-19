#pragma once
#include "Math/Color.h"
#include "Renderer/Types.h"

namespace RHI {
  void Clear();
  void ClearColor(const FColor& Color);
  void SetViewport(const FViewport& Viewport);
}  // namespace RHI
