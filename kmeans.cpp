#include <iostream>
#include <stdlib.h>
#include <utility>
#include <set>
#include <vector>
#define N 100005
#define K 2

using namespace std;

double pts[N][2] = {{15, 10}, {3, 10}, {15, 12}, {3, 14}, {18, 13}, {1, 7}, {10, 1}, {10, 30}};

double distance(double& x_1, double& y_1, double& x_2, double& y_2) {
    return sqrt((x_1 - x_2) * (x_1 - x_2) + (y_1 - y_2) * (y_1 - y_2));
}

void kmeans(int k, vector<pair<double, double> > means, int n) {
    bool changed = true;
    vector<set<int> > clusters(k);
    unordered_map<int, int> pts_map;
    int iter = 0;
    while (changed) {
        changed = false;
        // asign x_i to closest cluster
        for (int i = 0; i < n; ++i) {
            double min = 10000000.0;
            int idx = -1;
            for (int j = 0; j < k; ++j) {
                double dist = distance(means[j].first, means[j].second, pts[i][0], pts[i][1]);
                if (dist < min) {
                    idx = j;
                    min = dist;
                }
            }
            if (!clusters[idx].count(i)) {
                clusters[idx].insert(i);
                if (pts_map.count(i))
                    clusters[pts_map[i]].erase(i);
                changed = true;
            }
        }
        // calculate new means
        for (int i = 0; i < k; ++i) {
            double sum_x = 0.0, sum_y = 0.0;
            int size = clusters[i].size();
            for (int idx : clusters[i]) {
                sum_x += pts[idx][0];
                sum_y += pts[idx][1];
            }
            means[i].first = sum_x / size;
            means[i].second = sum_y / size;
        }
        // print clusters
        cout << "Iteration: " << iter << endl;
        for (int i = 0; i < k; ++i) {
            cout << "Cluster " << i << ": ";
            for (int idx : clusters[i]) {
                cout << "(" << pts[idx][0] << "," << pts[idx][1] << ") ";
            }
            cout << endl;
            cout << "Mean: " << means[i].first << " " << means[i].second << endl;
        }
        cout << endl;
        ++iter;
    }

}


int main() {
    int n;
    kmeans(2, {{10.0, 1.0}, {10.0, 30.0}}, 8);
    kmeans(2, {{3.0, 10.0}, {15.0, 10.0}}, 8);
    kmeans(3, {{10.0, 1.0}, {10.0, 30.0}, {3.0, 10.0}}, 8);
    kmeans(4, {{10.0, 1.0}, {10.0, 30.0}, {3.0, 10.0}, {15.0, 10.0}}, 8);

    return 0;
}
