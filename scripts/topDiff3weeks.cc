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

#include "createPlots.h"


void topDiff(int weeks, bool weekSwitch){

  
  int oNameSwitch = 0;
  std::string ldir = "/disk02/usr6/jmcelwee/monitoringData/plotting";
  std::string endname = "";
  std::string outname = "";

  std::string fileEx = ldir + "/top_diffuser.dat";
  std::cout << fileEx << std::endl;

  
  time_t timer;
  time(&timer);

  gStyle->SetCanvasColor(0);
  gStyle->SetTitleColor(0);
  gStyle->SetOptStat(0);  

  //  std::vector<std::vector<float>> dateVecVec, totQVecVec, spotQVecVec, date6VecVec,totQ6VecVec,spotQ6VecVec, date24VecVec,totQ24VecVec,spotQ24VecVec,errorTotVecVec,errorTot6VecVec,errorTot24VecVec,errorSpotVecVec,errorSpot6VecVec,errorSpot24VecVec;


  std::vector<int> run, subrun, year, month, day, hour, minute, second;
  std::vector<double> totQ, barrelQ, bottomQ, bz0y0Q;
  std::ifstream file(fileEx);
  extract_data_top(file,totQ,barrelQ,year,month,day,hour,minute,second,bottomQ,bz0y0Q);


  
  int avenum = 4500;
  if (weekSwitch){
    avenum = weeks*400;
  }

  
  std::vector<float> totDataPoints, totQVec, ratioDataPoints, ratioQVec, dateVec, errorRatioVec, errorTotVec, bbDataPoints, bbQVec, errorBBVec;
  float averageTot = 0;
  float averageRatio = 0;
  float averageBB = 0;
  int count = 0;

  int time = 1594080000;
  if (weekSwitch){
    time = timer - weeks*604800;
  }


  std::vector<float> barrelDataPoints, barrelVec, errorBarVec;
  std::vector<float> bottomDataPoints, bottomVec, errorBotVec;
  std::vector<float> bzyDataPoints, bzyVec, errorBzyVec;
  float averageBot = 0;
  float averageBar = 0;
  float averageBzy = 0;
  float errorTot, errorRatio, errorBB;
  float errorBar, errorBot, errorBzy;

  int normCount = 0;
  float normBarrel = 0;
  float normBottom = 0;
  float normBzy = 0;
  float normRatio = 0;
  float normBB = 0;

  int yearConv;
  TDatime date;
  for (int i=0; i<totQ.size(); i++){
    yearConv = year.at(i) + 1900;
    date.Set(yearConv,month.at(i),day.at(i),hour.at(i),minute.at(i),second.at(i));
    

    if (date.Convert() > 1594080000 && date.Convert() < 1594864800){
      normBarrel += barrelQ.at(i)/totQ.at(i);
      normBottom += bottomQ.at(i)/totQ.at(i);
      normBzy += bz0y0Q.at(i)/totQ.at(i);
      normRatio += bottomQ.at(i)/barrelQ.at(i);
      normBB += bottomQ.at(i)/bz0y0Q.at(i);
      normCount++;
    }
    else if (date.Convert() > 1594864800){
      normBarrel /= normCount;
      normBottom /= normCount;
      normBzy /= normCount;
      normRatio /= normCount;
      normBB /= normCount;
      normCount=0;
      break;
    }
  }
  
  std::cout << normBarrel << " " << normBottom << " " << normBzy << " " << normRatio << " " << normBB << std::endl;

  for (int i=0; i<totQ.size(); i++){
    yearConv = year.at(i) + 1900;
    date.Set(yearConv,month.at(i),day.at(i),hour.at(i),minute.at(i),second.at(i));

    //    std::cout << date.GetYear() << " " << date.GetMonth() << " " << date.GetDay() << " " << date.GetHour() << " " << date.GetMinute() << std::endl;
    
    if (date.Convert() > time && totQ.at(i) > 90000 && totQ.at(i) < 190000){
      if (i == 0){
	count++;
	averageTot += totQ.at(i);
	averageRatio += bottomQ.at(i)/barrelQ.at(i);
	averageBB += bottomQ.at(i)/bz0y0Q.at(i);

	averageBot += bottomQ.at(i)/totQ.at(i);
	averageBar += barrelQ.at(i)/totQ.at(i);
	averageBzy += bz0y0Q.at(i)/totQ.at(i);
	bottomDataPoints.push_back(bottomQ.at(i)/totQ.at(i));
	barrelDataPoints.push_back(barrelQ.at(i)/totQ.at(i));
	bzyDataPoints.push_back(bz0y0Q.at(i)/totQ.at(i));

      
	if (bottomQ.at(i)/bz0y0Q.at(i) > 1.0 && bottomQ.at(i)/bz0y0Q.at(i) < 1.7){
	  bbDataPoints.push_back(bottomQ.at(i)/bz0y0Q.at(i));
	}
	totDataPoints.push_back(totQ.at(i));
	ratioDataPoints.push_back(bottomQ.at(i)/barrelQ.at(i));
      }
      else if (i % avenum == 0){
	count++;
	// Average Calculation
	averageTot += totQ.at(i);
	averageTot /= count;
	averageRatio += bottomQ.at(i)/barrelQ.at(i);
	averageRatio /= count;
	averageBB += bottomQ.at(i)/bz0y0Q.at(i);
	averageBB /= count;

	averageBot += bottomQ.at(i)/totQ.at(i);
	averageBar += barrelQ.at(i)/totQ.at(i);
	averageBzy += bz0y0Q.at(i)/totQ.at(i);
	averageBot /= count;
	averageBar /= count;
	averageBzy /= count;

	totDataPoints.push_back(totQ.at(i));
	ratioDataPoints.push_back(bottomQ.at(i)/barrelQ.at(i));
	if (bottomQ.at(i)/bz0y0Q.at(i) > 1.0 && bottomQ.at(i)/bz0y0Q.at(i) < 1.7){
	  bbDataPoints.push_back(bottomQ.at(i)/bz0y0Q.at(i));
	}
	bottomDataPoints.push_back(bottomQ.at(i)/totQ.at(i));
	barrelDataPoints.push_back(barrelQ.at(i)/totQ.at(i));
	bzyDataPoints.push_back(bz0y0Q.at(i)/totQ.at(i));
      

	float max = *std::max_element(totDataPoints.begin(),totDataPoints.end());
	float min = *std::min_element(totDataPoints.begin(),totDataPoints.end());
	errorTot = (max - min) / (2 * std::sqrt(count));
	//std::cout << max << " " << min << std::endl;
	float maxsp = *std::max_element(ratioDataPoints.begin(),ratioDataPoints.end());
	float minsp = *std::min_element(ratioDataPoints.begin(),ratioDataPoints.end());
	errorRatio = (maxsp - minsp) / (2 * std::sqrt(count) * normRatio);
	float maxbb = *std::max_element(bbDataPoints.begin(),bbDataPoints.end());
	float minbb = *std::min_element(bbDataPoints.begin(),bbDataPoints.end());
	errorBB = (maxbb - minbb) / (2 * std::sqrt(count) * normBB);
	//      if (errorBB > 0.05) std::cout << errorBB << std::endl;
	float maxba = *std::max_element(barrelDataPoints.begin(),barrelDataPoints.end());
	float minba = *std::min_element(barrelDataPoints.begin(),barrelDataPoints.end());
	errorBar = (maxba - minba) / (2 * std::sqrt(count) * normBarrel);
	float maxbo = *std::max_element(bottomDataPoints.begin(),bottomDataPoints.end());
	float minbo = *std::min_element(bottomDataPoints.begin(),bottomDataPoints.end());
	errorBot = (maxbo - minbo) / (2 * std::sqrt(count) * normBottom);
	float maxbzy = *std::max_element(bzyDataPoints.begin(),bzyDataPoints.end());
	float minbzy = *std::min_element(bzyDataPoints.begin(),bzyDataPoints.end());
	errorBzy = (maxbzy - minbzy) / (2 * std::sqrt(count) * normBzy);

      
	errorTotVec.push_back(errorTot);
	errorRatioVec.push_back(errorRatio);
	errorBBVec.push_back(errorBB);
	errorBotVec.push_back(errorBot);
	errorBarVec.push_back(errorBar);
	errorBzyVec.push_back(errorBzy);
	totDataPoints.clear();
	ratioDataPoints.clear();
	bbDataPoints.clear();
	bottomDataPoints.clear();
	barrelDataPoints.clear();
	bzyDataPoints.clear();
      

      
	// Sort out average vectors
	totQVec.push_back(averageTot);
	ratioQVec.push_back(averageRatio/normRatio);
	bbQVec.push_back(averageBB/normBB);
	dateVec.push_back(date.Convert());
	bottomVec.push_back(averageBot/normBottom);
	barrelVec.push_back(averageBar/normBarrel);
	bzyVec.push_back(averageBzy/normBzy);
	averageTot = 0;
	averageRatio = 0;
	averageBB = 0;
	averageBot = 0;
	averageBar = 0;
	averageBzy = 0;
	count = 0;
      }
      else {
	averageTot += totQ.at(i);
	averageRatio += bottomQ.at(i)/barrelQ.at(i);
	averageBB += bottomQ.at(i)/bz0y0Q.at(i);
	count++;

	averageBot += bottomQ.at(i)/totQ.at(i);
	averageBar += barrelQ.at(i)/totQ.at(i);
	averageBzy += bz0y0Q.at(i)/totQ.at(i);

	totDataPoints.push_back(totQ.at(i));
	ratioDataPoints.push_back(bottomQ.at(i)/barrelQ.at(i));
	if (bottomQ.at(i)/bz0y0Q.at(i) > 1.0 && bottomQ.at(i)/bz0y0Q.at(i) < 1.7){
	  bbDataPoints.push_back(bottomQ.at(i)/bz0y0Q.at(i));
	}
	bottomDataPoints.push_back(bottomQ.at(i)/totQ.at(i));
	barrelDataPoints.push_back(barrelQ.at(i)/totQ.at(i));
	bzyDataPoints.push_back(bz0y0Q.at(i)/totQ.at(i));
      }
      
    }
  }

  
   
  if (oNameSwitch == 0) outname = "top_diffuser";
  std::string tempNameTot = "Total Q (top diffuser)";
  std::string tempNameRatio = "Charge Ratio (top diffuser) ";
  std::string tempNameBB = "Charge Ratio (top diffuser - z&y > 0) ";
  std::string tempNameBot = "Normalised Bottom Q ";
  std::string tempNameBar = "Normalised Barrel Q ";
  std::string tempNameBzy = "Normalised Barrel Q (z&y > 0) ";
  std::string ending = "_preGd";

  
  if (weekSwitch){
    tempNameTot = "Total Q (top diffuser)";
    tempNameRatio = "Charge Ratio (top diffuser) ";
    tempNameBB = "Charge Ratio (top diffuser - z&y > 0) ";
    tempNameBot = "Normalised Bottom Q ";
    tempNameBar = "Normalised Barrel Q ";
    tempNameBzy = "Normalised Barrel Q (z&y > 0) ";
    ending = "_" + std::to_string(weeks) + "weeks";
  }
  

  
  TCanvas c1("c1","", 850, 500);
  c1.SetGrid();
  TLegend *leg1 = new TLegend(0.13,0.85,0.9,0.9);
  TGraphErrors *totQ1 = new TGraphErrors(dateVec.size(), &dateVec[0], &totQVec[0], 0, &errorTotVec[0]);
  format_plots_top(totQ1, leg1, 150000,190000, "%m-%d","Tot Q (p.e)",tempNameTot+"");
  std::string printname1 = "plots/" + outname + ending + "_totQ_norm.png";
  c1.Print(printname1.c_str());

  std::cout << totQVec.back() << std::endl;
  
  TCanvas c2("c2","", 850, 500);
  c2.SetGrid();
  TLegend *leg2 = new TLegend(0.75,0.8,0.9,0.9);
  TGraphErrors *ratioQ1 = new TGraphErrors(dateVec.size(), &dateVec[0], &ratioQVec[0], 0, &errorRatioVec[0]);
  format_plots_top(ratioQ1, leg2, 0.985, 1.015,"%m-%d","bottomQ/barrelQ",tempNameRatio+"");
  std::string printname2 = "plots/" +  outname + ending + "_ratioQ_norm.png";
  c2.Print(printname2.c_str());

  std::cout << ratioQVec.back() << std::endl;

  TCanvas c7("c7","", 850, 500);
  c7.SetGrid();
  TLegend *leg7 = new TLegend(0.75,0.8,0.9,0.9);
  TGraphErrors *bbQ1 = new TGraphErrors(dateVec.size(), &dateVec[0], &bbQVec[0], 0, &errorBBVec[0]);
  format_plots_top(bbQ1, leg7, 0.985, 1.015,"%m-%d","bottomQ/barrelQ",tempNameBB+"");
  std::string printname7 = "plots/" + outname + ending + "_bbQ_norm.png";
  c7.Print(printname7.c_str());

  std::cout << bbQVec.back() << std::endl;

  TCanvas c10("c10","", 850, 500);
  c10.SetGrid();
  TLegend *leg10 = new TLegend(0.75,0.8,0.9,0.9);
  TGraphErrors *botQ1 = new TGraphErrors(dateVec.size(), &dateVec[0], &bottomVec[0], 0, &errorBotVec[0]);
  format_plots_top(botQ1, leg10, 0.985, 1.015,"%m-%d","bottomQ/totQ",tempNameBot+"");
  std::string printname10 = "plots/" + outname + ending + "_botQ_norm.png";
  c10.Print(printname10.c_str());

  std::cout << bottomVec.back() << std::endl;
  
  TCanvas c11("c11","", 850, 500);
  c11.SetGrid();
  TLegend *leg11 = new TLegend(0.75,0.8,0.9,0.9);
  TGraphErrors *barQ1 = new TGraphErrors(dateVec.size(), &dateVec[0], &barrelVec[0], 0, &errorBarVec[0]);
  format_plots_top(barQ1, leg11, 0.985, 1.015,"%m-%d","barrelQ/totQ",tempNameBar+"");
  std::string printname11 = "plots/" + outname + ending + "_barQ_norm.png";
  c11.Print(printname11.c_str());

  std::cout << barrelVec.back() << std::endl;
  
  TCanvas c12("c12","", 850, 500);
  c12.SetGrid();
  TLegend *leg12 = new TLegend(0.75,0.8,0.9,0.9);
  TGraphErrors *bzyQ1 = new TGraphErrors(dateVec.size(), &dateVec[0], &bzyVec[0], 0, &errorBzyVec[0]);
  format_plots_top(bzyQ1, leg12, 0.985, 1.015,"%m-%d","bz0y0Q/totQ",tempNameBzy+"");
  std::string printname12 = "plots/" + outname + ending + "_bzyQ_norm.png";
  c12.Print(printname12.c_str());

  std::cout << bzyVec.back() << std::endl;
  
}


