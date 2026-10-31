#pragma once
#include "Math/Utils.h"

struct FColor {
  uint8 r;
  uint8 g;
  uint8 b;
  uint8 a;
  static FColor Make(uint8 R, uint8 G, uint8 B, uint8 A) {
    return FColor{R, G, B, A};
  }
  static FColor Make(float R, float G, float B, float A) {
    uint8 r = (uint8)FMath::Clamp(R * 255, 0, 255);
    uint8 g = (uint8)FMath::Clamp(G * 255, 0, 255);
    uint8 b = (uint8)FMath::Clamp(B * 255, 0, 255);
    uint8 a = (uint8)FMath::Clamp(A * 255, 0, 255);
    return FColor{r, g, b, a};
  }
};

struct FColors {
  // Basic
  static constexpr FColor WHITE = {255, 255, 255, 255};
  static constexpr FColor BLACK = {0, 0, 0, 255};

  static constexpr FColor RED_LIGHT = {255, 102, 102, 255};
  static constexpr FColor RED = {255, 0, 0, 255};
  static constexpr FColor RED_DARK = {153, 0, 0, 255};

  static constexpr FColor GREEN_LIGHT = {102, 255, 102, 255};
  static constexpr FColor GREEN = {0, 255, 0, 255};
  static constexpr FColor GREEN_DARK = {0, 153, 0, 255};

  static constexpr FColor BLUE_LIGHT = {102, 102, 255, 255};
  static constexpr FColor BLUE = {0, 0, 255, 255};
  static constexpr FColor BLUE_DARK = {0, 0, 153, 255};

  static constexpr FColor YELLOW_LIGHT = {255, 255, 102, 255};
  static constexpr FColor YELLOW = {255, 255, 0, 255};
  static constexpr FColor YELLOW_DARK = {153, 153, 0, 255};

  static constexpr FColor ORANGE_LIGHT = {255, 178, 102, 255};
  static constexpr FColor ORANGE = {255, 128, 0, 255};
  static constexpr FColor ORANGE_DARK = {153, 77, 0, 255};

  static constexpr FColor PURPLE_LIGHT = {204, 153, 255, 255};
  static constexpr FColor PURPLE = {128, 0, 255, 255};
  static constexpr FColor PURPLE_DARK = {77, 0, 153, 255};

  static constexpr FColor CYAN_LIGHT = {102, 255, 255, 255};
  static constexpr FColor CYAN = {0, 255, 255, 255};
  static constexpr FColor CYAN_DARK = {0, 153, 153, 255};

  static constexpr FColor PINK_LIGHT = {255, 153, 204, 255};
  static constexpr FColor PINK = {255, 0, 128, 255};
  static constexpr FColor PINK_DARK = {153, 0, 77, 255};

  static constexpr FColor GRAY_LIGHT = {204, 204, 204, 255};
  static constexpr FColor GRAY = {128, 128, 128, 255};
  static constexpr FColor GRAY_DARK = {64, 64, 64, 255};

  static constexpr FColor SKY_LIGHT = {135, 206, 250, 255};
  static constexpr FColor SKY = {70, 160, 220, 255};
  static constexpr FColor SKY_DARK = {30, 90, 150, 255};

  static constexpr FColor BROWN_LIGHT = {181, 120, 75, 255};
  static constexpr FColor BROWN = {139, 69, 19, 255};
  static constexpr FColor BROWN_DARK = {90, 45, 12, 255};

  // Metal / Materials
  static constexpr FColor COPPER_LIGHT = {224, 143, 106, 255};
  static constexpr FColor COPPER = {184, 115, 51, 255};
  static constexpr FColor COPPER_DARK = {115, 65, 30, 255};

  static constexpr FColor BRONZE_LIGHT = {205, 160, 95, 255};
  static constexpr FColor BRONZE = {150, 105, 55, 255};
  static constexpr FColor BRONZE_DARK = {90, 60, 30, 255};

  static constexpr FColor GOLD_LIGHT = {255, 230, 130, 255};
  static constexpr FColor GOLD = {212, 175, 55, 255};
  static constexpr FColor GOLD_DARK = {130, 95, 20, 255};

  static constexpr FColor SILVER_LIGHT = {235, 235, 235, 255};
  static constexpr FColor SILVER = {192, 192, 192, 255};
  static constexpr FColor SILVER_DARK = {105, 105, 105, 255};

  static constexpr FColor STEEL_LIGHT = {180, 190, 200, 255};
  static constexpr FColor STEEL = {110, 125, 140, 255};
  static constexpr FColor STEEL_DARK = {55, 65, 75, 255};

  static constexpr FColor IRON_LIGHT = {145, 145, 145, 255};
  static constexpr FColor IRON = {80, 80, 80, 255};
  static constexpr FColor IRON_DARK = {35, 35, 35, 255};

  // Natural / Material
  static constexpr FColor SAND_LIGHT = {245, 220, 160, 255};
  static constexpr FColor SAND = {194, 178, 128, 255};
  static constexpr FColor SAND_DARK = {125, 105, 65, 255};

  static constexpr FColor WOOD_LIGHT = {205, 155, 95, 255};
  static constexpr FColor WOOD = {145, 90, 45, 255};
  static constexpr FColor WOOD_DARK = {75, 40, 20, 255};

  static constexpr FColor GRASS_LIGHT = {130, 200, 90, 255};
  static constexpr FColor GRASS = {75, 150, 50, 255};
  static constexpr FColor GRASS_DARK = {35, 90, 25, 255};

  static constexpr FColor STONE_LIGHT = {180, 175, 165, 255};
  static constexpr FColor STONE = {120, 115, 105, 255};
  static constexpr FColor STONE_DARK = {65, 62, 58, 255};
};
