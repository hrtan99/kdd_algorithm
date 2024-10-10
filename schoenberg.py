import numpy as np
# reference: https://math.stackexchange.com/questions/1000006/minimum-dimension-to-hold-n-points-with-given-distances
def compute_schoenberg_matrix(distance_matrix):
    n = distance_matrix.shape[0]
    
    # 创建一个全零的矩阵
    B = np.zeros((n - 1, n - 1))
    
    # 根据 Schoenberg 定理计算 B 矩阵
    for i in range(n - 1):
        for j in range(n - 1):
            B[i, j] = 0.5 * (distance_matrix[i, n - 1] ** 2 + distance_matrix[j, n - 1] ** 2 - distance_matrix[i, j] ** 2)
    
    return B

def check_triangle_inequality(distance_matrix):
    n = distance_matrix.shape[0]  # 获取矩阵大小
    for i in range(n):
        for j in range(n):
            for k in range(n):
                # 跳过对角线的元素
                if i != j and i != k and j != k:
                    # 检查三角不等式 a_ij <= a_ik + a_kj
                    if distance_matrix[i][j] > distance_matrix[i][k] + distance_matrix[k][j]:
                        print(f"Triangle inequality violated for indices ({i}, {j}, {k}): "
                              f"{distance_matrix[i][j]} > {distance_matrix[i][k]} + {distance_matrix[k][j]}")
                        return False
    print("The matrix satisfies the triangle inequality.")
    return True

def main():
    # 示例距离矩阵
    distance_matrix = np.array([
        [ 0.0, 11.0,  5.0, 12.0,  7.0, 13.0,  9.0, 11.0],
        [11.0,  0.0, 13.0,  2.0, 17.0,  4.0, 15.0, 20.0],
        [ 5.0, 13.0,  0.0, 14.0,  1.0, 15.0, 12.0, 12.0],
        [12.0,  2.0, 14.0,  0.0, 18.0,  5.0, 16.0, 21.0],
        [ 7.0, 17.0,  1.0, 18.0,  0.0, 20.0, 15.0, 17.0],
        [13.0,  4.0, 15.0,  5.0, 20.0,  0.0, 19.0, 22.0],
        [ 9.0, 15.0, 12.0, 16.0, 15.0, 19.0,  0.0, 30.0],
        [11.0, 20.0, 12.0, 21.0, 17.0, 22.0, 30.0,  0.0],
    ])
    
    print(check_triangle_inequality(distance_matrix))
    
    # 计算 Schoenberg 矩阵
    schoenberg_matrix = compute_schoenberg_matrix(distance_matrix)
    print("Schoenberg Matrix:")
    print(schoenberg_matrix)

    # 计算秩
    rank = np.linalg.matrix_rank(schoenberg_matrix)
    print("Rank of the Schoenberg Matrix:", rank)

    # 计算最小维度 k
    min_dimension = rank
    print("Minimum Dimension k:", min_dimension)

if __name__ == "__main__":
    main()