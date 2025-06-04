#pragma once

// Estrutura de um nó da árvore AVL
template <typename type>
struct node {
    type key;             // Chave do nó
    unsigned int height;  //  Altura do nó
    node* left;           // Ponteiro para o filho esquerdo
    node* right;          // Ponteiro para o filho direito

    // Construtor
    node(type key, unsigned int height, node* left, node* right) {
        this->key = key;
        this->height = height;
        this->left = left;
        this->right = right;
    }
};