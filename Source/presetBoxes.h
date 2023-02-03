#pragma once
#include "Statebox.h"
class Statebox;
namespace presets
{
  enum isEnabled
  {
    OFF,
    ON
  };

  enum FPS
  {
    FRAMES_60,
    VS,
    UNLIMITED
  };

  enum Construction
  {
    EMPTY,
    TRIANGLE,
    INCENTER,
    ORTHOCENTER
  };

  void stateboxesInit();

  void stateboxesApply();
}