#ifdef PLATFORM_WINDOWS

#include "Platform/Platform.h"
#include "Platform/Event.h"
#include <windows.h>
#include <windowsx.h>
#include <GL/glcorearb.h>
#include <GL/wglext.h>

#define IDI_APP_ICON 100

struct PWindow;

// ApiGL.cpp
extern bool ApiWindowsLoadGLFunctions();

// PInput.cpp
extern bool ApiRawInputInit(HWND hWindow);
extern void ApiRawInputUpdateKeyMap();
extern void ApiRawInputPollEvent(PWindow* Window, HRAWINPUT RawInput);

static LRESULT InternalWinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static void InternalCaptureMouse(PWindow* Window, bool bCapture);

struct PWindow {
  HDC hDevice;
  HWND hWindow;
  cstring title;
  uint32 width;
  uint32 height;
  int32 lastMousePosX;
  int32 lastMousePosY;
  int32 style;
  bool bShow;
  bool bFullscreen;
  bool bShouldClose;
  bool bFocused;
  bool bMouseCaptured;
  bool bNeedsResize;
  bool bNeedCapture;
  WINDOWPLACEMENT placement;
};

struct FApi {
  HGLRC hContext;
  PWindow mainWindow;
  int32 pixelFormatAttribs[16];
  PIXELFORMATDESCRIPTOR pfd;
  PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
  PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
  bool bMouseCaptured{false};
};

static FApi& GetApi() {
  static FApi api{};
  return api;
}

namespace Platform {

