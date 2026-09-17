#ifdef PLATFORM_LINUX
#include "Platform/Platform.h"

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

#endif  // PLATFORM_LINUX