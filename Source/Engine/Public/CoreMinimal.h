#pragma once

// ============================================================================
// CoreMinimal.h
//
// Minimal public core for Game/Plugin code.
//
// This is NOT the Engine Core entry point.
// Engine code should use "Core/Core.h" instead.
//
// Intended for:
//   - Game
//   - Plugin
//
// Avoid adding subsystem-specific APIs here.
// ============================================================================

#define GAME_API extern "C"

#include "Core/Core.h"
#include "Math/Math.h"

#include "Runtime/World.h"
#include "Runtime/GameInstance.h"
