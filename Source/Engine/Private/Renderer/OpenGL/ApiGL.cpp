#include "ApiGL.h"

#define _GT_DEBUG_GL_FUNCTION(TName)

#if defined(PLATFORM_LINUX)
#define GT_LOAD_GL_FUNCTION(TType, TName)                     \
  FGL::TName = (TType)GetProcAddress((const GLubyte*)#TName); \
  if(FGL::TName == nullptr) {                                 \
    _GT_DEBUG_GL_FUNCTION(TName);                             \
  }
bool ApiGLLinuxLoadFunctions(void*(GetProcAddress)(const unsigned char* Name)) {
  XMACRO_GL_LIST(GT_LOAD_GL_FUNCTION);
  return true;
}
#endif
