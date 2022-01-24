#include <chrono>

#include "metrics.h"
#include "dbscan.h"
#include "clara.h"
#include "data.h"

using namespace std;
using namespace std::chrono;

int main()
{
    auto start = high_resolution_clock::now();

    // Data dataset = load_data("toy");
    Data dataset = load_data("complex9");

    // vector<int> clustering_dbscan = dbscan(dataset.points, 3, 2);
    vector<int> clustering_clara = clara(dataset.points, 16, 2, 100);

    // for (int i = 0; i < clustering_dbscan.size(); i++)
    //     cout << clustering_dbscan[i] << " ";
    // cout << endl
    //      << "-----------------------------------" << endl;

    for (int i = 0; i < clustering_clara.size(); i++)
        cout << clustering_clara[i] << " ";
    cout << endl
         << "-----------------------------------" << endl;

    // vector<int> true_clusters = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1};
    // vector<int> predicted_clusters = {1, 3, 2, 2, 2, 2, 1, 1, 3, 3};

    std::cout << "          Pruity score: " << pruity_score(dataset.labels, clustering_clara) << endl;
    // std::cout << "            Rand score: " << rand_score(dataset.labels, clustering_clara) << endl;
    // std::cout << "  Davies-Bouldin score: " << davies_bouldin_score(dataset.points, clustering_clara) << endl;
    // std::cout << "Silhouette Coefficient: " << silhouette_coefficient(dataset.points, clustering) << endl; //TODO prawdopdobnie do porawki, walic to

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Processing time: "
         << duration.count() / 1000000 << " seconds" << endl;
    return 0;
}