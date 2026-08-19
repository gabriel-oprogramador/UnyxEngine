#ifdef RENDERER_OPENGL
#include "Renderer/RHI.h"
#include "Platform/Platform.h"
#include "ApiGL.h"

namespace RHI {

  void Clear() {
    float time = Platform::TimeGetNow();
    float speed = 0.5f;
    float angle = time * speed;
    float r = (sinf(angle) + 1.0f) / 2.0f;
    float g = (sinf(angle + 2.094f) + 1.0f) / 2.0f;
    float b = (sinf(angle + 4.188f) + 1.0f) / 2.0f;
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  }

  void ClearColor(const FColor& Color) {
    float color[4] = {
        Color.r / 255.f,  //
        Color.g / 255.f,  //
        Color.b / 255.f,  //
        Color.a / 255.f   //
    };
    glClearColor(color[0], color[1], color[2], color[3]);
  }

  void SetViewport(const FViewport& Viewport) {
    const FViewport& vp = Viewport;
    glViewport(vp.posX, vp.posY, vp.width, vp.height);
  }

}  // namespace RHI

#endif  // RENDERER_OPENGL
