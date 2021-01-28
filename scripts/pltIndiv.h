#ifndef PLTINDIV_H
#define PLTINDIV_H

#include <string>

extern bool debug;

enum BeamType{
  beam_diffuser,
  beam_collimator,
  beam_top_diffuser,
  beam_monitor,
  beam_all,
};

void plotUK(BeamType beam, int weeks, bool weekSwitch);
void topDiff(int weeks, bool weekSwitch);
void monPlot(BeamType beam, int weeks, bool weekSwitch);

#endif
