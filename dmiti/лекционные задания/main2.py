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

def tropical_mult(A, B):
    n, m = A.shape[0], B.shape[1] # размерность матрицы (строки и столбца соот.)
    C = np.zeros((n, m))
    for i in range(n):
        for j in range(m):
            for k in range(A.shape[1]):
                C[i, j] = max(C[i, j], A[i, k] + B[k, j])
    return C


# =============== Генерация графов ===============
def get_random_graph(n=10, k=12):
    G = nx.gnm_random_graph(n, k, directed=True)
    for node in nx.nodes(G):
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
    n, m = A.shape[0], A.shape[1]

    print("\033[4m | ", end='')
    for i in range(m):
        print(i, end=' ')
    print("\033[0m")

    for i in range(n):
        for j in range(m+1):
            if j == 0:
                print(f'{i}|', end=' ')
            else:
                print(int(A[i, j-1]), end=' ')
        print('')
    print('')


# Генерация графа Петерсена
# G = get_petersen_graph()
# Генерация случайного графа
G = get_random_graph()

# Получение матрицы смежности
A = nx.to_numpy_array(G)
print(" Исходный граф:")
print_matrix(A)

classical_res = classical_mult(A, A)
logical_res = logical_mult(A, A)
tropical_res = tropical_mult(A, A)
print(" Классическое умножение:")
print_matrix(classical_res)
print(" Логическое умножение:")
print_matrix(logical_res)
print(" Тропическое умножение:")
print_matrix(tropical_res)

visualize_graph(G)