#ifdef RENDERER_OPENGL
#include "Renderer/RHI.h"
#include "ApiGL.h"

namespace RHI {

  void Clear() {
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
