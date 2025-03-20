#include <iostream>
#include <fstream>
#include <sstream>
// #include <chrono>

#include "FeatureSelect.h"

void parse(std::vector<std::vector<double> >& data, std::string filename);

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
    parse(data, "datasets/" + filename);

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