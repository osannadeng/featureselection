#include <iostream>
#include <vector>

#include "FeatureSelect.h"

int main() {
    // initial prompting
    int testopt, algoopt;
    std::cout << "Welcome to the Feature Selection!" << std::endl;
    std::cout << "Type the number of the file to test:" << std::endl;
    std::cout << "\t(1) Small Dataset 43" << std::endl;
    std::cout << "\t(2) Large Dataset 2" << std::endl;
    std::cin >> testopt;
    std::cout << "Type the number of the algorithm to run:" << std::endl;
    std::cout << "\t(1) Forward Selection" << std::endl;
    std::cout << "\t(2) Backward Elimination" << std::endl;
    std::cin >> algoopt;

    std::vector<double> test_data(5, 0.0);
    std::vector<std::vector<double> > data(10, test_data);
    feature_search(data);

    return 0;
}