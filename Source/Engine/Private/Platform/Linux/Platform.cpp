#ifdef PLATFORM_LINUX

#include "Platform/Platform.h"
#include "Platform/Event.h"
#include "STB/stb_image.h"
#include "STB/stb_image_resize2.h"

#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XInput2.h>
#include <GL/glx.h>
#include <cstring>

#ifdef Success
#undef Success
#endif

// Source/Engine/Private/Renderer/OpenGL/ApiGL.cpp
extern bool ApiGLLinuxLoadFunctions(void*(GetProcAddress)(const unsigned char* Name));

// Source/Engine/Private/Runtime/App.cpp
extern void AppBootstrapInitialize();
extern void AppBootstrapTerminate();
extern void AppBootstrapUpdate();

// Source/Engine/Private/Platform/Linux/Input.cpp
extern bool ApiXInput2Init(Display* Display);
extern void ApiXInput2UpdateKeyMap();
extern void ApiXInput2PollCoreEvent(XEvent* Event);
extern void ApiXInput2PollRawEvent(XEvent* Event);

static void InternalWinProc();
static void InternalCaptureMouse(bool bCapture);

struct PWindow {
  cstring title{nullptr};
  Atom wmDelete{0};
  Window win{0};
  uint32 width{0};
  uint32 height{0};
  int32 lastMousePosX{0};
  int32 lastMousePosY{0};
  bool bShouldClose{true};
  bool bNeedResize{false};
  bool bFullscreen{false};
  bool bMouseCaptured{false};
  bool bFocused{false};
};

struct FApi {
  Display* display{nullptr};
  Window root{0};
  uint32 screen{0};
  int32 xkbEventCode{0};
  int32 xiOpcode{0};
  XVisualInfo* visualInfo{nullptr};
  Colormap colormap{0};
  GLXContext context{nullptr};
  PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT{nullptr};
  PWindow mainWindow;
  Cursor invisibleCursor;
  bool bMouseCaptured{false};

  FApi() {
    display = XOpenDisplay(NULL);
    UE_ASSERT(display);

    root = XDefaultRootWindow(display);
    screen = XDefaultScreen(display);

    XkbSetDetectableAutoRepeat(display, true, NULL);

    Pixmap noData;
    XColor black;
    char noDataBits[] = {0};
    noData = XCreateBitmapFromData(display, root, noDataBits, 1, 1);
    black.red = black.green = black.blue = 0;
    invisibleCursor = XCreatePixmapCursor(display, noData, noData, &black, &black, 0, 0);
    XFreePixmap(display, noData);

    int32 xkbOpcode, xkbEvent, xkbError;
    int32 major = XkbMajorVersion;
    int32 minor = XkbMinorVersion;
    if(XkbQueryExtension(display, &xkbOpcode, &xkbEvent, &xkbError, &major, &minor)) {
      xkbEventCode = xkbEvent;
      XkbSelectEventDetails(display, XkbUseCoreKbd, XkbStateNotify, XkbAllStateComponentsMask, XkbGroupStateMask);
    }

    int32 opcode, event, error;
    if(XQueryExtension(display, "XInputExtension", &opcode, &event, &error)) {
      int32 xiMajor = 2;
      int32 xiMinor = 0;
      if(XIQueryVersion(display, &xiMajor, &xiMinor) == 0) {
        xiOpcode = opcode;
        ApiXInput2Init(display);
      }
    }
  }
};

static FApi& GetApi() {
  static FApi api{};
  return api;
}

int main(int argc, const char** argv) {
  UE_INFO("Unyx Engine for Linux");

  AppBootstrapInitialize();

  Display* dpy = GetApi().display;
  GLXContext ctx = GetApi().context;
  PWindow& pWin = GetApi().mainWindow;
  while(!pWin.bShouldClose) {
    InternalWinProc();
    AppBootstrapUpdate();
    glXSwapBuffers(dpy, pWin.win);
  }

  AppBootstrapTerminate();

  return 0;
}

