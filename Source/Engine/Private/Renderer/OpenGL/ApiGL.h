#pragma once

#if defined(PLATFORM_WEB) || defined(PLATFORM_ANDROID)
#include <GLES3/gl3.h>

#elif defined(PLATFORM_WINDOWS) || defined(PLATFORM_LINUX)

#include "GL/glcorearb.h"

#define XMACRO_GL_LIST(X)                                          \
  /* State */                                                      \
  X(PFNGLCLEARCOLORPROC, glClearColor)                             \
  X(PFNGLCLEARPROC, glClear)                                       \
  X(PFNGLVIEWPORTPROC, glViewport)                                 \
  X(PFNGLENABLEPROC, glEnable)                                     \
  X(PFNGLDISABLEPROC, glDisable)                                   \
  X(PFNGLBLENDFUNCPROC, glBlendFunc)                               \
  X(PFNGLDEPTHFUNCPROC, glDepthFunc)                               \
  X(PFNGLDEPTHMASKPROC, glDepthMask)                               \
  X(PFNGLCULLFACEPROC, glCullFace)                                 \
  X(PFNGLFRONTFACEPROC, glFrontFace)                               \
                                                                   \
  /* VAO */                                                        \
  X(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays)                   \
  X(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray)                   \
  X(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays)             \
                                                                   \
  /* Vertex Buffers */                                             \
  X(PFNGLGENBUFFERSPROC, glGenBuffers)                             \
  X(PFNGLBINDBUFFERPROC, glBindBuffer)                             \
  X(PFNGLBUFFERDATAPROC, glBufferData)                             \
  X(PFNGLBUFFERSUBDATAPROC, glBufferSubData)                       \
  X(PFNGLDELETEBUFFERSPROC, glDeleteBuffers)                       \
  X(PFNGLMAPBUFFERRANGEPROC, glMapBufferRange)                     \
  X(PFNGLUNMAPBUFFERPROC, glUnmapBuffer)                           \
                                                                   \
  /* Vertex Attributes */                                          \
  X(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray)   \
  X(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray) \
  X(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer)           \
  X(PFNGLVERTEXATTRIBIPOINTERPROC, glVertexAttribIPointer)         \
                                                                   \
  /* Shader */                                                     \
  X(PFNGLCREATESHADERPROC, glCreateShader)                         \
  X(PFNGLSHADERSOURCEPROC, glShaderSource)                         \
  X(PFNGLCOMPILESHADERPROC, glCompileShader)                       \
  X(PFNGLGETSHADERIVPROC, glGetShaderiv)                           \
  X(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog)                 \
  X(PFNGLDELETESHADERPROC, glDeleteShader)                         \
                                                                   \
  /* Program */                                                    \
  X(PFNGLCREATEPROGRAMPROC, glCreateProgram)                       \
  X(PFNGLATTACHSHADERPROC, glAttachShader)                         \
  X(PFNGLLINKPROGRAMPROC, glLinkProgram)                           \
  X(PFNGLGETPROGRAMIVPROC, glGetProgramiv)                         \
  X(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog)               \
  X(PFNGLUSEPROGRAMPROC, glUseProgram)                             \
  X(PFNGLDELETEPROGRAMPROC, glDeleteProgram)                       \
                                                                   \
  /* Uniforms */                                                   \
  X(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation)             \
  X(PFNGLUNIFORM1IPROC, glUniform1i)                               \
  X(PFNGLUNIFORM1UIPROC, glUniform1ui)                             \
  X(PFNGLUNIFORM1FPROC, glUniform1f)                               \
  X(PFNGLUNIFORM2FPROC, glUniform2f)                               \
  X(PFNGLUNIFORM3FPROC, glUniform3f)                               \
  X(PFNGLUNIFORM4FPROC, glUniform4f)                               \
  X(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv)                 \
                                                                   \
  /* UBO */                                                        \
  X(PFNGLGETUNIFORMBLOCKINDEXPROC, glGetUniformBlockIndex)         \
  X(PFNGLUNIFORMBLOCKBINDINGPROC, glUniformBlockBinding)           \
  X(PFNGLBINDBUFFERBASEPROC, glBindBufferBase)                     \
  X(PFNGLBINDBUFFERRANGEPROC, glBindBufferRange)                   \
                                                                   \
  /* Textures */                                                   \
  X(PFNGLGENTEXTURESPROC, glGenTextures)                           \
  X(PFNGLBINDTEXTUREPROC, glBindTexture)                           \
  X(PFNGLTEXPARAMETERIPROC, glTexParameteri)                       \
  X(PFNGLTEXIMAGE2DPROC, glTexImage2D)                             \
  X(PFNGLTEXSUBIMAGE2DPROC, glTexSubImage2D)                       \
  X(PFNGLGENERATEMIPMAPPROC, glGenerateMipmap)                     \
  X(PFNGLDELETETEXTURESPROC, glDeleteTextures)                     \
                                                                   \
  /* Framebuffer */                                                \
  X(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers)                   \
  X(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer)                   \
  X(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D)         \
  X(PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus)     \
  X(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers)             \
  X(PFNGLDRAWBUFFERPROC, glDrawBuffer)                             \
  X(PFNGLREADBUFFERPROC, glReadBuffer)                             \
                                                                   \
  /* Renderbuffers */                                              \
  X(PFNGLGENRENDERBUFFERSPROC, glGenRenderbuffers)                 \
  X(PFNGLBINDRENDERBUFFERPROC, glBindRenderbuffer)                 \
  X(PFNGLRENDERBUFFERSTORAGEPROC, glRenderbufferStorage)           \
  X(PFNGLFRAMEBUFFERRENDERBUFFERPROC, glFramebufferRenderbuffer)   \
  X(PFNGLDELETERENDERBUFFERSPROC, glDeleteRenderbuffers)           \
                                                                   \
  /* Draw */                                                       \
  X(PFNGLDRAWARRAYSPROC, glDrawArrays)                             \
  X(PFNGLDRAWELEMENTSPROC, glDrawElements)                         \
  X(PFNGLDRAWARRAYSINSTANCEDPROC, glDrawArraysInstanced)           \
  X(PFNGLDRAWELEMENTSINSTANCEDPROC, glDrawElementsInstanced)       \
                                                                   \
  /* Queries / Debug */                                            \
  X(PFNGLGETERRORPROC, glGetError)                                 \
  X(PFNGLGETSTRINGPROC, glGetString)                               \
  X(PFNGLGETINTEGERVPROC, glGetIntegerv)

