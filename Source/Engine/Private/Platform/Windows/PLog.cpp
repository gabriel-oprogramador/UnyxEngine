#ifdef PLATFORM_WINDOWS

#include "Core/Log.h"
#include <windows.h>

struct FApi {
  HANDLE hConsole;
  int32 consoleDefaultAttribute;
  FApi() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    consoleDefaultAttribute = consoleInfo.wAttributes;
  }
};

static FApi& GetApi() {
  static FApi api{};
  return api;
}

namespace Platform {
  void LogPrint(ELogLevel Level, cstring FuncName, cstring Context, cstring Format, va_list Args) {
    cstring logTag = "";
    char buffer[FLog::BUFFER_SIZE] = "";
    uint64 offset = 0;

    enum class EColor {
      White = 7,       //
      Green = 10,      //
      Yellow = 14,     //
      Red = 12,        //
      Dark_red = 4,    //
      Dark_yellow = 6  //
    };
    EColor logColor = EColor::White;

    if(Format == NULL) {
      fprintf(stderr, "Log Format invalid\n");
      return;
    }

    switch(Level) {
      case ELogLevel::Info: {
        logTag = "[LOG INFO]";
        logColor = EColor::White;
        break;
      }

      case ELogLevel::Alert: {
        logTag = "[LOG ALERT]";
        logColor = EColor::Yellow;
        break;
      }

      case ELogLevel::Success: {
        logTag = "[LOG SUCCESS]";
        logColor = EColor::Green;
        break;
      }

      case ELogLevel::Warning: {
        logTag = "[LOG WARNING]";
        logColor = EColor::Dark_yellow;
        break;
      }

      case ELogLevel::Error: {
        logTag = "[LOG ERROR]";
        logColor = EColor::Red;
        break;
      }

      case ELogLevel::Fatal: {
        logTag = "[LOG FATAL]";
        logColor = EColor::Dark_red;
        break;
      }
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

    FApi& api = GetApi();
    SetConsoleTextAttribute(api.hConsole, static_cast<int32>(logColor));
    puts(buffer);
    SetConsoleTextAttribute(api.hConsole, api.consoleDefaultAttribute);
  }

}  // namespace Platform

#endif  // PLATFORM_WINDOWS
