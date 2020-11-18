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

#include "pltIndiv.h"
#include "createPlots.h"



void monPlot(BeamType beam, int weeks, bool weekSwitch){


  int oNameSwitch = 0;
  std::string ldir = "/disk02/usr6/jmcelwee/monitoringData/plotting";
  std::string endname = "";
  std::string outname = "";


  std::string beamType = "dif";
  switch(beam)
    {
    case beam_diffuser:
      beamType = "dif";
      break;
    case beam_collimator:
      beamType = "col";
      break;
    default:
      std::cout << "\033[31;1m[ERROR]\033[0m No beamtype has been supplied." << std::endl;
    };
  
  
  
  std::vector<std::string> fileList;
  for (int inj=0; inj<5;inj++){
    int injcount = inj+1;
    std::string fileEx = ldir + "/" +  beamType  + "_B" + std::to_string(inj+1) + ".dat";
    fileList.push_back(fileEx);
    std::cout << fileEx << std::endl;
  }

  
  time_t timer;
  time(&timer);

  gStyle->SetCanvasColor(0);
  gStyle->SetTitleColor(0);
  gStyle->SetOptStat(0);  

  std::vector<std::vector<float>> dateVecVec;
  std::vector<std::vector<float>> monQVecVec, errorMonVecVec;
  std::vector<std::vector<float>> normQVecVec, errorNormVecVec;
  
  for (int inj=0; inj<5;inj++){
    std::vector<int> run, subrun, year, month, day, hour, minute, second;
    std::vector<float> monQ, normQ, totQ, spotQ;
    std::ifstream file(fileList[inj]);
    std::cout << fileList[inj] << std::endl;
    extract_data(file,month,day,hour,minute,second,totQ,spotQ,monQ);


    int avenum = 4500;
    if (weekSwitch){
      avenum = 400*weeks;
    }

    std::vector<float> dateVec;
    std::vector<float> monDataPoints, monQVec, errorMonVec;
    std::vector<float> normDataPoints, normQVec, errorNormVec;
    float averageNorm = 0;
    float averageMon = 0;
    int count = 0;
    float errorMon, errorNorm;

    int time = 1594080000;
    if (weekSwitch){
      time = timer - 604800*weeks;
    }
    
    int countMon = 0;
    
    for (int i=0; i<monQ.size(); i++){
      TDatime date(2020,month.at(i),day.at(i),hour.at(i),minute.at(i),second.at(i));

      if (monQ.at(i) > 500 && monQ.at(i) < 4000 && date.Convert() > time){
	if (i == 0){

	  if (totQ.at(i) < 2000){
	    count++;
	    
	    monDataPoints.push_back(monQ.at(i));
	    averageMon += monQ.at(i);
	    countMon++;
	    
	    normDataPoints.push_back(totQ.at(i)/monQ.at(i));
	    averageNorm += totQ.at(i)/monQ.at(i);
	  }
	}
	else if (i % avenum == 0){

	  if (totQ.at(i) < 2000){
	    count++;
	    // Average Calculation
	    
	    monDataPoints.push_back(monQ.at(i));
	    averageMon += monQ.at(i);
	    countMon++;
	    
	    normDataPoints.push_back(totQ.at(i)/monQ.at(i));
	    averageNorm += totQ.at(i)/monQ.at(i);
	  }
	  
	  averageMon /= countMon;
	  averageNorm /= countMon;
	  
	  float maxmon = *std::max_element(monDataPoints.begin(),monDataPoints.end());
	  float minmon = *std::min_element(monDataPoints.begin(),monDataPoints.end());
	  errorMon = (maxmon - minmon) / (2 * std::sqrt(countMon));
	  errorMonVec.push_back(errorMon);
	  monDataPoints.clear();

	  float maxnorm = *std::max_element(normDataPoints.begin(),normDataPoints.end());
	  float minnorm = *std::min_element(normDataPoints.begin(),normDataPoints.end());
	  errorNorm = (maxnorm - minnorm) / (2 * std::sqrt(countMon));
	  errorNormVec.push_back(errorNorm);
	  normDataPoints.clear();
	  
	  // Sort out average vectors
	  dateVec.push_back(date.Convert());
	  count = 0;
	  monQVec.push_back(averageMon);
	  normQVec.push_back(averageNorm);
	  averageMon = 0;
	  averageNorm = 0;
	  countMon = 0;
	  
	}
	else {
	  if (totQ.at(i) < 2000){
	    count++;
	    
	    monDataPoints.push_back(monQ.at(i));
	    averageMon += monQ.at(i);
	    countMon++;
	    
	    normDataPoints.push_back(totQ.at(i)/monQ.at(i));
	    averageNorm += totQ.at(i)/monQ.at(i);
	  }
	}
      
      }
    
    }

    dateVecVec.push_back(dateVec);

    monQVecVec.push_back(monQVec);
    errorMonVecVec.push_back(errorMonVec);

    normQVecVec.push_back(normQVec);
    errorNormVecVec.push_back(errorNormVec);

    
  }
  


  if (oNameSwitch == 0) outname = beamType;
  std::string tempNameMon = "Mon Q (" + beamType + ") Since Gd";
  std::string tempNameNorm = "Norm Q (" + beamType + ") Since Gd";
  std::string ending = "_preGd";
  
  if (weekSwitch){
    tempNameMon = "Mon Q (" + beamType + ") " + std::to_string(weeks) + " Weeks";
    tempNameNorm = "Norm Q (" + beamType + ") " + std::to_string(weeks) + " Weeks";
    ending = "_" + std::to_string(weeks) + "weeks";
  }

  TCanvas c7("c7","");
  c7.SetGrid();
  TLegend *leg7 = new TLegend(0.13,0.85,0.9,0.9);
  TGraphErrors *monQ1 = new TGraphErrors(dateVecVec[0].size(), &dateVecVec[0][0], &monQVecVec[0][0], 0, &errorMonVecVec[0][0]);
  TGraphErrors *monQ2 = new TGraphErrors(dateVecVec[1].size(), &dateVecVec[1][0], &monQVecVec[1][0], 0, &errorMonVecVec[1][0]);
  TGraphErrors *monQ3 = new TGraphErrors(dateVecVec[2].size(), &dateVecVec[2][0], &monQVecVec[2][0], 0, &errorMonVecVec[2][0]);
  TGraphErrors *monQ4 = new TGraphErrors(dateVecVec[3].size(), &dateVecVec[3][0], &monQVecVec[3][0], 0, &errorMonVecVec[3][0]);
  TGraphErrors *monQ5 = new TGraphErrors(dateVecVec[4].size(), &dateVecVec[4][0], &monQVecVec[4][0], 0, &errorMonVecVec[4][0]);
  format_plots(monQ1, monQ2, monQ3, monQ4, monQ5,leg7, 2870, 2970, "%m-%d","Mon Q",tempNameMon);
  std::string printname7 = "plots/" + outname + ending + "_monQ.png";
  c7.Print(printname7.c_str());
  
  TCanvas c1("c1","");
  c1.SetGrid();
  TLegend *leg1 = new TLegend(0.13,0.85,0.9,0.9);
  TGraphErrors *normQ1 = new TGraphErrors(dateVecVec[0].size(), &dateVecVec[0][0], &normQVecVec[0][0], 0, &errorNormVecVec[0][0]);
  TGraphErrors *normQ2 = new TGraphErrors(dateVecVec[1].size(), &dateVecVec[1][0], &normQVecVec[1][0], 0, &errorNormVecVec[1][0]);
  TGraphErrors *normQ3 = new TGraphErrors(dateVecVec[2].size(), &dateVecVec[2][0], &normQVecVec[2][0], 0, &errorNormVecVec[2][0]);
  TGraphErrors *normQ4 = new TGraphErrors(dateVecVec[3].size(), &dateVecVec[3][0], &normQVecVec[3][0], 0, &errorNormVecVec[3][0]);
  TGraphErrors *normQ5 = new TGraphErrors(dateVecVec[4].size(), &dateVecVec[4][0], &normQVecVec[4][0], 0, &errorNormVecVec[4][0]);
  format_plots(normQ1, normQ2, normQ3, normQ4, normQ5,leg1, 0, 0.3, "%m-%d","Norm Q",tempNameNorm);
  std::string printname1 = "plots/" + outname + ending + "_normQ.png";
  c1.Print(printname1.c_str());
  

}
