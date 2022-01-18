#include "metrics.h"
#include "dbscan.h"
#include "clara.h"
#include "data.h"

using namespace std;

int main()
{

    Data dataset;
    dataset = load_data("toy");

    vector<int> clustering_dbscan = dbscan(dataset.points, 3, 2);
    vector<int> clustering_clara = clara(dataset.points, 2, 5, 3);

    for (int i = 0; i < clustering_dbscan.size(); i++)
        cout << clustering_dbscan[i] << " ";
    cout << endl
         << "-----------------------------------" << endl;

    for (int i = 0; i < clustering_clara.size(); i++)
        cout << clustering_clara[i] << " ";
    cout << endl
         << "-----------------------------------" << endl;

    vector<int> true_clusters = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1};
    vector<int> predicted_clusters = {1, 3, 2, 2, 2, 2, 1, 1, 3, 3};

    std::cout << "          Pruity score: " << pruity_score(true_clusters, predicted_clusters) << endl;
    std::cout << "            Rand score: " << rand_score(true_clusters, predicted_clusters) << endl;
    std::cout << "  Davies-Bouldin score: " << davies_bouldin_score(dataset.points, clustering_dbscan) << endl;
    // std::cout << "Silhouette Coefficient: " << silhouette_coefficient(dataset.points, clustering) << endl; //TODO prawdopdobnie do porawki, walic to

    return 0;
}