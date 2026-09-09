#include "GameFramework/Game.h"
#include "Platform/Platform.h"
#include "Math/Vector2.h"

uint32 FGame::WindowWidth() {
  uint32 width = 0;
  uint32 height = 0;
  Platform::WindowGetSize(width, height);
  return width;
}

uint32 FGame::WindowHeight() {
  uint32 width = 0;
  uint32 height = 0;
  Platform::WindowGetSize(width, height);
  return height;
}

FVector2 FGame::WindowSize() {
  uint32 width = 0;
  uint32 height = 0;
  Platform::WindowGetSize(width, height);
  return FVector2{(float)width, (float)height};
}

void FGame::ToggleFullscreen() {
  Platform::WindowSetFullscreen(!Platform::WindowIsFullcreen());
}

void FGame::QuitGame() {
  Platform::WindowClose();
}
