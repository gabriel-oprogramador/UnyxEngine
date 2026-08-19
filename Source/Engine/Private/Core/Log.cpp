#include "Core/Log.h"
#include "Platform/Platform.h"

void FLog::PrintLogInfo(ELogLevel Level, cstring FuncName, cstring Context, cstring Format, ...) {
  va_list Args;
  va_start(Args, Format);
  Platform::LogPrint(Level, FuncName, Context, Format, Args);
  va_end(Args);
}
