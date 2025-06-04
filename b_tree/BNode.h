#ifndef BNODE_H
#define BNODE_H

#include <utility>  // Para std::pair

template <typename T>
struct Bnode {
    int n;          // número atual de chaves
    bool leaf;      // true se e somente se esse nó for uma folha
    std::pair<T, int> *key;  // vetor de chaves com um int associado a cada chave
    Bnode<T> **child;         // vetor de ponteiros para os filhos do nó

    // Construtor: cria um nó vazio.
    // Recebe como entrada o grau mínimo do nó e um booleano
    // indicando se ele é ou não folha.
    Bnode(int degree, bool is_leaf) {
        leaf = is_leaf;                 
        n = 0; 
        key = new std::pair<T, int>[2*degree-1];  // Aloca espaço para as chaves com int associado
        child = new Bnode<T>*[2*degree];  // Aloca espaço para os ponteiros para os filhos
    }

    // Destrutor: quando um nó for deletado,
    // tem que liberar os vetores alocados.
    ~Bnode() {
        delete[] key;
        delete[] child;
    }
};

#endif
