#include <iostream>

#include "FeatureSelect.h"

double leave_one_out_cross_validation(std::vector<std::vector<double> > data, const std::vector<int>& curr_set, int feature) {
    // reusable index variables
    int i, j, k;

    // set features not considered to 0
    std::vector<bool> considering(data[0].size() - 1, false);
    for (int c : curr_set) {
        considering[c - 1] = true;
    }
    if (feature > 0) considering[feature - 1] = true;
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

double base_accuracy(const std::vector<std::vector<double> >& data) {
    int count = 0;
    for (unsigned i = 0; i < data.size(); ++i) {
        if ((int)data[i][0] == 1) ++count;
    }
    if (count > data.size() - count) return (double)count / data.size();
    else return (data.size() - (double)count) / data.size();
}

void forward_selection(const std::vector<std::vector<double> >& data) {
    // store result
    std::vector<int> best_set;

    // recycled index variables
    int i, j, k;

    std::vector<int> feature_set;
    std::vector<int> to_add;
    double accuracy, curr_best;

    std::cout << "This dataset has " << data[0].size() - 1 << " features (not including the class attribute), with " << data.size() << " instances." << std::endl;

    // populate tree for evaluation of all features
    for (i = 1; i < data[0].size(); ++i) {
        feature_set.push_back(i);
    }
    
    std::cout << "Running nearest neighbor with all " << data[0].size() - 1 << " features, using \"leaving-one-out\" evaluation, I get an accuracy of " << leave_one_out_cross_validation(data, feature_set, 0) * 100 << "%" << std::endl;

    feature_set.clear();

    std::cout << "Beginning search." << std::endl;

    // empty subset
    double best = base_accuracy(data);
    std::cout << "\tUsing only feature(s) {}, accuracy is " << best * 100 << "%" << std::endl;
    std::cout << "Feature set {} was best, accuracy is " << best * 100 << "%" << std::endl;
    bool changed = false;

    // for each level
    for (i = 1; i < data[0].size(); ++i) {
        to_add.clear();
        curr_best = 0.0;

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

            if (accuracy > curr_best) {
                curr_best = accuracy;
                to_add.push_back(j);
            }
        }

        if (curr_best < best)  std::cout << "Warning: Accuracy has decreased! Continuing in case of local maxima" << std::endl;

        feature_set.push_back(to_add[to_add.size() - 1]);

        if (curr_best > best) {
            best = curr_best;
            best_set = feature_set;
            changed = true;
        }

        std::cout << "Feature set {";
        for (k = 0; k < feature_set.size(); ++k) {
            std::cout << feature_set[k];
            if (k < feature_set.size() - 1) std::cout << ", ";
        }
        std::cout << "} was best, accuracy is " << curr_best * 100 << "%" << std::endl;
    }

    // print results
    if (!changed) {
        std::cout << "Finished search!! No features improved accuracy. The base accuracy is: " << best * 100 << "%" << std::endl;
        return;
    }
    std::cout << "Finished search!! The best feature subset is {";
    for (k = 0; k < best_set.size(); ++k) {
        std::cout << best_set[k];
        if (k < best_set.size() - 1) std::cout << ", ";
    }
    std::cout << "}, which has an accuracy of " << best * 100 << "%" << std::endl;
}

// strategy: start with all features, remove least helpful until accuracy decreases
void backward_elimination(const std::vector<std::vector<double> >& data) {
    // store result
    std::vector<int> best_set;

    // recycled index variables
    int i, j, k;

    // populate tree
    std::vector<int> feature_set;
    for (i = 1; i < data[0].size(); ++i) {
        feature_set.push_back(i);
    }

    std::vector<int> to_remove;
    std::vector<int> exclude_j;
    double accuracy, curr_best;

    std::cout << "This dataset has " << data[0].size() - 1 << " features (not including the class attribute), with " << data.size() << " instances." << std::endl;

    std::cout << "Running nearest neighbor with all " << data[0].size() - 1 << " features, using \"leaving-one-out\" evaluation, I get an accuracy of " << leave_one_out_cross_validation(data, feature_set, 0) * 100 << "%" << std::endl;

    std::cout << "Beginning search." << std::endl;

    // full set
    double best = leave_one_out_cross_validation(data, feature_set, 0);
    std::cout << "\tUsing only feature(s) {";
    for (i = 1; i < data[0].size(); ++i) {
        std::cout << i;
        if (i < data[0].size() - 1) std::cout << ", ";
    }
    std::cout << "}, accuracy is " << best * 100 << "%" << std::endl;
    std::cout << "Feature set {";
    for (i = 1; i < data[0].size(); ++i) {
        std::cout << i;
        if (i < data[0].size() - 1) std::cout << ", ";
    }
    std::cout << "} was best, accuracy is " << best * 100 << "%" << std::endl;

    // for each level
    for (i = 1; i < data[0].size(); ++i) {
        if (feature_set.size() <= 1) break;

        to_remove.clear();
        curr_best = 0.0;
        
        // for each feature
        for (j = 0; j < feature_set.size(); ++j) {
            exclude_j = feature_set;
            exclude_j.erase(exclude_j.begin() + j);

            accuracy = leave_one_out_cross_validation(data, exclude_j, 0);

            std::cout << "\tUsing only feature(s) {";
            for (k = 0; k < exclude_j.size(); ++k) {
                if (k > 0) std::cout << ", ";
                std::cout << exclude_j[k];
            }
            std::cout << "}, accuracy is " << accuracy * 100 << "%" << std::endl;

            if (accuracy > curr_best) {
                curr_best = accuracy;
                to_remove.push_back(j);
            }
        }

        if (curr_best < best) std::cout << "Warning: Accuracy has decreased! Continuing in case of local maxima" << std::endl;
        
        feature_set.erase(feature_set.begin() + to_remove[to_remove.size() - 1]);

        if (curr_best > best) {
            best = curr_best;
            best_set = feature_set;
        }

        std::cout << "Feature set {";
        for (k = 0; k < feature_set.size(); ++k) {
            std::cout << feature_set[k];
            if (k < feature_set.size() - 1) std::cout << ", ";
        }
        std::cout << "} was best, accuracy is " << curr_best * 100 << "%" << std::endl;
    }

    // empty subset
    accuracy = base_accuracy(data);
    std::cout << "\tUsing only feature(s) {}, accuracy is " << accuracy * 100 << "%" << std::endl;
    std::cout << "Feature set {} was best, accuracy is " << accuracy * 100 << "%" << std::endl;
    if (accuracy > best) {
        best = accuracy;
        std::cout << "Finished search. No features improved accuracy. The base accuracy is: " << best * 100 << "%" << std::endl;
        return;
    }

    std::cout << "Finished search. The best feature subset is {";
    for (k = 0; k < best_set.size(); ++k) {
        std::cout << best_set[k];
        if (k < best_set.size() - 1) std::cout << ", ";
    }
    std::cout << "}, which has an accuracy of " << best * 100 << "%" << std::endl;
}