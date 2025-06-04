#pragma once

#include <iostream>

// Definições para as cores dos nós na árvore rubro-negra
#define RED false
#define BLACK true

// Estrutura que define um nó da árvore rubro-negra
template <typename type>
struct node {
    type key;      // Chave do nó
    bool color;    // Cor do nó (RED ou BLACK)
    node *left;    // Ponteiro para o filho esquerdo
    node *right;   // Ponteiro para o filho direito
    node *parent;  // Ponteiro para o nó pai

    // Construtor para inicializar um nó com uma chave, cor, e ponteiros para o
    // filho esquerdo, direito e o pai
    node(type k, bool c, node *l, node *r, node *p)
        : key(k), color(c), left(l), right(r), parent(p) {}
};
