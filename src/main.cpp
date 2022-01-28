#include <chrono>
#include <string>
#include <fstream>

#include "metrics.h"
#include "dbscan.h"
#include "clara.h"
#include "data.h"
#include "globals.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char **argv)
{
    if ((argc != 7) & (argc != 5))
    {
        std::cout << "Wrong number of parameters!" << std::endl;
        // wypisz jak powinno wygladac polecenie dla obu
        return -1;
    }

    string model_name = argv[1];
    string data_name = argv[2];
    int clouster_count, random_group_count, random_group_size, max_swap, min_pts;
    float eps;
    int minkowski_n = 2;
    if (model_name == "clara")
    {
        clouster_count = stoi(argv[3]);
        random_group_count = stoi(argv[4]);
        random_group_size = stoi(argv[5]);
        max_swap = stoi(argv[6]);

        string params = (model_name + "_" + data_name +
                         "_C" + std::to_string(clouster_count) +
                         "_GC" + std::to_string(random_group_count) +
                         "_GS" + std::to_string(random_group_size) +
                         "_MS" + std::to_string(max_swap));
        out_name = ("OUT_" + params + ".csv");
        stat_name = ("STAT_" + params + ".csv");
        debug_name = ("DEBUG_" + params + ".csv");
    }
    else if (model_name == "dbscan")
    {
        eps = stof(argv[3]);
        min_pts = stoi(argv[4]);

        string params = (model_name + "_" + data_name + "_eps" + std::to_string(eps) + "_min_pts" + std::to_string(min_pts) + "_GS");
        out_name = ("OUT_" + params + ".csv");
        stat_name = ("STAT_" + params + ".csv");
        debug_name = ("DEBUG_" + params + ".csv");
    }

    auto start_program_time = high_resolution_clock::now();

    Data dataset = load_data(data_name);
    auto end_loading_data_time = high_resolution_clock::now();
    auto data_load_duration = duration_cast<microseconds>(end_loading_data_time - start_program_time);

    vector<int> clustering;
    vector<string> times;
    if (model_name == "clara")
    {
        auto clustering_res = clara(dataset.points, clouster_count, random_group_count, random_group_size, minkowski_n, max_swap);
        clustering = get<0>(clustering_res);
        times = get<1>(clustering_res);
    }
    else if (model_name == "dbscan")
    {
        clustering = dbscan(dataset.points, eps, min_pts, minkowski_n);
    }

    auto end_program_time = high_resolution_clock::now();
    auto program_duration = duration_cast<microseconds>(end_program_time - start_program_time);
    cout << "Processing time: "
         << program_duration.count() / 1000000 << " seconds" << endl;

    //PERPARE OUT

    string out_text = "";
    for (int i = 0; i < dataset.points.size(); i++)
    {
        string row_text = "";
        row_text += std::to_string(dataset.points[i]->index) + ",";
        for (int d = 0; d < dataset.points[i]->coords.size(); d++)
            row_text += std::to_string(dataset.points[i]->coords[d]) + ","; //na pozniej - poprawic zeby zapisywal jako inty nie floaty

        row_text += std::to_string(dataset.points[i]->metric_calculate_count) + ",";
        row_text += "0,"; // POINT TYPE!!!
        row_text += std::to_string(clustering[i]);
        row_text += '\n';

        out_text += row_text;
    }
    write_to_file(out_name, out_text);

    //PERPARE STATS

    string stat_text = "";

    stat_text += "Data name: " + data_name + "\n";
    if (model_name == "clara")
    {
        stat_text += "Cluster count: " + std::to_string(clouster_count) + "\n";
        stat_text += "Random group count: " + std::to_string(random_group_count) + "\n";
        stat_text += "Random group size: " + std::to_string(random_group_size) + "\n";
        stat_text += "Max swaps:" + std::to_string(max_swap) + "\n";
    }
    else if (model_name == "dbscan")
    {
        stat_text += "eps: " + std::to_string(eps) + "\n";
        stat_text += "min_pts: " + std::to_string(min_pts) + "\n";
    }

    stat_text += "Time of data loading [s]: " + std::to_string(data_load_duration.count() / 1000000) + "\n";

    if (model_name == "clara")
    {
        for (int i = 0; i < times.size(); ++i)
        {
            stat_text += "Time of " + to_string(i) + " PAM [s]: " + times[i] + "\n";
        }
    }
    else if (model_name == "dbscan")
    {
    }

    stat_text += "Time of processing [s]: " + std::to_string(program_duration.count() / 1000000) + "\n";
    if (model_name == "dbscan")
    {

        //statistics about count of clusters etc.
    }

    float mean_minkowski_count = 0.0;
    for (int i = 0; i < dataset.points.size(); ++i)
        mean_minkowski_count += dataset.points[i]->metric_calculate_count;
    mean_minkowski_count /= dataset.points.size();
    stat_text += "Avg metric calculation per point: " + std::to_string(mean_minkowski_count) + "\n";

    auto rand_res = rand_score(dataset.labels, clustering);
    stat_text += "TP : " + std::to_string(std::get<1>(rand_res)) + "\n";
    stat_text += "TN : " + std::to_string(std::get<2>(rand_res)) + "\n";
    stat_text += "# pairs : " + std::to_string(std::get<3>(rand_res)) + "\n";
    stat_text += "Rand score : " + std::to_string(std::get<0>(rand_res)) + "\n";

    stat_text += "Pruity score : " + std::to_string(pruity_score(dataset.labels, clustering)) + "\n";
    stat_text += "Silhouette coefficient : " + std::to_string(silhouette_coefficient(dataset.points, clustering)) + "\n";

    if (model_name == "clara")
        stat_text += "Davies Bouldin : " + std::to_string(davies_bouldin_score(dataset.points, clustering)) + "\n";

    write_to_file(stat_name, stat_text);

    //TODO free dataset
    return 0;
}