static void InternalWinProc() {
  FApi& api = GetApi();
  Display* dpy = api.display;
  PWindow& pWindow = api.mainWindow;
  uint32 eventCount = 0;
  XEvent event{};
  while(XPending(dpy)) {
    XNextEvent(dpy, &event);
    eventCount++;

    if(event.type == api.xkbEventCode) {
      XkbEvent* xkbEvent = (XkbEvent*)&event;
      if(xkbEvent->any.xkb_type == XkbStateNotify) {
        ApiXInput2UpdateKeyMap();
        continue;
      }
    }

    if(pWindow.bFocused && event.xcookie.type == GenericEvent && event.xcookie.extension == GetApi().xiOpcode) {
      ApiXInput2PollRawEvent(&event);
      continue;
    }

    switch(event.type) {
      case ClientMessage: {
        if((Atom)event.xclient.data.l[0] == pWindow.wmDelete) {
          pWindow.bShouldClose = true;
        }
        break;
      }
      case FocusIn: {
        Window focused;
        int32 revertTO;
        XGetInputFocus(dpy, &focused, &revertTO);
        if(pWindow.win == focused && pWindow.bFocused != true) {
          pWindow.bFocused = true;
          InternalCaptureMouse(pWindow.bMouseCaptured);
          PEvent pEvent = {PEventType::WindowFocus, &pWindow};
          pEvent.windowFocus.bFocused = true;
          Platform::PushEvent(pEvent);
        }
        break;
      }
      case FocusOut: {
        Window focused;
        int32 revertTO;
        XGetInputFocus(dpy, &focused, &revertTO);
        if(pWindow.win != focused && pWindow.bFocused != false) {
          pWindow.bFocused = false;
          InternalCaptureMouse(false);
          PEvent pEvent = {PEventType::WindowFocus, &pWindow};
          pEvent.windowFocus.bFocused = false;
          Platform::PushEvent(pEvent);
        }
        break;
      }
      case KeyPress:
      case KeyRelease:
      case ButtonPress:
      case ButtonRelease: {
        ApiXInput2PollCoreEvent(&event);
        break;
      }
      case MotionNotify: {
        PEvent pEvent = {PEventType::MousePos, &pWindow};
        pEvent.mousePos.posX = event.xmotion.x;
        pEvent.mousePos.posY = event.xmotion.y;
        Platform::PushEvent(pEvent);
        break;
      }
      case ConfigureNotify: {
        int32 width = event.xconfigure.width;
        int32 height = event.xconfigure.height;
        if(width != pWindow.width || height != pWindow.height) {
          pWindow.width = event.xconfigure.width;
          pWindow.height = event.xconfigure.height;
          pWindow.bNeedResize = true;
        }
        break;
      }
    }
  }
  if(pWindow.bNeedResize) {
    pWindow.bNeedResize = false;
    PEvent pEvent{PEventType::WindowResize, &pWindow};
    pEvent.windowResize.width = pWindow.width;
    pEvent.windowResize.height = pWindow.height;
    Platform::PushEvent(pEvent);
  }
}

namespace Platform {