#define DECLARE_GL_FUNCTIONS(Type, Name) static inline Type Name = nullptr;

struct FGL {
  XMACRO_GL_LIST(DECLARE_GL_FUNCTIONS)
};

#define glClearColor FGL::glClearColor
#define glClear      FGL::glClear
#define glViewport   FGL::glViewport
#define glEnable     FGL::glEnable
#define glDisable    FGL::glDisable
#define glBlendFunc  FGL::glBlendFunc
#define glDepthFunc  FGL::glDepthFunc
#define glDepthMask  FGL::glDepthMask
#define glCullFace   FGL::glCullFace
#define glFrontFace  FGL::glFrontFace

#define glGenVertexArrays    FGL::glGenVertexArrays
#define glBindVertexArray    FGL::glBindVertexArray
#define glDeleteVertexArrays FGL::glDeleteVertexArrays

#define glGenBuffers     FGL::glGenBuffers
#define glBindBuffer     FGL::glBindBuffer
#define glBufferData     FGL::glBufferData
#define glBufferSubData  FGL::glBufferSubData
#define glDeleteBuffers  FGL::glDeleteBuffers
#define glMapBufferRange FGL::glMapBufferRange
#define glUnmapBuffer    FGL::glUnmapBuffer

#define glEnableVertexAttribArray  FGL::glEnableVertexAttribArray
#define glDisableVertexAttribArray FGL::glDisableVertexAttribArray
#define glVertexAttribPointer      FGL::glVertexAttribPointer
#define glVertexAttribIPointer     FGL::glVertexAttribIPointer

#define glCreateShader     FGL::glCreateShader
#define glShaderSource     FGL::glShaderSource
#define glCompileShader    FGL::glCompileShader
#define glGetShaderiv      FGL::glGetShaderiv
#define glGetShaderInfoLog FGL::glGetShaderInfoLog
#define glDeleteShader     FGL::glDeleteShader

#define glCreateProgram     FGL::glCreateProgram
#define glAttachShader      FGL::glAttachShader
#define glLinkProgram       FGL::glLinkProgram
#define glGetProgramiv      FGL::glGetProgramiv
#define glGetProgramInfoLog FGL::glGetProgramInfoLog
#define glUseProgram        FGL::glUseProgram
#define glDeleteProgram     FGL::glDeleteProgram

#define glGetUniformLocation FGL::glGetUniformLocation
#define glUniform1i          FGL::glUniform1i
#define glUniform1ui         FGL::glUniform1ui
#define glUniform1f          FGL::glUniform1f
#define glUniform2f          FGL::glUniform2f
#define glUniform3f          FGL::glUniform3f
#define glUniform4f          FGL::glUniform4f
#define glUniformMatrix4fv   FGL::glUniformMatrix4fv

#define glGetUniformBlockIndex FGL::glGetUniformBlockIndex
#define glUniformBlockBinding  FGL::glUniformBlockBinding
#define glBindBufferBase       FGL::glBindBufferBase
#define glBindBufferRange      FGL::glBindBufferRange

#define glGenTextures    FGL::glGenTextures
#define glBindTexture    FGL::glBindTexture
#define glTexParameteri  FGL::glTexParameteri
#define glTexImage2D     FGL::glTexImage2D
#define glTexSubImage2D  FGL::glTexSubImage2D
#define glGenerateMipmap FGL::glGenerateMipmap
#define glDeleteTextures FGL::glDeleteTextures

#define glGenFramebuffers        FGL::glGenFramebuffers
#define glBindFramebuffer        FGL::glBindFramebuffer
#define glFramebufferTexture2D   FGL::glFramebufferTexture2D
#define glCheckFramebufferStatus FGL::glCheckFramebufferStatus
#define glDeleteFramebuffers     FGL::glDeleteFramebuffers
#define glDrawBuffer             FGL::glDrawBuffer
#define glReadBuffer             FGL::glReadBuffer

#define glGenRenderbuffers        FGL::glGenRenderbuffers
#define glBindRenderbuffer        FGL::glBindRenderbuffer
#define glRenderbufferStorage     FGL::glRenderbufferStorage
#define glFramebufferRenderbuffer FGL::glFramebufferRenderbuffer
#define glDeleteRenderbuffers     FGL::glDeleteRenderbuffers

#define glDrawArrays            FGL::glDrawArrays
#define glDrawElements          FGL::glDrawElements
#define glDrawArraysInstanced   FGL::glDrawArraysInstanced
#define glDrawElementsInstanced FGL::glDrawElementsInstanced

#define glGetError    FGL::glGetError
#define glGetString   FGL::glGetString
#define glGetIntegerv FGL::glGetIntegerv

#endif
