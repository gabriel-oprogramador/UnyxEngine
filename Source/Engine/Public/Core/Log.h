#pragma once
#include "Core/BaseTypes.h"
#include <csignal>

#define NO_EXPAND(A)                   #A
#define STR(A)                         NO_EXPAND(A)
#define UE_LOG_CONTEXT                 STR(__FILE__) ":" STR(__LINE__)
#define UE_LOG(ELogLevel, Format, ...) FLog::PrintLogInfo(ELogLevel, __func__, UE_LOG_CONTEXT, Format, ##__VA_ARGS__)
#define UE_INFO(Format, ...)           FLog::PrintLogInfo(ELogLevel::Info, __func__, UE_LOG_CONTEXT, Format, ##__VA_ARGS__)
#define UE_ALERT(Format, ...)          FLog::PrintLogInfo(ELogLevel::Alert, __func__, UE_LOG_CONTEXT, Format, ##__VA_ARGS__)
#define UE_SUCCESS(Format, ...)        FLog::PrintLogInfo(ELogLevel::Success, __func__, UE_LOG_CONTEXT, Format, ##__VA_ARGS__)
#define UE_WARNING(Format, ...)        FLog::PrintLogInfo(ELogLevel::Warning, __func__, UE_LOG_CONTEXT, Format, ##__VA_ARGS__)
#define UE_ERROR(Format, ...)          FLog::PrintLogInfo(ELogLevel::Error, __func__, UE_LOG_CONTEXT, Format, ##__VA_ARGS__)
#define UE_FATAL(Format, ...)          FLog::PrintLogInfo(ELogLevel::Fatal, __func__, UE_LOG_CONTEXT, Format, ##__VA_ARGS__)

#if defined(_MSC_VER)
#define UE_DEBUG_BREAK() __debugbreak()
#elif defined(__clang__)
#define UE_DEBUG_BREAK() __builtin_debugtrap()
#elif defined(__GNUC__)
#define UE_DEBUG_BREAK() __builtin_trap()
#else
#define UE_DEBUG_BREAK() std::raise(SIGTRAP)
#endif

#if defined(SHIP_MODE)
#define UE_ASSERT(Expr) ((void)0)
#else
#define UE_ASSERT(Expr)                                        \
  do {                                                         \
    if(!(Expr)) {                                              \
      UE_LOG(ELogLevel::Fatal, "Assert Failure: [%s]", #Expr); \
      UE_DEBUG_BREAK();                                        \
    }                                                          \
  } while(0)

#endif

#define UE_CHECK(Expr)                                        \
  do {                                                        \
    if(!(Expr)) {                                             \
      UE_LOG(ELogLevel::Fatal, "Check Failure: [%s]", #Expr); \
      UE_DEBUG_BREAK();                                       \
    }                                                         \
  } while(0)

enum class ELogLevel : uint8 {
  Info,     //
  Alert,    //
  Success,  //
  Warning,  //
  Error,    //
  Fatal,    //
};

struct FLog {
  static constexpr usize BUFFER_SIZE = 2048;
  static void PrintLogInfo(ELogLevel Level, cstring FuncName, cstring Context, cstring Format, ...);
};
