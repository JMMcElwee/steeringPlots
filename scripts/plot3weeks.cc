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
#include "createPlots.h"

void plotUK(BeamType beam, int weeks, bool weekSwitch){

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

  std::vector<std::vector<float>> dateVecVec, totQVecVec, spotQVecVec, date6VecVec,totQ6VecVec,spotQ6VecVec, date24VecVec,totQ24VecVec,spotQ24VecVec,errorTotVecVec,errorTot6VecVec,errorTot24VecVec,errorSpotVecVec,errorSpot6VecVec,errorSpot24VecVec;

  
  for (int inj=0; inj<5;inj++){
    std::vector<int> run, subrun, year, month, day, hour, minute, second;
    std::vector<float> totQ, spotQ, monQ;
    std::ifstream file(fileList[inj]);
    std::cout << fileList[inj] << std::endl;
    extract_data(file,year,month,day,hour,minute,second,totQ,spotQ,monQ);

    
    int avenum = 4500;
    if (weekSwitch){
      avenum = weeks*400;
    }
    std::vector<float> totDataPoints, totQVec, spotDataPoints, spotQVec, dateVec, errorSpotVec, errorTotVec;
    float averageTot = 0;
    float averageSpot = 0;
    int count = 0;

    
    int time = 1594080000;
    if (weekSwitch){
      time = timer - weeks*604800;
    }
    float errorTot,errorSpot;

    int normCount = 0;
    float normAverageTot = 0;
    float normAverageSpot = 0;

    int yearConv;
    TDatime date;
    for (int i=0; i<totQ.size(); i++){
      
      yearConv = year.at(i) + 1900;
      date.Set(yearConv,month.at(i),day.at(i),hour.at(i),minute.at(i),second.at(i));
      
      if (date.Convert() > 1594080000 && date.Convert() < 1594864800){
	if (totQ.at(i) < 2000){
	  normAverageTot += totQ.at(i);
	  normAverageSpot += spotQ.at(i);
	  normCount++;
	}
      }
      else if (date.Convert() > 1594864800){
	normAverageTot /= normCount;
	normAverageSpot /= normCount;
	normCount=0;
	break;
      }
      
    }


    
    for (int i=0; i<totQ.size(); i++){
      yearConv = year.at(i) + 1900;
      date.Set(yearConv,month.at(i),day.at(i),hour.at(i),minute.at(i),second.at(i));
      
      if (date.Convert() > time){
	if (i == 0){
	  count++;
	  
	  averageSpot += (spotQ.at(i));
	  if (totQ.at(i) < 2000){
	    totDataPoints.push_back(totQ.at(i));
	    averageTot += (totQ.at(i));
	  }
	  if (spotQ.at(i) < 2000){
	    spotDataPoints.push_back(spotQ.at(i));
	  }
	}
	else if (i % avenum == 0){
	  count++;
	// Average Calculation
	  averageSpot += spotQ.at(i);
	  averageSpot /= count;
	  
	  if (totQ.at(i) < 2000){
	    averageTot += totQ.at(i);
	    averageTot /= count;
	    totDataPoints.push_back(totQ.at(i));
	  }
	  if (spotQ.at(i) < 2000){
	    spotDataPoints.push_back(spotQ.at(i));
	  }
	  
	  float max = *std::max_element(totDataPoints.begin(),totDataPoints.end());
	  float min = *std::min_element(totDataPoints.begin(),totDataPoints.end());
	  errorTot = (max/normAverageTot - min/normAverageTot) / (2 * std::sqrt(count));
	  //std::cout << max << " " << min << std::endl;
	  float maxsp = *std::max_element(spotDataPoints.begin(),spotDataPoints.end());
	  float minsp = *std::min_element(spotDataPoints.begin(),spotDataPoints.end());
	  errorSpot = (maxsp/normAverageSpot - minsp/normAverageSpot) / (2 * std::sqrt(count));
	  errorTotVec.push_back(errorTot);
	  errorSpotVec.push_back(errorSpot);
	  totDataPoints.clear();
	  spotDataPoints.clear();
	  
	  // Sort out average vectors
	  totQVec.push_back(averageTot/normAverageTot);
	  spotQVec.push_back(averageSpot/normAverageSpot);
	  dateVec.push_back(date.Convert());
	  averageTot = 0;
	  averageSpot = 0;
	  count = 0;
	}
	else {
	  averageSpot += spotQ.at(i);
	  count++;
	  if (totQ.at(i) < 2000){
	    averageTot += totQ.at(i);
	    totDataPoints.push_back(totQ.at(i));
	  }
	  if (spotQ.at(i) < 2000){
	    spotDataPoints.push_back(spotQ.at(i));
	  }
	}
      }  
    }
    
    
    errorTotVecVec.push_back(errorTotVec);
    errorSpotVecVec.push_back(errorSpotVec);
    dateVecVec.push_back(dateVec);
    totQVecVec.push_back(totQVec);
    spotQVecVec.push_back(spotQVec);
  }
  


  if (oNameSwitch == 0) outname = beamType;
  std::string tempNameTot = "Total Q (" + beamType + ") Since Gd";
  std::string tempNameSpot = "Spot Q (" + beamType + ") Since Gd";
  std::string ending = "_preGd";
  
  if (weekSwitch){
    tempNameTot = "Total Q (" + beamType + ") " + std::to_string(weeks) + " Weeks";
    tempNameSpot = "Spot Q (" + beamType + ") " + std::to_string(weeks) + " Weeks";
    ending = "_" + std::to_string(weeks) + "weeks";
  }

  
  TCanvas c1("c1","");
  c1.SetGrid();
  TLegend *leg1 = new TLegend(0.13,0.85,0.9,0.9);
  TGraphErrors *totQ1 = new TGraphErrors(dateVecVec[0].size(), &dateVecVec[0][0], &totQVecVec[0][0], 0, &errorTotVecVec[0][0]);
  TGraphErrors *totQ2 = new TGraphErrors(dateVecVec[1].size(), &dateVecVec[1][0], &totQVecVec[1][0], 0, &errorTotVecVec[1][0]);
  TGraphErrors *totQ3 = new TGraphErrors(dateVecVec[2].size(), &dateVecVec[2][0], &totQVecVec[2][0], 0, &errorTotVecVec[2][0]);
  TGraphErrors *totQ4 = new TGraphErrors(dateVecVec[3].size(), &dateVecVec[3][0], &totQVecVec[3][0], 0, &errorTotVecVec[3][0]);
  TGraphErrors *totQ5 = new TGraphErrors(dateVecVec[4].size(), &dateVecVec[4][0], &totQVecVec[4][0], 0, &errorTotVecVec[4][0]);
  format_plots(totQ1, totQ2, totQ3, totQ4, totQ5,leg1, 0.25, 1.4, "%m-%d","Tot Q (p.e)",tempNameTot);
  std::string printname1 = "plots/" + outname + ending + "_totQ_norm.png";
  c1.Print(printname1.c_str());
  
  
  TCanvas c2("c2","");
  c2.SetGrid();
  TLegend *leg2 = new TLegend(0.13,0.85,0.9,0.9);
  TGraphErrors *spotQ1 = new TGraphErrors(dateVecVec[0].size(), &dateVecVec[0][0], &spotQVecVec[0][0], 0, &errorSpotVecVec[0][0]);
  TGraphErrors *spotQ2 = new TGraphErrors(dateVecVec[1].size(), &dateVecVec[1][0], &spotQVecVec[1][0], 0, &errorSpotVecVec[1][0]);
 TGraphErrors *spotQ3 = new TGraphErrors(dateVecVec[2].size(), &dateVecVec[2][0], &spotQVecVec[2][0], 0, &errorSpotVecVec[2][0]);
  TGraphErrors *spotQ4 = new TGraphErrors(dateVecVec[3].size(), &dateVecVec[3][0], &spotQVecVec[3][0], 0, &errorSpotVecVec[3][0]);
  TGraphErrors *spotQ5 = new TGraphErrors(dateVecVec[4].size(), &dateVecVec[4][0], &spotQVecVec[4][0], 0, &errorSpotVecVec[4][0]);
  format_plots(spotQ1, spotQ2, spotQ3, spotQ4, spotQ5,leg2, 0.25, 1.4,"%m-%d","Spot Q (p.e)",tempNameSpot);
  std::string printname2 = "plots/" + outname + ending + "_spotQ_norm.png";
  c2.Print(printname2.c_str());

  
}