  void WindowInit(uint32 Width, uint32 Height, cstring Title) {
    Display* dpy = GetApi().display;
    Window root = GetApi().root;

    Visual* visual = GetApi().visualInfo->visual;
    int depth = GetApi().visualInfo->depth;

    int windowMask = ExposureMask | StructureNotifyMask | FocusChangeMask;
    int inputMask = PointerMotionMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask;
    int attrsMask = CWEventMask | CWColormap;

    int posX = CopyFromParent;
    int posY = CopyFromParent;

    XSetWindowAttributes attrs = {};
    attrs.colormap = GetApi().colormap;
    attrs.event_mask = windowMask | inputMask;
    Window win = XCreateWindow(dpy, root, posX, posY, Width, Height, 0, depth, InputOutput, visual, attrsMask, &attrs);

    XClassHint hint{};
    hint.res_class = const_cast<char*>(Title);
    hint.res_name = const_cast<char*>(PROJECT_NAME);
    XSetClassHint(dpy, win, &hint);

    Atom wmDelete = XInternAtom(dpy, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(dpy, win, &wmDelete, true);
    XStoreName(dpy, win, Title);
    XMapWindow(dpy, win);
    glXMakeContextCurrent(dpy, win, win, GetApi().context);

    PWindow& pWindow = GetApi().mainWindow;
    pWindow.width = Width;
    pWindow.height = Height;
    pWindow.title = Title;
    pWindow.bShouldClose = false;
    pWindow.wmDelete = wmDelete;
    pWindow.win = win;

    PEvent pEvent{PEventType::WindowResize, &pWindow};
    pEvent.windowResize.width = pWindow.width;
    pEvent.windowResize.height = pWindow.height;
    Platform::PushEvent(pEvent);
  }

  void WindowClose() {
    GetApi().mainWindow.bShouldClose = true;
  }

  bool WindowShouldClose() {
    return GetApi().mainWindow.bShouldClose;
  }

  bool WindowIsFullcreen() {
    return GetApi().mainWindow.bFullscreen;
  }

  void WindowSetFullscreen(bool bFullscreen) {
    Display* dpy = GetApi().display;
    PWindow& pWin = GetApi().mainWindow;
    XEvent event;
    if(pWin.bFullscreen == bFullscreen) {
      return;
    }
    pWin.bFullscreen = bFullscreen;
    Atom wmState = XInternAtom(dpy, "_NET_WM_STATE", false);
    Atom wmFullscreen = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", false);
    memset(&event, 0, sizeof(event));
    event.type = ClientMessage;
    event.xclient.window = pWin.win;
    event.xclient.message_type = wmState;
    event.xclient.format = 32;
    event.xclient.data.l[0] = (bFullscreen) ? 1 : 0;
    event.xclient.data.l[1] = wmFullscreen;
    event.xclient.data.l[2] = 0;
    event.xclient.data.l[3] = 1;
    XSendEvent(dpy, DefaultRootWindow(dpy), false, SubstructureRedirectMask | SubstructureNotifyMask, &event);
  }

  bool WindowIsMouseCaptured() {
    return GetApi().mainWindow.bMouseCaptured;
  }

  void WindowCaptureMouse(bool bCapture) {
    Display* dpy = GetApi().display;
    PWindow& pWindow = GetApi().mainWindow;
    if(pWindow.bMouseCaptured == bCapture) {
      return;
    }
    pWindow.bMouseCaptured = bCapture;
    if(bCapture) {
      Window root;
      Window child;
      int32 posX = 0;
      int32 posY = 0;
      int32 rootX = 0;
      int32 rootY = 0;
      uint32 mask = 0;
      XQueryPointer(dpy, pWindow.win, &root, &child, &rootX, &rootY, &posX, &posY, &mask);
      pWindow.lastMousePosX = posX;
      pWindow.lastMousePosY = posY;
    }
    InternalCaptureMouse(bCapture);
  }

  void WindowGetSize(uint32& OutWidth, uint32& OutHeight) {
    PWindow& pWindow = GetApi().mainWindow;
    OutWidth = pWindow.width;
    OutHeight = pWindow.height;
  }

  void WindowSetIcon(cstring Path) {
    if(!Path) {
      return;
    }

    int32 srcW, srcH, channels;
    uint8* src = stbi_load(Path, &srcW, &srcH, nullptr, 4);
    if(!src) {
      return;
    }

    const int32 iconSizes[] = {16, 32, 48, 64, 128, 256};
    const int32 sizeCount = sizeof(iconSizes) / sizeof(iconSizes[0]);
    int64 totalPixels = 0;
    for(int32 i = 0; i < sizeCount; i++) {
      totalPixels += (int64)iconSizes[i] * iconSizes[i] + 2;
    }

    uint64* iconData = new uint64[totalPixels];
    int64 offset = 0;
    for(int32 i = 0; i < sizeCount; i++) {
      int32 dstW = iconSizes[i];
      int32 dstH = iconSizes[i];
      uint8* dst = new uint8[dstW * dstH * 4];
      stbir_resize_uint8_linear(src, srcW, srcH, 0, dst, dstW, dstH, 0, STBIR_RGBA);
      iconData[offset++] = dstW;
      iconData[offset++] = dstH;
      for(int32 y = 0; y < dstH; y++) {
        for(int32 x = 0; x < dstW; x++) {
          uint8 r = dst[(y * dstW + x) * 4 + 0];
          uint8 g = dst[(y * dstW + x) * 4 + 1];
          uint8 b = dst[(y * dstW + x) * 4 + 2];
          uint8 a = dst[(y * dstW + x) * 4 + 3];
          iconData[offset++] = ((uint64)a << 24) | ((uint64)r << 16) | ((uint64)g << 8) | (uint64)b;
        }
      }
      delete[] dst;
    }
    stbi_image_free(src);

    Display* dpy = GetApi().display;
    Window win = GetApi().mainWindow.win;
    Atom net_wm_icon = XInternAtom(dpy, "_NET_WM_ICON", False);
    Atom cardinal = XInternAtom(dpy, "CARDINAL", False);
    XChangeProperty(dpy, win, net_wm_icon, cardinal, 32, PropModeReplace, reinterpret_cast<unsigned char*>(iconData), (int)totalPixels);
    XFlush(dpy);
    delete[] iconData;
  }

  uint32 GraphicInitOpenGL() {
    constexpr int32 Major = 3;
    constexpr int32 Minor = 3;
    Display* dpy = GetApi().display;
    Window root = GetApi().root;
    int32 scr = GetApi().screen;

    int32 colorBits = 8;
    int32 depthBits = 24;

    int32 fbAttribs[] = {
        GLX_X_RENDERABLE, True,                               //
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT | GLX_PBUFFER_BIT,  //
        GLX_RENDER_TYPE, GLX_RGBA_BIT,                        //
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,                    //
        GLX_DOUBLEBUFFER, true,                               //
        GLX_STENCIL_SIZE, colorBits,                          //
        GLX_RED_SIZE, colorBits,                              //
        GLX_GREEN_SIZE, colorBits,                            //
        GLX_BLUE_SIZE, colorBits,                             //
        GLX_DEPTH_SIZE, depthBits,                            //
        None                                                  //
    };

    int32 glxFlags = 0;
#ifdef DEV_MODE
    glxFlags |= GLX_CONTEXT_DEBUG_BIT_ARB;
#endif
    int32 contextAttribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, Major,                            //
        GLX_CONTEXT_MINOR_VERSION_ARB, Minor,                            //
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,  //
        GLX_CONTEXT_FLAGS_ARB, glxFlags,                                 //
        None                                                             //
    };

    int32 pBufferAttribs[] = {
        GLX_PBUFFER_WIDTH, 1,   //
        GLX_PBUFFER_HEIGHT, 1,  //
        None                    //
    };

    int32 fbCount = 0;
    GLXFBConfig* fbConfigs = glXChooseFBConfig(dpy, scr, fbAttribs, &fbCount);
    if(!fbConfigs || fbCount == 0) {
      return 0;
    }

    GLXFBConfig bestConfig = NULL;
    XVisualInfo* bestVisual = NULL;
    int bestScore = -1;

    auto ScoreFBConfig = [](Display* dpy, GLXFBConfig config) -> int {
      int score = 0;
      int samples = 0;
      int depth = 0;
      int stencil = 0;
      int doubleBuffer = 0;
      glXGetFBConfigAttrib(dpy, config, GLX_SAMPLES, &samples);
      score += samples * 1000;
      glXGetFBConfigAttrib(dpy, config, GLX_DEPTH_SIZE, &depth);
      score += depth;
      glXGetFBConfigAttrib(dpy, config, GLX_STENCIL_SIZE, &stencil);
      score += stencil;
      glXGetFBConfigAttrib(dpy, config, GLX_DOUBLEBUFFER, &doubleBuffer);
      if(doubleBuffer) {
        score += 1000;
      }
      return score;
    };

    for(int c = 0; c < fbCount; c++) {
      XVisualInfo* vi = glXGetVisualFromFBConfig(dpy, fbConfigs[c]);
      if(!vi) {
        continue;
      }
      int score = ScoreFBConfig(dpy, fbConfigs[c]);
      if(score > bestScore) {
        bestScore = score;
        bestConfig = fbConfigs[c];
        if(bestVisual) {
          XFree(bestVisual);
        }
        bestVisual = vi;
      } else {
        XFree(vi);
      }
    }

    if(bestVisual == NULL) {
      for(int c = 0; c < fbCount; c++) {
        bestVisual = glXGetVisualFromFBConfig(dpy, fbConfigs[c]);
        if(bestVisual) {
          bestConfig = fbConfigs[c];
          break;
        }
      }
    }
    XFree(fbConfigs);
    if(!bestVisual) {
      return 0;
    }

    auto glXSwapIntervalEXT = reinterpret_cast<PFNGLXSWAPINTERVALEXTPROC>(glXGetProcAddressARB(((const GLubyte*)"glXSwapIntervalEXT")));
    auto glXCreateContextAttribsARB = reinterpret_cast<PFNGLXCREATECONTEXTATTRIBSARBPROC>(glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB"));
    if(!glXCreateContextAttribsARB) {
      XFree(bestVisual);
      return 0;
    }

    GLXPbuffer pBuffer = glXCreatePbuffer(dpy, bestConfig, pBufferAttribs);
    if(pBuffer == 0) {
      return 0;
    }
    GLXContext ctx = glXCreateContextAttribsARB(dpy, bestConfig, nullptr, True, contextAttribs);
    glXMakeContextCurrent(dpy, pBuffer, pBuffer, ctx);

    using FOnGLLoader = void* (*)(const unsigned char*);
    if(!ApiGLLinuxLoadFunctions((FOnGLLoader)glXGetProcAddressARB)) {
      return 0;
    }

    GetApi().glXSwapIntervalEXT = glXSwapIntervalEXT;
    GetApi().visualInfo = bestVisual;
    GetApi().context = ctx;
    GetApi().colormap = XCreateColormap(dpy, root, bestVisual->visual, AllocNone);
    return 33;  /// OpenGL 3.3 Core Profile
  }

  void LogPrint(ELogLevel Level, cstring FuncName, cstring Context, cstring Format, va_list Args) {
    cstring logTag = "";
    cstring logColor = "";

    char buffer[FLog::BUFFER_SIZE] = "";
    uint64 offset = 0;

    if(Format == NULL) {
      fprintf(stderr, "Log Format invalid\n");
      return;
    }

    switch(Level) {
      case ELogLevel::Info: {
        logTag = "[LOG INFO]";
        logColor = "\e[97m";
        break;
      }

      case ELogLevel::Alert: {
        logTag = "[LOG ALERT]";
        logColor = "\e[93m";
        break;
      }

      case ELogLevel::Success: {
        logTag = "[LOG SUCCESS]";
        logColor = "\e[32m";
        break;
      }

      case ELogLevel::Warning: {
        logTag = "[LOG WARNING]";
        logColor = "\e[33m";
        break;
      }

      case ELogLevel::Error: {
        logTag = "[LOG ERROR]";
        logColor = "\e[91m";
        break;
      }

      case ELogLevel::Fatal: {
        logTag = "[LOG FATAL]";
        logColor = "\e[31m";
        break;
      }
    }

    if(Level != ELogLevel::Info && FuncName != NULL) {
      offset = snprintf(buffer, sizeof(buffer), "%s%s %s() => ", logColor, logTag, FuncName);
    } else {
      offset = snprintf(buffer, sizeof(buffer), "%s%s => ", logColor, logTag);
    }

    if(offset < sizeof(buffer)) {
      offset += vsnprintf(buffer + offset, sizeof(buffer) - offset, Format, Args);
    }

    if(offset < sizeof(buffer)) {
      if(Level != ELogLevel::Info && Level != ELogLevel::Alert && Context != NULL) {
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, " -> %s%s", Context, "\e[0m");
      } else {
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "%s", "\e[m");
      }
    }

