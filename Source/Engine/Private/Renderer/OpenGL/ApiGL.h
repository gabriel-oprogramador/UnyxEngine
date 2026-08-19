#pragma once

#if defined(PLATFORM_WEB) || defined(PLATFORM_ANDROID)
#include <GLES3/gl3.h>

#elif defined(PLATFORM_WINDOWS) || defined(PLATFORM_LINUX)
#include "GL/glcorearb.h"
#define XMACRO_GL_LIST(X)              \
  X(PFNGLCLEARCOLORPROC, glClearColor) \
  X(PFNGLCLEARPROC, glClear)           \
  X(PFNGLVIEWPORTPROC, glViewport)

#define DECLARE_GL_FUNCTIONS(Type, Name) static inline Type Name = nullptr;
struct FGL {
  XMACRO_GL_LIST(DECLARE_GL_FUNCTIONS)
};

#define glViewport   FGL::glViewport
#define glClear      FGL::glClear
#define glClearColor FGL::glClearColor

#endif
