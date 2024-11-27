import networkx as nx
import matplotlib.pyplot as plt

def read_graph_from_file(filename):
    """Читает граф из файла."""
    with open(filename, 'r') as file:
        lines = file.readlines()
        n, m = map(int, lines[0].split())  # Читаем количество вершин и рёбер
        edges = [tuple(map(int, line.split())) for line in lines[1:m+1]]  # Читаем рёбра
        colors = lines[m+1:]  # Читаем цвета вершин
        colors = [color.strip() for color in colors]
    return n, m, edges, colors

def visualize_graph(n, edges, colors):
    """Визуализирует граф с раскраской."""
    # Создаём граф
    G = nx.Graph()
    G.add_edges_from(edges)

    pos = nx.spring_layout(G)

    plt.figure(figsize=(8, 6))
    nx.draw(
        G,
        pos,
        with_labels=True,
        node_color=colors, 
        node_size=800,
        font_weight='bold',
        font_color='white'
    )
    plt.title("Визуализация графа")
    plt.show()

filename = "correct_graph.txt"  
n, m, edges, colors = read_graph_from_file(filename)
visualize_graph(n, edges, colors)