    puts(buffer);
  }
}  // namespace Platform

// Internal Functions //======================================================================================//
void InternalCaptureMouse(bool bCapture) {
  FApi& api = GetApi();
  Window win = api.mainWindow.win;
  Display* dpy = api.display;

  if(api.bMouseCaptured == bCapture) {
    return;
  }
  api.bMouseCaptured = bCapture;
  if(bCapture) {
    //XDefineCursor(dpy, win, api.invisibleCursor);
    const int maxAttempts = 10;
    const useconds_t retryDelay = 10000;
    int32 res;
    int32 attempts = 0;
    do {
      res = XGrabPointer(dpy,
          win,                                                      //
          True,                                                     //
          ButtonPressMask | ButtonReleaseMask | PointerMotionMask,  //
          GrabModeAsync,                                            //
          GrabModeAsync,                                            //
          win,                                                      //
          None,                                                     //
          CurrentTime                                               //
      );
      attempts++;
      if(res != GrabSuccess && attempts < maxAttempts) {
        usleep(retryDelay);
      }
    } while(res != GrabSuccess && attempts < maxAttempts);
    if(res != GrabSuccess) {
      UE_ERROR("Failed to grab pointer after %d attempts", attempts);
      api.bMouseCaptured = false;
    }
  } else {
    api.bMouseCaptured = false;
    XUndefineCursor(dpy, win);
    XUngrabPointer(dpy, CurrentTime);
    int32 posX = api.mainWindow.lastMousePosX;
    int32 posY = api.mainWindow.lastMousePosY;
    XWarpPointer(dpy, None, win, 0, 0, 0, 0, posX, posY);
  }
}

#endif  // PLATFORM_LINUX
