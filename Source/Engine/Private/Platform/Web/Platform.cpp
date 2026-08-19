#ifdef PLATFORM_WEB

#include "Platform/Platform.h"
#include "Platform/Event.h"
#include <cstdio>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/html5_webgl.h>

#define CANVAS_ID         "#canvas"
#define TEXT_INPUT_ID     "#hidden-text"
#define GAME_CONTAINER_ID "#game-container"

// Source/Engine/Private/Runtime/App.cpp
extern void AppBootstrapInitialize();
extern void AppBootstrapTerminate();
extern void AppBootstrapUpdate();

struct PWindow {
  uint32 width{0};
  uint32 height{0};
  cstring title{nullptr};
  bool bFocused{false};
  bool bFullscreen{false};
  bool bNeedsResize{false};
  bool bMouseCaptured{false};
  bool bShouldClose{true};
};

struct FApi {
  EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context{};
  EmscriptenWebGLContextAttributes attribs{};
  PWindow mainWindow{};
  uint32 glMajor{0};
  uint32 glMinor{0};
  bool bMouseCaptured{false};
};

static FApi& GetApi() {
  static FApi api = {};
  return api;
}

// Web/Input.cpp
bool ShouldRecaptureMouse() {
  return GetApi().mainWindow.bMouseCaptured && !GetApi().bMouseCaptured;
}

// Source/Engine/Private/Platform/Web/Input.cpp
extern bool ApiInputRawInit(cstring CanvasID, PWindow* Window);

static void InternalShowMouse(bool bShow);

int main() {
  UE_INFO("Unyx Engine For Web");

  ApiInputRawInit(CANVAS_ID, &GetApi().mainWindow);
  AppBootstrapInitialize();

  auto MainLoop = []() {
    static PWindow& win = GetApi().mainWindow;
    if(win.bShouldClose) {
      AppBootstrapTerminate();
      emscripten_cancel_main_loop();
      return;
    }
    AppBootstrapUpdate();
  };

  auto FocusCallback = [](int Type, const EmscriptenFocusEvent* Event, void* UserData) {
    bool bFocused = (Type == EMSCRIPTEN_EVENT_FOCUS) ? true : false;
    GetApi().mainWindow.bFocused = bFocused;
    PEvent pEvent = {PEventType::WindowFocus, &GetApi().mainWindow};
    pEvent.windowFocus.bFocused = bFocused;
    Platform::PushEvent(pEvent);
    return true;
  };
  emscripten_set_focus_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_TRUE, FocusCallback);
  emscripten_set_blur_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_TRUE, FocusCallback);

  auto FullscreenCallback = [](int Type, const EmscriptenFullscreenChangeEvent* Event, void* UserData) {
    GetApi().mainWindow.bFullscreen = Event->isFullscreen;
    return true;
  };
  emscripten_set_fullscreenchange_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_TRUE, FullscreenCallback);

  auto PointerlockCallback = [](int Type, const EmscriptenPointerlockChangeEvent* Event, void* UserData) {
    GetApi().bMouseCaptured = Event->isActive;
    InternalShowMouse(!Event->isActive);
    return true;
  };
  emscripten_set_pointerlockchange_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_TRUE, PointerlockCallback);

  auto ResizeCallback = [](int Type, const EmscriptenUiEvent* Event, void* UserData) {
    PWindow* win = &GetApi().mainWindow;
    win->width = EM_ASM_INT({ return window.innerWidth; });
    win->height = EM_ASM_INT({ return window.innerHeight; });
    emscripten_set_element_css_size(CANVAS_ID, win->width, win->height);
    emscripten_set_canvas_element_size(CANVAS_ID, win->width, win->height);
    PEvent pEvent = {PEventType::WindowResize, win};
    pEvent.windowResize.width = win->width;
    pEvent.windowResize.height = win->height;
    Platform::PushEvent(pEvent);
    return true;
  };
  emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_TRUE, ResizeCallback);

  emscripten_set_main_loop(MainLoop, 0, true);

  return 0;
}

namespace Platform {

