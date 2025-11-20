#pragma once

#include "Bar.h"
#include "Track.h"

class Chart {
  public:
    Chart(std::vector<Bar*>);
    Chart();
    ~Chart();
    int AddTrack(Track*);
    int AddBar(Bar*);
    const std::vector<Bar*> GetBars() { return bars; }

  private:
    std::vector<Bar*> bars;
};
