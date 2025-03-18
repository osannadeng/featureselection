#pragma once

#include <iostream>
#include <vector>
#include <cmath>

double leave_one_out_cross_validation(std::vector<std::vector<double> > data, std::vector<int>& curr_set, int feature);

void feature_search(std::vector<std::vector<double> >& data);