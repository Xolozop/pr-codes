import copy
import random
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

# =============== Реализация умножения ===============
def classical_mult(A, B):
    n, m = A.shape[0], B.shape[1] # размерность матрицы (строк A и столбцов B соот.)
    C = np.zeros((n, m))
    for i in range(n):
        for j in range(m):
            for k in range(A.shape[1]):
                C[i, j] += A[i, k] * B[k, j]
    return C

def logical_mult(A, B):
    n, m = A.shape[0], B.shape[1]
    C = np.zeros((n, m))
    for i in range(n):
        for j in range(m):
            for k in range(A.shape[1]):
                C[i, j] = C[i, j] or A[i, k] and B[k, j]
    return C

def tropical_mult_max(A, B):
    n, m = A.shape[0], B.shape[1] # размерность матрицы (строки и столбца соот.)
    C = np.zeros((n, m))
    for i in range(n):
        for j in range(m):
            for k in range(A.shape[1]):
                C[i, j] = max(C[i, j], A[i, k] + B[k, j])
    return C

def tropical_mult_min(A, B):
    n, m = A.shape[0], A.shape[1] # размерность матрицы (строки и столбца соот.)
    C = np.full((n, m), np.inf)
    for i in range(n):
        for j in range(m):
            for k in range(A.shape[1]):
                if A[i, k] + B[k, j] != 0:
                    C[i, j] = min(C[i, j], A[i, k] + B[k, j])
    return C


# =============== Генерация графов ===============
def get_random_graph(n=10, k=12):
    G = nx.gnm_random_graph(n, k, directed=True)
    nodes = copy.deepcopy(nx.nodes(G))
    for node in nodes:
        if len(G.out_edges(node)) == 0: # устраняем вершины без связей
            G.add_edge(node, random.randint(0, n))
    return G

def get_petersen_graph():
    G = nx.petersen_graph().to_directed()
    prev_node = 1
    for node in nx.nodes(G):
        li = list(G.in_edges(node))
        for e in li:
            rand = random.choice([0, 1])
            if (e[rand], e[1-rand]) in nx.edges(G) and (e[1-rand], e[rand]) in nx.edges(G):
                G.remove_edge(e[rand], e[1-rand])
    return G


# =============== Интерфейс ===============
def visualize_graph(G):
    pos = nx.spring_layout(G)
    nx.draw(G, pos, with_labels=True, node_color='lightblue', edge_color='gray', node_size=500)
    plt.show()

def print_matrix(A):
    n, m = A.number_of_nodes()

    print("\033[4m | ", end='')
    for i in range(n):
        print(i, end=' ')
    print("\033[0m")

    for i in range(n):
        for j in range(n+1):
            if j == 0:
                print(f'{i}|', end=' ')
            else:
                if A[i, j-1] == np.inf:
                    A[i, j-1] = 0
                print(int(A[i, j-1]), end=' ')
        print('')
    print('')
    return A

# Example
'''A = np.array([
    [0, 0, 0, 0, 1, 0, 0, 0, 0, 1],
    [0, 0, 1, 0, 1, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [0, 0, 1, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [0, 0, 0, 1, 1, 0, 0, 0, 0, 0],
    [0, 0, 1, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 1, 0, 1, 0, 0, 0, 0],
    [0, 1, 0, 0, 0, 0, 1, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 1, 0, 0, 0]
])

A = np.array([
    [0, 0, 0, 0, 1, 1, 0, 0, 0, 0],
    [1, 0, 1, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 1, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 1, 0, 0, 0, 0, 0, 1],
    [0, 0, 0, 0, 0, 0, 0, 1, 1, 0],
    [0, 1, 0, 0, 0, 0, 0, 0, 1, 1],
    [0, 0, 1, 0, 0, 0, 0, 0, 0, 1],
    [0, 0, 0, 1, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
])
G = nx.from_numpy_array(A, create_using=nx.MultiDiGraph())
visualize_graph(G)

# Получение матрицы смежности
A = nx.to_numpy_array(G)
print(" Исходный граф:")
print_matrix(A)

classical_res = classical_mult(A, A)
logical_res = logical_mult(A, A)
tropical_res_max = tropical_mult_max(A, A)
tropical_res_min = tropical_mult_min(A, A)
print(" Классическое умножение:")
print_matrix(classical_res)
# visualize_graph(nx.from_numpy_array(classical_res, create_using=nx.MultiDiGraph()))
print(" Логическое умножение:")
print_matrix(logical_res)
# visualize_graph(nx.from_numpy_array(logical_res, create_using=nx.MultiDiGraph()))
print(" Тропическое умножение (max):")
print_matrix(tropical_res_max)
# visualize_graph(nx.from_numpy_array(tropical_res_max, create_using=nx.MultiDiGraph()))
print(" Тропическое умножение (min):")
print_matrix(tropical_res_min)
# visualize_graph(nx.from_numpy_array(tropical_res_min, create_using=nx.MultiDiGraph()))
'''

# Expr 1
'''
G = get_petersen_graph()
g1 = nx.to_numpy_array(G)
visualize_graph(G)
for i in range(2, 5):
    cls_res = classical_mult(A, A)
    log_res = logical_mult(A, A)
    trp_res = tropical_mult_max(A, A)
    visualize_graph(nx.from_numpy_array(cls_res, create_using=nx.MultiDiGraph()))
    visualize_graph(nx.from_numpy_array(log_res, create_using=nx.MultiDiGraph()))
    visualize_graph(nx.from_numpy_array(trp_res, create_using=nx.MultiDiGraph()))
'''

# Expr 2
'''
G1 = get_petersen_graph()
G2 = get_petersen_graph()
A = get_random_graph()
B = get_random_graph()

g1 = nx.to_numpy_array(G1)
g2 = nx.to_numpy_array(G2)
a = nx.to_numpy_array(A)
b = nx.to_numpy_array(B)

visualize_graph(G1)
visualize_graph(G2)
visualize_graph(A)
visualize_graph(B)

visualize_graph(nx.from_numpy_array(classical_mult(a, b), create_using=nx.MultiDiGraph()))
visualize_graph(nx.from_numpy_array(logical_mult(a, b), create_using=nx.MultiDiGraph()))
visualize_graph(nx.from_numpy_array(classical_mult(g1, a), create_using=nx.MultiDiGraph()))
visualize_graph(nx.from_numpy_array(logical_mult(g1, a), create_using=nx.MultiDiGraph()))
visualize_graph(nx.from_numpy_array(classical_mult(g1, g2), create_using=nx.MultiDiGraph()))
visualize_graph(nx.from_numpy_array(logical_mult(g1, g2), create_using=nx.MultiDiGraph()))
'''
