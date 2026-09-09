#ifdef PLATFORM_WEB
#include "Core/Log.h"
#include <emscripten.h>

namespace Platform {

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

#endif  // PLATFORM_WEB
