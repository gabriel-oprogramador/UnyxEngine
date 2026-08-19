#pragma once
#include "Core/Core.h"

struct FRenderer {
  FRenderer(const FRenderer& Other) = delete;
  FRenderer(FRenderer&& Other) = delete;
  FRenderer& operator=(const FRenderer& Other) = delete;
  FRenderer& operator=(FRenderer&& Other) = delete;

private:
  uint32 glVersion{0};
  friend struct FEngine;
  FRenderer();
  ~FRenderer();
  void BeginFrame();
  void EndFrame();
};
