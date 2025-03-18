#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "FeatureSelect.h"

void parse(std::vector<std::vector<double> >& data, std::string filename);

int main() {
    // initial prompting
    int testopt, algoopt;
    std::cout << "Welcome to the Feature Selection!" << std::endl;
    std::cout << "Type the number of the file to test:" << std::endl;
    std::cout << "\t(1) Small Dataset 2" << std::endl;
    std::cout << "\t(2) Small Dataset 43" << std::endl;
    std::cout << "\t(3) Large Dataset 2" << std::endl;
    std::cout << "\t(4) Large Dataset 12" << std::endl;
    std::cout << "\t(5) Large Dataset 17" << std::endl;

    std::cin >> testopt;
    std::cout << "Type the number of the algorithm to run:" << std::endl;
    std::cout << "\t(1) Forward Selection" << std::endl;
    std::cout << "\t(2) Backward Elimination" << std::endl;
    std::cin >> algoopt;

    std::cout << std::endl;

    std::vector<std::vector<double> > data;

    // parse dataset option
    if (testopt == 1) parse(data, "datasets/CS170_Small_Data__2.txt");
    else if (testopt == 2) parse(data, "datasets/CS170_Small_Data__43.txt");
    else if (testopt == 3) parse(data, "datasets/CS170_Large_Data__2.txt");
    else if (testopt == 4) parse(data, "datasets/CS170_Large_Data__12.txt"); 
    else if (testopt == 5) parse(data, "datasets/CS170_Large_Data__17.txt"); 
    else {
        std::cout << "Invalid option." << std::endl;
        return 1;
    }

    // run algorithm
    if (algoopt == 1) {
        std::cout << "Running Forward Selection..." << std::endl;
        forward_selection(data);
    } else if (algoopt == 2) {
        std::cout << "Running Backward Elimination..." << std::endl;
        backward_elimination(data);
    } else {
        std::cout << "Invalid option." << std::endl;
        return 1;
    }

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