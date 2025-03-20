#include <iostream>
#include <fstream>
#include <sstream>
// #include <chrono>
#include <map>

#include "FeatureSelect.h"

void parse(std::vector<std::vector<double> >& data, std::string filename);

void parse_yeast_file(std::vector<std::vector<double> >& data);

int main() {
    // initial prompting
    std::string filename;
    std::cout << "Welcome to the Feature Selection Algorithm." << std::endl;
    std::cout << "Type in the name of the file to test: ";
    std::cin >> filename;
    std::cout << std::endl;

    int algoopt;
    std::cout << "Type the number of the algorithm to run:" << std::endl;
    std::cout << "\t(1) Forward Selection" << std::endl;
    std::cout << "\t(2) Backward Elimination" << std::endl;
    std::cin >> algoopt;

    std::cout << std::endl;

    std::vector<std::vector<double> > data;

    // parse dataset option
    if (filename == "yeast.data") parse_yeast_file(data);
    else parse(data, "datasets/" + filename);

    // run algorithm
    // auto start_time = std::chrono::high_resolution_clock::now();
    if (algoopt == 1) forward_selection(data);
    else if (algoopt == 2) backward_elimination(data);
    else {
        std::cout << "Invalid option." << std::endl;
        return 1;
    }
    // auto end_time = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> elapsed = end_time - start_time;
    // std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}

void parse_yeast_file(std::vector<std::vector<double> >& data) {
    std::ifstream f("yeast/yeast.data");

    if (!f.is_open()) {
        std::cout << "Cannot open yeast/yeast.data" << std::endl;
        return;
    }

    // to compute z-scores later
    std::vector<double> means(8, 0.0);

    std::string line;
    // convert categorization to integers
    std::map<std::string, int> labels = {{"CYT", 0}, {"NUC", 1}, {"MIT", 2}, {"ME3", 3}, {"ME2", 4}, {"ME1", 5}, {"EXC", 6}, {"VAC", 7}, {"POX", 8}, {"ERL", 9}};
    std::vector<double> obj;

    unsigned i, j;
    while (std::getline(f, line)) {
        std::stringstream iss(line);
        std::string ignore;

        iss >> ignore;

        // feature data
        obj.clear();
        double value;
        for (i = 0; i < 8; ++i) {
            iss >> value;
            obj.push_back(value);
            means[i] += value;
        }
        
        // put labels at the beginning of each instance
        std::string label;
        iss >> label;
        obj.insert(obj.begin(), labels[label]);
        
        data.push_back(obj);
    }

    // z-score normalization
    for (i = 0; i < 8; ++i) {
        means[i] /= data.size();
    }
    std::vector<double> stddevs(8, 0.0);
    for (i = 0; i < data.size(); ++i) {
        for (j = 1; j < 9; ++j) {
            stddevs[j - 1] += (data[i][j] - means[j - 1]) * (data[i][j] - means[j - 1]);
        }
    }
    for (i = 0; i < 8; ++i) {
        stddevs[i] = sqrt(stddevs[i] / data.size());
        if (stddevs[i] == 0) throw std::runtime_error("Division by zero in standard deviation calculation");
    }
    for (i = 0; i < data.size(); ++i) {
        for (j = 1; j < 9; ++j) {
            data[i][j] = (data[i][j] - means[j - 1]) / stddevs[j - 1];
        }
    }
}

void parse(std::vector<std::vector<double> >& data, std::string filename) {
    std::ifstream f(filename);

    if (!f.is_open()) {
        std::cout << "Cannot open " << filename << std::endl;
        return;
    }

    std::string line;
    std::vector<double> obj;
    while (std::getline(f, line)) {
        std::stringstream iss(line);
        double value;

        obj.clear();
        while (iss >> value) {
            obj.push_back(value);
        }

        data.push_back(obj);
    }
}