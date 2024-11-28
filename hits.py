import numpy as np

# Adjacency matrix
M = np.array([
    [0, 1, 1, 0],  # A -> B, C
    [0, 0, 0, 0],  # B -> None
    [0, 0, 0, 1],  # C -> D
    [0, 0, 1, 0]   # D -> C
], dtype=float)

# M = np.array([
#     [1, 1, 1],  # A -> B, C
#     [0, 0, 1],  # B -> None
#     [1, 1, 0],  # C -> D
# ], dtype=float)

def normalize(vec):
    return vec / np.linalg.norm(vec, ord=1) * len(vec)

def hits_algorithm(M, iterations=100, tolerance=1e-7):
    n = M.shape[0]
    authority = np.ones(n)
    hub = np.ones(n)

    for _ in range(iterations):
        # Update authority
        new_authority = normalize(M.T @ hub)

        # Update hub
        new_hub = normalize(M @ authority)

        # Check for convergence
        if (np.allclose(authority, new_authority, atol=tolerance) and
            np.allclose(hub, new_hub, atol=tolerance)):
            break

        authority, hub = new_authority, new_hub

    return authority, hub

def rank_sites(authority, hub):
    # Compute the final score for each site
    scores = 0.6 * authority + 0.4 * hub
    # Pair scores with site names
    sites = [(chr(65 + i), scores[i]) for i in range(len(scores))]
    # Sort by score (descending) and then alphabetically
    sites.sort(key=lambda x: (-x[1], x[0]))
    return sites

authority, hub = hits_algorithm(M)

# Rank the sites
ranking = rank_sites(authority, hub)

# Print the results
print("Site Rankings:")
for site, score in ranking:
    print(f"{site}: Score = {score:.6f}")


# Print normalized results
for i, (a, h) in enumerate(zip(authority, hub)):
    print(f"Node {chr(65 + i)}: Authority = {a:.6f}, Hub = {h:.6f}")