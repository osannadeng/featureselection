#include "FeatureSelect.h"

double leave_one_out_cross_validation(std::vector<std::vector<double> > data, const std::vector<int>& curr_set, int feature) {
    // reusable index variables
    int i, j, k;

    // set features not considered to 0
    std::vector<bool> considering(data[0].size() - 1, false);
    for (int c : curr_set) {
        considering[c - 1] = true;
    }
    considering[feature - 1] = true;
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

        nearest_dist = std::numeric_limits<double>::max();
        nearest_loc = std::numeric_limits<int>::max();
        for (k = 1; k <= data.size(); ++k) {
            // skip self
            if (k == i) continue;

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

        if (label == nearest_label) ++correct;
    }

    // accuracy = num correct/ num instances in database
    return (double)correct / data.size();
}

void forward_selection(const std::vector<std::vector<double> >& data) {
    // store tree
    std::vector<int> feature_set;

    int i, j, k;
    std::vector<int> to_add;
    double accuracy;
    double best = 0.0;
    bool improve = true;

    std::cout << "\nThis dataset has " << data[0].size() - 1 << " features, with " << data.size() << " instances." << std::endl << std::endl;
    std::cout << "Beginning search." << std::endl;

    // for each level
    for (i = 1; i < data[0].size(); ++i) {
        if (improve) std::cout << std::endl;

        to_add.clear();
        // for each feature
        for (j = 1; j < data[0].size(); ++j) {
            // skip if already chosen
            if (std::find(feature_set.begin(), feature_set.begin() + i - 1, j) != feature_set.end()) continue;

            accuracy = leave_one_out_cross_validation(data, feature_set, j);

            std::cout << "\tUsing feature(s) {" << j;
            for (k = 0; k < feature_set.size(); ++k) {
                std::cout << ", " << feature_set[k];
            }
            std::cout << "}, accuracy is " << accuracy * 100 << "%" << std::endl;

            if (accuracy > best) {
                best = accuracy;
                to_add.push_back(j);
            }
        }

        if (to_add.empty() && improve)  {
            std::cout << "\nWarning: Accuracy has decreased! Continuing in case of local maxima" << std::endl;
            improve = false;
            continue;
        } else if (to_add.empty() && !improve) continue;
        else if (!improve) improve = true;

        feature_set.push_back(to_add[to_add.size() - 1]);

        std::cout << "\nFeature set {";
        for (k = 0; k < feature_set.size(); ++k) {
            std::cout << feature_set[k];
            if (k < feature_set.size() - 1) std::cout << ", ";
        }
        std::cout << "} is better, with an accuracy of " << best * 100 << "%" << std::endl;
    }

    // print results
    std::cout << "\nFinished search. The best feature subset is {";
    for (k = 0; k < feature_set.size(); ++k) {
        std::cout << feature_set[k];
        if (k < feature_set.size() - 1) std::cout << ", ";
    }
    std::cout << "}, which has an accuracy of " << best * 100 << "%" << std::endl;
}