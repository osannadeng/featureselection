#pragma once

#include <vector>

double leave_one_out_cross_validation(std::vector<std::vector<double> > data, const std::vector<int>& curr_set, int feature);

double base_accuracy(const std::vector<std::vector<double> >& data);

void forward_selection(const std::vector<std::vector<double> >& data);

void backward_elimination(const std::vector<std::vector<double> >& data);