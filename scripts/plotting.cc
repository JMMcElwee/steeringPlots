/* ---------------- main.cc ---------------- *
 * This will extract a variety of variables  *
 * from the diffuser root files for the UK   *
 * light injection system.                   *
 *                                           *
 * SOFTWARE REQUIRED:                        *
 * ---> ROOT                                 *
 * ---> position.h                           *
 * ---> rndFunc.h                            *
 *                                           *
 *                jmmcelwee1@sheffield.ac.uk *
 * ------------------------- J. McElwee ---- */

#include <iostream>
#include <vector>
#include <cmath>
#include <math.h>
#include <string>
#include <sstream>
#include <fstream>
#include <numeric>
#include <ctime>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <typeinfo>
#include <array>
#include <time.h>
#include <algorithm>
#include <typeinfo>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TVector3.h"
#include "TDatime.h"
#include "TStyle.h"
#include "TGraphErrors.h"
#include "TTimeStamp.h"

#include "pltIndiv.h"



int main(int argc, char *argv[]){


  BeamType beam = beam_all;
  int oNameSwitch = 0;
  std::string ldir = "/disk02/usr6/jmcelwee/monitoringData/plotting";
  std::string endname = "";
  std::string outname = "";
  int weeks = 3;
  bool weekSwitch = false;
  int opt;

  while ((opt = getopt(argc, argv, ":dctmo:w:l:")) != -1){
    switch (opt)
      {
      case 'd':
	beam = beam_diffuser;
	break;
      case 'c':
	beam = beam_collimator;
	break;
      case 't':
	beam = beam_top_diffuser;
	break;
      case 'm':
	beam = beam_monitor;
	break;
      case 'o':
	oNameSwitch = 1;
	outname = optarg;
	break;
      case 'w':
	weekSwitch = true;
	weeks = std::stoi(optarg);
	break;
      case 'l':
	ldir = optarg;
	break;
      case ':':
	printf("\033[1;31m[ERROR]\033[0m -%c requires an argument.\n",optopt);
	return 0;
      case '?':
	printf("\033[1;33m[ERROR]\033[0m -%c is an unknown argument... just ignoring it.\n",optopt);
	break;
      }
  }
  
  time_t timer;
  time(&timer);

  gStyle->SetCanvasColor(0);
  gStyle->SetTitleColor(0);
  gStyle->SetOptStat(0);  
  

  switch(beam)
    {
    case beam_diffuser:
      plotUK(beam_diffuser,weeks,weekSwitch);
      break;
    case beam_collimator:
      plotUK(beam_collimator,weeks,weekSwitch);
      break;
    case beam_top_diffuser:
      topDiff(weeks,weekSwitch);
      break;
    case beam_monitor:
      monPlot(beam_diffuser,weeks,weekSwitch);
      monPlot(beam_collimator,weeks,weekSwitch);
      break;
    default:
      plotUK(beam_diffuser,weeks,weekSwitch);
      plotUK(beam_collimator,weeks,weekSwitch);
      topDiff(weeks,weekSwitch);
      break;
    };
  
  
  return 0;
}