  void WindowInit(uint32 Width, uint32 Height, cstring Title) {
    PWindow& win = GetApi().mainWindow;
    EM_ASM_({ document.title = UTF8ToString($0); }, Title);
    uint32 width = EM_ASM_INT({ return window.innerWidth; });
    uint32 height = EM_ASM_INT({ return window.innerHeight; });
    emscripten_set_element_css_size(CANVAS_ID, width, height);
    emscripten_set_canvas_element_size(CANVAS_ID, width, height);
    win.bShouldClose = false;
    PEvent pEvent = {PEventType::WindowResize, &win};
    pEvent.windowResize.width = width;
    pEvent.windowResize.height = height;
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
    EMSCRIPTEN_RESULT result;
    if(bFullscreen) {
      result = emscripten_request_fullscreen(GAME_CONTAINER_ID, 1);
    } else {
      result = emscripten_exit_fullscreen();
    }
    if(result == EMSCRIPTEN_RESULT_SUCCESS) {
      GetApi().mainWindow.bFullscreen = bFullscreen;
    } else {
      UE_ERROR("Not Set Fullscreen:%d", result);
    }
  }

  bool WindowIsMouseCaptured() {
    return GetApi().bMouseCaptured;
  }

  void WindowCaptureMouse(bool bCapture) {
    if(GetApi().mainWindow.bMouseCaptured == bCapture) {
      return;
    }
    GetApi().mainWindow.bMouseCaptured = bCapture;
    if(bCapture) {
      emscripten_request_pointerlock(CANVAS_ID, false);
    } else {
      emscripten_exit_pointerlock();
    }
  }

  void WindowGetSize(uint32& OutWidth, uint32& OutHeight) {
    PWindow& pWindow = GetApi().mainWindow;
    OutWidth = pWindow.width;
    OutHeight = pWindow.height;
  }

  void WindowSetIcon(cstring Path) {}

  uint32 GraphicInitOpenGL() {
    FApi& api = GetApi();
    if(api.context != 0) {
      return 0;
    }
    emscripten_webgl_init_context_attributes(&api.attribs);
    api.attribs.majorVersion = 2;
    api.attribs.minorVersion = 0;
    api.attribs.depth = true;
    api.attribs.alpha = false;
    api.attribs.stencil = true;
    api.attribs.antialias = true;
    api.attribs.premultipliedAlpha = false;

    api.context = emscripten_webgl_create_context(CANVAS_ID, &api.attribs);
    UE_ASSERT(api.context != 0);
    emscripten_webgl_make_context_current(api.context);
    UE_INFO("API:Emscripten Context => WebGL:2.0 @ OpenGL:ES3.0");

    return 30;  // WebGL 2.0 => OpenGL ES 3.0
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
      case ELogLevel::Info:
        logTag = "[LOG INFO]";
        logColor = "color: white;";
        break;
      case ELogLevel::Alert:
        logTag = "[LOG ALERT]";
        logColor = "color: yellow;";
        break;
      case ELogLevel::Success:
        logTag = "[LOG SUCCESS]";
        logColor = "color: green;";
        break;
      case ELogLevel::Warning:
        logTag = "[LOG WARNING]";
        logColor = "color: yellow;";
        break;
      case ELogLevel::Error:
        logTag = "[LOG ERROR]";
        logColor = "color: red;";
        break;
      case ELogLevel::Fatal:
        logTag = "[LOG FATAL]";
        logColor = "color: darkred; font-weight: bold;";
        break;
    }

    if(Level != ELogLevel::Info && FuncName != NULL) {
      offset = snprintf(buffer, sizeof(buffer), "%s %s() => ", logTag, FuncName);
    } else {
      offset = snprintf(buffer, sizeof(buffer), "%s => ", logTag);
    }

    if(offset < sizeof(buffer)) {
      offset += vsnprintf(buffer + offset, sizeof(buffer) - offset, Format, Args);
    }

    if(offset < sizeof(buffer)) {
      if(Level != ELogLevel::Info && Level != ELogLevel::Alert && Context != NULL) {
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, " -> %s", Context);
      }
    }

    EM_ASM_(
        {
          var msg = UTF8ToString($0);
          var style = UTF8ToString($1);
          console.log("%c%s", style, msg);
        },
        buffer, logColor);
  }
}  // namespace Platform

// Internal Functions
void InternalShowMouse(bool bShow) {
  EM_ASM(
      {
        var canvas = document.getElementById('canvas');
        if(canvas) {
          canvas.style.cursor = $0 ? 'default' : 'none';
        }
      },
      bShow);
}

#endif  // PLATFORM_WEB
