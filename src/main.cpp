// #include <iostream>
// #include <vector>
// #include <algorithm>
// #include <cmath>
// #include <queue>
// #include <limits>
// #include "metrics.h"
#include "dbscan.h"
#include "data.h"

using namespace std;

int main()
{

    Data dataset;
    dataset = load_data("toy");

    vector<int> clustering = dbscan(dataset.points, 3, 2);

    // for (int i = 0; i < dataset.labels.size(); i++)
    //     cout << dataset.labels[i] << " ";
    // cout << endl
    //      << "-----------------------------------" << endl;

    // vector<int> true_clusters = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1};
    // vector<int> predicted_clusters = {1, 3, 2, 2, 2, 2, 1, 1, 3, 3};

    // std::cout << "          Pruity score: " << pruity_score(true_clusters, predicted_clusters) << endl;
    // std::cout << "            Rand score: " << rand_score(true_clusters, predicted_clusters) << endl;
    // std::cout << "  Davies-Bouldin score: " << davies_bouldin_score(dataset.points, clustering) << endl;
    // std::cout << "Silhouette Coefficient: " << silhouette_coefficient(dataset.points, clustering) << endl; //TODO prawdopdobnie do porawki, walic to

    return 0;
}