#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <numeric>  // For std::accumulate

enum class LinkageType {
    COMPLETE, // Complete linkage (max distance)
    SINGLE,   // Single linkage (min distance)
    AVERAGE   // Group average linkage (mean distance)
};

// Function to calculate cluster distance based on linkage type
double calculateClusterDistance(const std::vector<int>& cluster1, const std::vector<int>& cluster2, 
                                const std::vector<std::vector<double>>& distanceMatrix, 
                                LinkageType linkage) {
    if (cluster1.size() == 0 || cluster2.size() == 0) return 0.0;
    std::vector<double> distances;
    for (int i : cluster1) {
        for (int j : cluster2) {
            distances.push_back(distanceMatrix[i][j]);
        }
    }
    switch (linkage) {
        case LinkageType::COMPLETE:  // Max distance
            return *std::max_element(distances.begin(), distances.end());
        case LinkageType::SINGLE:    // Min distance
            return *std::min_element(distances.begin(), distances.end());
        case LinkageType::AVERAGE:   // Average distance
            return std::accumulate(distances.begin(), distances.end(), 0.0) / distances.size();
    }
    return 0.0;
}

// Function to split a cluster based on distance to maximize between-cluster distance
std::pair<std::vector<int>, std::vector<int>> splitCluster(const std::vector<int>& cluster, 
                                                           const std::vector<std::vector<double>>& distanceMatrix, 
                                                           LinkageType linkage) {
    int n = cluster.size();
    if (n <= 1) return {{ }, { }}; // Cannot split further

    // Start by assigning the two furthest apart points into separate clusters
    int point = -1;
    double maxDist = -std::numeric_limits<double>::infinity();
    for (int i = 0; i < n; ++i) {
        std::vector<int> tmpCluster1 = {cluster[i]};
        std::vector<int> tmpCluster2 = { };
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            tmpCluster2.push_back(cluster[j]);
        }
        double dist = calculateClusterDistance(tmpCluster1, tmpCluster2, distanceMatrix, linkage);
        if (dist > maxDist) {
            maxDist = dist;
            point = i;
        }
    }

    std::vector<int> cluster1 = {cluster[point]}, cluster2;
    for (int i = 0; i < n; ++i) {
        if (i == point) continue;
        cluster2.push_back(cluster[i]);
    }
    
    while (true) {
        // Assign the remaining points to one of the first clusters based on linkage distance
        int maxPoint = -1;
        double maxDelta = -std::numeric_limits<double>::infinity();
        for (int i = 0; i < cluster2.size(); ++i) {
            std::vector<int> tmpCluster = { };
            for (int j = 0; j < cluster2.size(); ++j) {
                if (i == j) continue;
                tmpCluster.push_back(cluster2[j]);
            }

            double distToCluster1 = calculateClusterDistance({cluster2[i]}, cluster1, distanceMatrix, linkage);
            double distToCluster2 = calculateClusterDistance({cluster2[i]}, tmpCluster, distanceMatrix, linkage);
            double delta = distToCluster2 - distToCluster1;

            if (delta < 0) continue;
            
            if (delta > maxDelta) {
                maxDelta = delta;
                maxPoint = i;
            }

        }
        // No point to move
        if (maxPoint == -1) break;
        // found a point to move
        cluster1.push_back(cluster2[maxPoint]);
        cluster2.erase(cluster2.begin() + maxPoint);
        
    }

    return {cluster1, cluster2};
}

// Recursive function to perform divisive clustering
void divisiveClustering(const std::vector<int>& cluster, 
                        const std::vector<std::vector<double>>& distanceMatrix, 
                        LinkageType linkage, int depth = 0) {
    // Print the current cluster
    std::cout << std::string(depth, '-') << "Cluster: ";
    for (int i : cluster) std::cout << i << " ";
    std::cout << std::endl;

    // Split the cluster
    auto [cluster1, cluster2] = splitCluster(cluster, distanceMatrix, linkage);

    // Recursively split each sub-cluster
    if (!cluster1.empty()) divisiveClustering(cluster1, distanceMatrix, linkage, depth + 1);
    if (!cluster2.empty()) divisiveClustering(cluster2, distanceMatrix, linkage, depth + 1);
}

int main() {
    // // Example pairwise distance matrix (symmetric)
    // std::vector<std::vector<double>> distanceMatrix = {
    //     { 0.0, 10.0,  7.0, 30.0, 29.0, 38.0, 42.0},
    //     {10.0,  0.0,  7.0, 23.0, 25.0, 24.0, 36.0},
    //     { 7.0,  7.0,  0.0, 21.0, 22.0, 31.0, 36.0},
    //     {30.0, 23.0, 21.0,  0.0,  7.0, 10.0, 13.0},
    //     {29.0, 25.0, 22.0,  7.0,  0.0, 11.0, 17.0},
    //     {38.0, 24.0, 31.0, 10.0, 11.0,  0.0,  9.0},
    //     {42.0, 36.0, 36.0, 13.0, 17.0,  9.0,  0.0},
    // };

    // // Initial cluster containing all points
    // std::vector<int> initialCluster = {0, 1, 2, 3, 4, 5, 6};

    // // Perform divisive clustering with a chosen linkage type
    // divisiveClustering(initialCluster, distanceMatrix, LinkageType::AVERAGE);

    // Example pairwise distance matrix (symmetric)
    std::vector<std::vector<double>> distanceMatrix = {
        { 0.0, 11.0,  5.0, 12.0,  7.0, 13.0,  9.0, 11.0},
        {11.0,  0.0, 13.0,  2.0, 17.0,  4.0, 15.0, 20.0},
        { 5.0, 13.0,  0.0, 14.0,  1.0, 15.0, 12.0, 12.0},
        {12.0,  2.0, 14.0,  0.0, 18.0,  5.0, 16.0, 21.0},
        { 7.0, 17.0,  1.0, 18.0,  0.0, 20.0, 15.0, 17.0},
        {13.0,  4.0, 15.0,  5.0, 20.0,  0.0, 19.0, 22.0},
        { 9.0, 15.0, 12.0, 16.0, 15.0, 19.0,  0.0, 30.0},
        {11.0, 20.0, 12.0, 21.0, 17.0, 22.0, 30.0,  0.0},
    };

    // Initial cluster containing all points
    std::vector<int> initialCluster = {0, 1, 2, 3, 4, 5, 6, 7};

    // Perform divisive clustering with a chosen linkage type
    divisiveClustering(initialCluster, distanceMatrix, LinkageType::COMPLETE);

    return 0;
}