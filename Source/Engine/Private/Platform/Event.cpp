#include "Platform/Event.h"
#include "Core/Log.h"

#include <mutex>

static constexpr uint32 EVENT_QUEUE_CAPACITY = 512;

struct FEventQueue {
  PEvent events[EVENT_QUEUE_CAPACITY];
  uint32 read = 0;
  uint32 write = 0;
  uint32 count = 0;
  std::mutex mutex;
};

static FEventQueue SEventQueue{};

namespace Platform {

  bool PushEvent(const PEvent& Event) {
    std::lock_guard<std::mutex> lock(SEventQueue.mutex);

    UE_ASSERT(SEventQueue.count < EVENT_QUEUE_CAPACITY);
    if(SEventQueue.count >= EVENT_QUEUE_CAPACITY) {
      return false;
    }

    SEventQueue.events[SEventQueue.write] = Event;
    SEventQueue.write = (SEventQueue.write + 1) % EVENT_QUEUE_CAPACITY;
    ++SEventQueue.count;
    return true;
  }

  bool NextEvent(PEvent& OutEvent) {
    std::lock_guard<std::mutex> lock(SEventQueue.mutex);

    if(SEventQueue.count == 0) {
      return false;
    }

    OutEvent = SEventQueue.events[SEventQueue.read];
    SEventQueue.read = (SEventQueue.read + 1) % EVENT_QUEUE_CAPACITY;
    --SEventQueue.count;
    return true;
  }

}  // namespace Platform
