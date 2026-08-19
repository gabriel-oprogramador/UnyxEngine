#include "Runtime/Schedule.h"

void FSchedule::RegisterSystem(EStage Stage, FSystem System) {
  switch(Stage) {
    case EStage::BeginPlay: sysBeginPlay.Add(System); break;
    case EStage::EndPlay: sysEndPlay.Add(System); break;
    case EStage::Update: sysUpdate.Add(System); break;
  }
}

void FSchedule::RunStage(EStage Stage, FWorld& World) const {
  switch(Stage) {
    case EStage::BeginPlay: {
      for(auto& sys : sysBeginPlay) {
        sys.OnExecute(World);
      }
      break;
    }
    case EStage::EndPlay: {
      for(auto& sys : sysEndPlay) {
        sys.OnExecute(World);
      }
      break;
    }
    case EStage::Update: {
      for(auto& sys : sysUpdate) {
        sys.OnExecute(World);
      }
      break;
    }
  }
}
