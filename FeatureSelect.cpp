#include "FeatureSelect.h"

double leave_one_out_cross_validation(std::vector<std::vector<double> > data, std::vector<int>& curr_set, int feature) {
    // reusable index variables
    int i, j, k;

    // set features not considered to 0
    std::vector<bool> considering(data[0].size() - 1, false);
    for (int c : curr_set) {
        considering[c - 1] = true;
    }
    for (i = 0; i < considering.size(); ++i) {
        if (considering[i]) continue;
        for (j = 1; j < data.size(); ++j) {
            data[j][i + 1] = 0;
        }
    }

    int label, nearest_loc, nearest_label;
    double nearest_dist, distance;
    int correct = 0;
    for (i = 1; i <= data.size(); ++i) {
        std::vector<double> curr(data[i - 1].begin() + 1, data[i - 1].end());
        label = (int)data[i - 1][0];

        // std::cout << "Looping over i, at the " << i << " location" << std::endl;
        // std::cout << "The " << i;
        // if (i == 1) std::cout << "st";
        // else if (i == 2) std::cout << "nd";
        // else if (i == 3) std::cout << "rd";
        // else std::cout << "th";
        // std::cout << " object is in class " << label << std::endl;

        nearest_dist = std::numeric_limits<double>::max();
        nearest_loc = std::numeric_limits<int>::max();
        for (k = 1; k <= data.size(); ++k) {
            // skip self
            if (k == i) continue;
            // std::cout << "Ask if " << i << " is the nearest neighbor of " << k << std::endl;

            // euclidean distance
            distance = 0.0;
            for (j = 0; j < curr.size(); ++j) {
                distance += (curr[j] - data[k - 1][j + 1]) * (curr[j] - data[k - 1][j + 1]);
            }
            distance = sqrt(distance);

            // check to update
            if (distance < nearest_dist) {
                nearest_dist = distance;
                nearest_loc = k;
                nearest_label = (int)data[k - 1][0];
            }
        }
        // std::cout << "Object " << i << " is class " << label << std::endl;
        // std::cout << "Its nearest neighbor is object " << nearest_loc << " which is in class " << nearest_label << std::endl;

        if (label == nearest_label) ++correct;
    }

    // accuracy = num correct/ num instances in database
    return (double)correct / data.size();
}

void feature_search(std::vector<std::vector<double> >& data) {
    // store tree
    std::vector<int> feature_set;

    int i, j;
    std::vector<int> to_add;
    double curr_best, accuracy;

    // for each level
    for (i = 1; i < data[0].size(); ++i) {
        std::cout << "On the " << i;
        if (i == 1) std::cout << "st";
        else if (i == 2) std::cout << "nd";
        else if (i == 3) std::cout << "rd";
        else std::cout << "th";
        std::cout << " level of the search tree" << std::endl;

        to_add.clear();
        curr_best = 0.0;
        // for each feature
        for (j = 1; j < data[0].size(); ++j) {
            // skip if already chosen
            if (std::find(feature_set.begin(), feature_set.begin() + i - 1, j) != feature_set.end()) continue;

            // std::cout << "Considering adding the " << j;
            // if (j == 1) std::cout << "st";
            // else if (j == 2) std::cout << "nd";
            // else if (j == 3) std::cout << "rd";
            // else std::cout << "th";
            // std::cout << " feature" << std::endl;

            accuracy = leave_one_out_cross_validation(data, feature_set, j);
            if (accuracy > curr_best) {
                curr_best = accuracy;
                to_add.push_back(j);
            }
        }
        
        feature_set.push_back(to_add[to_add.size() - 1]);
        std::cout << "On level " << i << ", I added feature " << to_add[to_add.size() - 1] << " to current set" << std::endl;
    }
}