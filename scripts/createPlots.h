#ifndef CREATEPLOTS_H
#define CREATEPLOTS_H

#include <string>
#include <vector>
#include <fstream>
#include "TGraph.h"
#include "TLegend.h"

void extract_data(std::ifstream &file,std::vector<int> &month,std::vector<int> &day,std::vector<int> &hour,std::vector<int> &minute,std::vector<int> &second,std::vector<float> &totQ, std::vector<float> &spotQ,std::vector<float> &monQ);
void format_plots(TGraph *graph1,TGraph *graph2,TGraph *graph3,TGraph *graph4,TGraph *graph5,TLegend *leg ,float min, float max, std::string timeform, std::string yaxis, std::string title);

void extract_data_top(std::ifstream &file,std::vector<double> &totQ, std::vector<double> &barrelQ,std::vector<int> &month,std::vector<int> &day,std::vector<int> &hour,std::vector<int> &minute,std::vector<int> &second,std::vector<double> &bottomQ,std::vector<double> &bz0y0Q);
void format_plots_top(TGraph *graph1,TLegend *leg,float min, float max, std::string timeform, std::string yaxis, std::string title);

#endif
