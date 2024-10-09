import numpy as np

# Initialize data points
data_points = np.array([
    [15, 10], [3, 10], [15, 12], [3, 14], [18, 13], [1, 7], [10, 1], [10, 30]
])



# Function to calculate Euclidean distance
def euclidean_distance(p1, p2):
    return np.sqrt(np.sum((p1 - p2) ** 2))

# Function to assign clusters based on centroids
def assign_clusters(data, centroids):
    clusters = []
    for point in data:
        distances = [euclidean_distance(point, centroid) for centroid in centroids]
        cluster = np.argmin(distances)
        clusters.append(cluster)
    return np.array(clusters)

# Function to update centroids based on current clusters
def update_centroids(data, clusters, k):
    new_centroids = []
    for i in range(k):
        cluster_points = data[clusters == i]
        new_centroid = np.mean(cluster_points, axis=0)
        new_centroids.append(new_centroid)
    return np.array(new_centroids)

# K-means algorithm
def k_means(data, centroids, k, max_iters=100):
    for _ in range(max_iters):
        clusters = assign_clusters(data, centroids)
        new_centroids = update_centroids(data, clusters, k)
        
        # If centroids don't change, stop
        if np.all(centroids == new_centroids):
            break
        centroids = new_centroids
    return centroids, clusters

# Apply k-means
def apply_k_means(data, centroids, k):
    final_centroids, final_clusters = k_means(data, centroids, k)
    clusters_info = {}
    for i in range(k):
        clusters_info[f"Cluster {i+1}"] = {
            "Centroid": final_centroids[i],
            "Points": data[final_clusters == i]
        }
    return clusters_info

def main():
    # Initialize k = 2 and the initial means
    centroids = np.array([[10, 1], [10, 30]])
    clusters_info = apply_k_means(data_points, centroids, 2)
    print("\nK-Means Clustering")
    for cluster, info in clusters_info.items():
        print(f"{cluster} - Centroid: {info['Centroid']}, Points: {info['Points']}")
    
    # Initialize k = 2 and the initial means
    centroids = np.array([[3, 10], [15, 10]])
    clusters_info = apply_k_means(data_points, centroids, 2)
    print("\nK-Means Clustering")
    for cluster, info in clusters_info.items():
        print(f"{cluster} - Centroid: {info['Centroid']}, Points: {info['Points']}")

    centroids = np.array([[10, 1], [10, 30], [3, 10]])
    clusters_info = apply_k_means(data_points, centroids, 3)
    print("\nK-Means Clustering")
    for cluster, info in clusters_info.items():
        print(f"{cluster} - Centroid: {info['Centroid']}, Points: {info['Points']}") 

    centroids = np.array([[10, 1], [10, 30], [3, 10], [15, 10]])
    clusters_info = apply_k_means(data_points, centroids, 4)
    print("\nK-Means Clustering")
    for cluster, info in clusters_info.items():
        print(f"{cluster} - Centroid: {info['Centroid']}, Points: {info['Points']}") 
        
if __name__ == "__main__":
    main()