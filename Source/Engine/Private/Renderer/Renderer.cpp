#include "Renderer/Renderer.h"
#include "Renderer/RHI.h"
#include "Platform/Platform.h"

FRenderer::FRenderer() {}

FRenderer::~FRenderer() {}

void FRenderer::BeginFrame() {
  RHI::Clear();
}

void FRenderer::EndFrame() {}
