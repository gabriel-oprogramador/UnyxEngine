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

#elif defined(PLATFORM_WINDOWS)
#include <windows.h>
#define GT_LOAD_GL_FUNCTION(TType, TName)            \
  FGL::TName = (TType)GetProcAddress(libGL, #TName); \
  if(FGL::TName == nullptr) {                        \
    FGL::TName = (TType)wglGetProcAddress(#TName);   \
  } else if(FGL::TName == NULL) {                    \
    _GT_DEBUG_GL_FUNCTION(TName)                     \
  }

bool ApiWindowsLoadGLFunctions() {
  HMODULE libGL = LoadLibraryA("opengl32.dll");
  if(libGL == NULL) {
    return false;
  }
  XMACRO_GL_LIST(GT_LOAD_GL_FUNCTION);
  return true;
}

#endif