  void WindowInit(uint32 Width, uint32 Height, cstring Title) {
    FApi& api = GetApi();
    PWindow& pWindow = api.mainWindow;
    if(pWindow.hWindow) {
      return;
    }
    cstring className = "GameWindow";
    HINSTANCE hInstance = GetModuleHandleA(NULL);
    uint32 style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    WNDCLASSEXA wc;
    memset(&wc, 0, sizeof(wc));
    if(!GetClassInfoExA(hInstance, className, &wc)) {
      wc.cbSize = sizeof(WNDCLASSEXA);
      wc.style = CS_OWNDC;
      wc.hInstance = hInstance;
      wc.lpszClassName = className;
      wc.lpfnWndProc = &InternalWinProc;
      wc.hIcon = LoadIconA(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
      wc.hIconSm = LoadIconA(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
      wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
      wc.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME + 1);
      UE_CHECK(RegisterClassExA(&wc) != 0);
    }
    RECT winRect = {0, 0, (LONG)Width, (LONG)Height};
    AdjustWindowRect(&winRect, style, false);
    int32 w = winRect.right - winRect.left;
    int32 h = winRect.bottom - winRect.top;
    HWND hWin = CreateWindowExA(0, className, Title, style, CW_USEDEFAULT, CW_USEDEFAULT, w, h, NULL, NULL, hInstance, &pWindow);
    HDC dc = GetDC(hWin);
    UE_CHECK(hWin && hWin);

    pWindow.width = Width;
    pWindow.height = Height;
    pWindow.title = Title;
    pWindow.style = style;
    pWindow.hWindow = hWin;
    pWindow.hDevice = dc;
    pWindow.bMouseCaptured = false;
    pWindow.bFullscreen = false;
    pWindow.bShouldClose = false;
    pWindow.bShow = false;
    pWindow.style = GetWindowLongA(hWin, GWL_STYLE);
    GetWindowPlacement(hWin, &pWindow.placement);

    if(api.hContext) {
      int32 pixelFormat = 0;
      uint32 numPixelFormat = 0;
      api.wglChoosePixelFormatARB(dc, api.pixelFormatAttribs, NULL, 1, &pixelFormat, (UINT*)&numPixelFormat);
      UE_CHECK(numPixelFormat > 0);
      UE_CHECK(SetPixelFormat(dc, pixelFormat, &api.pfd));
      UE_CHECK(wglMakeCurrent(dc, api.hContext));
    }
  }

  void WindowTerm() {
    FApi& api = GetApi();
    PWindow& pWindow = api.mainWindow;
    wglMakeCurrent(0, 0);
    wglDeleteContext(api.hContext);
    ReleaseDC(pWindow.hWindow, pWindow.hDevice);
    DestroyWindow(pWindow.hWindow);
  }

  void WindowPollEvent() {
    MSG msg{};
    while(PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessageA(&msg);
    }
    PWindow& window = GetApi().mainWindow;
    if(window.bNeedsResize) {
      window.bNeedsResize = false;
      PEvent pResize = {PEventType::WindowResize, &window};
      pResize.windowResize.width = window.width;
      pResize.windowResize.height = window.height;
      Platform::PushEvent(pResize);
    }
    if(window.bNeedCapture) {
      window.bNeedCapture = false;
      RECT rect;
      GetClientRect(window.hWindow, &rect);
      POINT point = {rect.left, rect.top};
      ClientToScreen(window.hWindow, &point);
      OffsetRect(&rect, point.x - rect.left, point.y - rect.top);
      ClipCursor(&rect);
    }
  }

  void WindowSwapBuffers() {
    SwapBuffers(GetApi().mainWindow.hDevice);
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
    PWindow& win = GetApi().mainWindow;
    if(win.bFullscreen == bFullscreen) {
      return;
    }
    bool bCapture = Platform::WindowIsMouseCaptured();
    Platform::WindowCaptureMouse(false);
    win.bFullscreen = bFullscreen;
    if(bFullscreen) {
      GetWindowPlacement(win.hWindow, &win.placement);
      win.style = GetWindowLongA(win.hWindow, GWL_STYLE);
      LONG_PTR fullStyle = win.style & ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
      MONITORINFO mi = {};
      mi.cbSize = sizeof(MONITORINFO);
      GetMonitorInfoA(MonitorFromWindow(win.hWindow, MONITOR_DEFAULTTONEAREST), &mi);
      int32 width = mi.rcMonitor.right - mi.rcMonitor.left;
      int32 height = mi.rcMonitor.bottom - mi.rcMonitor.top;
      int32 posX = mi.rcMonitor.left;
      int32 posY = mi.rcMonitor.top;
      SetWindowLongPtrA(win.hWindow, GWL_STYLE, fullStyle);
      SetWindowPos(win.hWindow, HWND_TOP, posX, posY, width, height, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    } else {
      SetWindowLongPtrA(win.hWindow, GWL_STYLE, win.style);
      SetWindowPlacement(win.hWindow, &win.placement);
    }
    Platform::WindowCaptureMouse(bCapture);
  }

  bool WindowIsMouseCaptured() {
    return GetApi().mainWindow.bMouseCaptured;
  }

  void WindowCaptureMouse(bool bCapture) {
    PWindow& win = GetApi().mainWindow;
    if(win.bMouseCaptured == bCapture) {
      return;
    }
    win.bMouseCaptured = bCapture;
    if(bCapture) {
      POINT point;
      GetCursorPos(&point);
      ScreenToClient(win.hWindow, &point);
      win.lastMousePosX = point.x;
      win.lastMousePosY = point.y;
    }
    InternalCaptureMouse(&win, bCapture);
  }

  void WindowGetSize(uint32& OutWidth, uint32& OutHeight) {
    PWindow& window = GetApi().mainWindow;
    OutWidth = window.width;
    OutHeight = window.height;
  }

  void WindowSetIcon(cstring Path) {}

  uint32 WindowInitOpenGL() {
    const uint32 glMajor = 3;
    const uint32 glMinor = 3;
    const BYTE colorBits = 32;
    const BYTE depthBits = 24;
    const BYTE stencilBits = 8;

    auto CreateDummyWindow = []() {
      cstring className = "DummyWindow";
      HINSTANCE hInstance = GetModuleHandleA(NULL);
      uint32 style = WS_OVERLAPPEDWINDOW;
      WNDCLASSEXA wc{};
      memset(&wc, 0, sizeof(wc));
      if(!GetClassInfoExA(hInstance, className, &wc)) {
        wc.cbSize = sizeof(WNDCLASSEXA);
        wc.style = CS_OWNDC;
        wc.hInstance = hInstance;
        wc.lpszClassName = className;
        wc.lpfnWndProc = &InternalWinProc;
        wc.hIcon = LoadIconA(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
        wc.hIconSm = LoadIconA(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
        wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME + 1);
        UE_CHECK(RegisterClassExA(&wc) != 0);
      }
      return CreateWindowExA(0, className, "DummyWindow", style, CW_USEDEFAULT, CW_USEDEFAULT, 100, 100, NULL, NULL, hInstance, nullptr);
    };

    HWND hWin = CreateDummyWindow();
    HDC dc = GetDC(hWin);
    UE_CHECK(hWin && dc);

    PIXELFORMATDESCRIPTOR pfd{};
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = colorBits;
    pfd.cDepthBits = depthBits;
    pfd.cStencilBits = stencilBits;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int32 legacyPF = ChoosePixelFormat(dc, &pfd);
    UE_CHECK(legacyPF);
    UE_CHECK(SetPixelFormat(dc, legacyPF, &pfd));

    HGLRC legacyContext = wglCreateContext(dc);
    UE_CHECK(legacyContext);
    UE_CHECK(wglMakeCurrent(dc, legacyContext));

    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    UE_ASSERT(wglChoosePixelFormatARB && wglCreateContextAttribsARB);

    wglMakeCurrent(0, 0);
    wglDeleteContext(legacyContext);
    ReleaseDC(hWin, dc);
    DestroyWindow(hWin);

    int32 pixelFormatAttribs[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,    //
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,    //
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,     //
        WGL_COLOR_BITS_ARB, colorBits,      //
        WGL_STENCIL_BITS_ARB, stencilBits,  //
        WGL_DEPTH_BITS_ARB, depthBits,      //
        0                                   //
    };

    int32 glFlags = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
#ifdef DEVELOPMENT_MODE
    glFlags |= WGL_CONTEXT_DEBUG_BIT_ARB;
#endif  // DEVELOPMENT_MODE

    int32 contextAttribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, glMajor,  //
        WGL_CONTEXT_MINOR_VERSION_ARB, glMinor,  //
        WGL_CONTEXT_FLAGS_ARB, glFlags,          //
        0                                        //
    };

    hWin = CreateDummyWindow();
    dc = GetDC(hWin);
    UE_CHECK(hWin && dc);

    int32 pixelFormat = 0;
    uint32 numPixelFormat = 0;
    wglChoosePixelFormatARB(dc, pixelFormatAttribs, NULL, 1, &pixelFormat, (UINT*)&numPixelFormat);
    UE_CHECK(numPixelFormat > 0);
    UE_CHECK(SetPixelFormat(dc, pixelFormat, &pfd));

    HGLRC hContext = wglCreateContextAttribsARB(dc, NULL, contextAttribs);
    UE_CHECK(hContext);
    UE_CHECK(wglMakeCurrent(dc, hContext));
    UE_CHECK(ApiWindowsLoadGLFunctions());

    ReleaseDC(hWin, dc);
    DestroyWindow(hWin);

    FApi& api = GetApi();
    memcpy(&api.pixelFormatAttribs, &pixelFormatAttribs, sizeof(pixelFormatAttribs));
    api.wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    api.wglChoosePixelFormatARB = wglChoosePixelFormatARB;
    api.hContext = hContext;
    api.pfd = pfd;

    UE_INFO("API:WIN32 Context => OpenGL 3.3 core profile");
    return 33;  /// OpenGL 3.3 Core Profile
  }

}  // namespace Platform

// Internal Functions //======================================================================================//
LRESULT InternalWinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
  PWindow* self = nullptr;
  if(Msg == WM_NCCREATE) {
    CREATESTRUCTA* cs = (CREATESTRUCTA*)lParam;
    self = (PWindow*)cs->lpCreateParams;
    SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)self);
    if(self) {
      ApiRawInputInit(self->hWindow);
    }
  } else {
    self = (PWindow*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
  }

  switch(Msg) {
    case WM_CLOSE: {
      self->bShouldClose = true;
      break;
    }
    case WM_SETFOCUS:
    case WM_KILLFOCUS: {
      bool bFocused = (Msg == WM_SETFOCUS);
      bool bCapture = (bFocused) ? self->bMouseCaptured : false;
      self->bFocused = bFocused;
      InternalCaptureMouse(self, bCapture);
      PEvent pWindowFocus = {PEventType::WindowFocus, self};
      pWindowFocus.windowFocus.bFocused = bFocused;
      Platform::PushEvent(pWindowFocus);
      break;
    }
    case WM_MOUSEMOVE: {
      PEvent pMouseMove = {PEventType::MousePos, self};
      pMouseMove.mousePos.posX = (int32)GET_X_LPARAM(lParam);
      pMouseMove.mousePos.posY = (int32)GET_Y_LPARAM(lParam);
      Platform::PushEvent(pMouseMove);
      break;
    }
    case WM_SIZE: {
      if(wParam == SIZE_MINIMIZED) {
        break;
      }
      uint32 width = LOWORD(lParam);
      uint32 height = HIWORD(lParam);
      if(width != self->width || height != self->height) {
        self->width = width;
        self->height = height;
        self->bNeedsResize = true;
      }
      break;
    }
    case WM_INPUTLANGCHANGE: {
      ApiRawInputUpdateKeyMap();
      break;
    }
    case WM_INPUT: {
      ApiRawInputPollEvent(self, (HRAWINPUT)lParam);
      break;
    }
    default: {
      return DefWindowProcA(hWnd, Msg, wParam, lParam);
    }
  }

  return 0;
}

static void InternalCaptureMouse(PWindow* Window, bool bCapture) {
  if(GetApi().bMouseCaptured == bCapture) {
    return;
  }
  GetApi().bMouseCaptured = bCapture;
  if(bCapture) {
    RECT rect;
    GetClientRect(Window->hWindow, &rect);
    POINT point = {rect.left, rect.top};
    ClientToScreen(Window->hWindow, &point);
    OffsetRect(&rect, point.x - rect.left, point.y - rect.top);
    ShowCursor(false);
    Window->bNeedCapture = true;
  } else {
    ClipCursor(NULL);
    ShowCursor(true);
    POINT point = {Window->lastMousePosX, Window->lastMousePosY};
    ClientToScreen(Window->hWindow, &point);
    SetCursorPos(point.x, point.y);
  }
  RECT current;
  GetClipCursor(&current);
}

#endif  // PLATFORM_WINDOWS
