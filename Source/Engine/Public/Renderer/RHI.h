#pragma once
#include "Math/Color.h"
#include "Renderer/Types.h"

namespace RHI {
  ENGINE_API void Clear();
  ENGINE_API void ClearColor(const FColor& Color);
  ENGINE_API void SetViewport(const FViewport& Viewport);
}  // namespace RHI
