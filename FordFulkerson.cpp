#include <iostream>
#include <vector>
#include <climits>

using namespace std;

class Grapho {
private:
	vector<vector<int>> matriz_adjacente;

	// Função responsável por adicionar um vértice
	void add_vertice(size_t vertice) {
		matriz_adjacente.resize(vertice + 1);
		for (int i = 0; i < vertice + 1; i++) {
			matriz_adjacente[i].resize(vertice + 1, false);
		}
	}

public:

	Grapho(vector<vector<int>>& matriz) {
		matriz_adjacente = matriz;
	}

	Grapho() {
		matriz_adjacente = vector<vector<int>>();
	}

	// Função para adicionar uma aresta ao grafo
	void insert(int vertice1, int vertice2, int peso) {
		if (vertice1 >= int(matriz_adjacente.size())) {
			add_vertice(vertice1);
		}
		if (vertice2 >= int(matriz_adjacente.size())) {
			add_vertice(vertice2);
		}

		matriz_adjacente[vertice1][vertice2] = peso;
	}

	void insert(int vertice1, int vertice2) {
		if (vertice1 >= int(matriz_adjacente.size())) {
			add_vertice(vertice1);
		}
		if (vertice2 >= int(matriz_adjacente.size())) {
			add_vertice(vertice2);
		}

		matriz_adjacente[vertice1][vertice2] = 1;
	}

	void imprimirMatriz() {
		cout << "Matriz de adjacencia: \n   " << endl << "  ";
		for (int i = 0; i < int(matriz_adjacente.size()); i++) {
			cout << i << " ";
		}
		printf("\n");
		for (int i = 0; i < int(matriz_adjacente.size()); i++) {
			cout << i << " ";
			for (int aresta : matriz_adjacente[i]) {
				cout << aresta << " ";
			}
			cout << endl;
		}
	}

	friend int ford_fulkerson(Grapho& matriz, int s, int t);

};

// Algoritmo de Ford-Fulkerson com DFS
int ford_fulkerson(Grapho& matriz, int s, int t) {
	const size_t n = matriz.matriz_adjacente.size();
	vector<vector<int>> matriz_residual(n, vector<int>(n, 0));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			matriz_residual[i][j] = matriz.matriz_adjacente[i][j];
		}
	}

	vector<int> caminho(n, -1);

	int fluxo_maximo = 0;

	while (true) {
		vector<bool> visitado(n, false);
		vector<int> pilha;
		pilha.push_back(s);
		visitado[s] = true;

		while (!pilha.empty()) {
			int u = pilha.back();
			pilha.pop_back();

			for (int v = 0; v < n; ++v) {
				if (!visitado[v] && matriz_residual[u][v] > 0) {
					pilha.push_back(v);
					visitado[v] = true;
					caminho[v] = u;
				}
			}
		}

		if (!visitado[t]) {
			break;
		}

		int fluxo = INT_MAX;
		for (int v = t; v != s; v = caminho[v]) {
			int u = caminho[v];
			fluxo = min(fluxo, matriz_residual[u][v]);
		}

		fluxo_maximo += fluxo;

		for (int v = t; v != s; v = caminho[v]) {
			int u = caminho[v];
			matriz_residual[u][v] -= fluxo;
			matriz_residual[v][u] += fluxo;
		}
	}

	return fluxo_maximo;
}

int main() {

	/*
	* Exemplo prático:
	* Um sistema de distribuição de água possui 8 reservatórios interligados por canos.
	* A capacidade de cada cano é dada em litros por segundo.
	* As arestas do grafo representam os canos e os vértices representam os reservatórios.
	* O peso de cada aresta é a capacidade de água que o cano pode transportar.
	* O objetivo é determinar o fluxo máximo de água que pode ser distribuído do reservatório 0 ao reservatório 7.
	* O fluxo máximo é a quantidade máxima de água que pode ser transportada do reservatório 0 ao reservatório 7.
	* O fluxo máximo é de 26 litros por segundo.
	*/
	Grapho grafo;

	grafo.insert(0, 1, 13);
	grafo.insert(0, 2, 10);
	grafo.insert(0, 3, 10);
	grafo.insert(1, 4, 24);
	grafo.insert(2, 1, 5);
	grafo.insert(2, 3, 15);
	grafo.insert(2, 6, 6);
	grafo.insert(3, 6, 15);
	grafo.insert(4, 5, 1);
	grafo.insert(4, 7, 9);
	grafo.insert(5, 6, 6);
	grafo.insert(5, 7, 15);
	grafo.insert(6, 7, 16);

	auto fluxo_maximo = ford_fulkerson(grafo, 0, 7);

	//grafo.imprimirMatriz();
	cout << "Fluxo máximo: " << fluxo_maximo << endl;
	return 0;
}