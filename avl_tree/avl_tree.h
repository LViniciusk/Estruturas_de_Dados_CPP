#pragma once

#include <iostream>
#include <string>

#include "node.h"

// Classe que representa uma árvore AVL, uma árvore binária de busca balanceada
template <typename type>
class avl_tree {
   private:
    node<type>* _root = nullptr;  // Raiz da árvore
    unsigned int _size = 0;       // Número de elementos na árvore

    // Limpa a árvore recursivamente desalocando a memória de cada nó
    void _clear(node<type>* n) {
        if (n == nullptr) {
            return;
        }

        _clear(n->left);
        _clear(n->right);
        delete n;
    }

    // Retorna uma cópia da árvore
    node<type>* _copy(node<type>* other_root) {
        if (other_root == nullptr) {
            return nullptr;
        }

        node<type>* new_node = new node<type>(
            other_root->key, other_root->height, nullptr, nullptr);
        new_node->left = _copy(other_root->left);
        new_node->right = _copy(other_root->right);
        return new_node;
    }

    // Retorna a altura de um nó
    int _height(node<type>* n) const {
        if (n == nullptr) {
            return 0;
        }
        return n->height;
    }

    // Calcula o fator de balanceamento de um nó
    int _balance(node<type>* n) { return _height(n->right) - _height(n->left); }

    // Faz uma rotação para a direita em um nó
    node<type>* _right_rotation(node<type>* p) {
        node<type>* u = p->left;
        p->left = u->right;
        u->right = p;

        p->height = 1 + std::max(_height(p->left), _height(p->right));
        u->height = 1 + std::max(_height(u->left), _height(u->right));

        return u;
    }

    // Faz uma rotação para a esquerda em um nó
    node<type>* _left_rotation(node<type>* p) {
        node<type>* u = p->right;
        p->right = u->left;
        u->left = p;

        p->height = 1 + std::max(_height(p->left), _height(p->right));
        u->height = 1 + std::max(_height(u->left), _height(u->right));

        return u;
    }

    // Insere uma chave na árvore
    node<type>* _insert(node<type>* p, int key) {
        if (p == nullptr) {
            _size++;
            return new node<type>{key, 1, nullptr, nullptr};
        }

        if (key == p->key) {
            return p;
        }

        if (key < p->key) {
            p->left = _insert(p->left, key);
        } else {
            p->right = _insert(p->right, key);
        }

        p = _fixup_node(p, key);
        return p;
    }

    // Corrige o balanceamento de um nó após uma inserção
    node<type>* _fixup_node(node<type>* p, int key) {
        p->height = 1 + std::max(_height(p->left), _height(p->right));

        int bal = _balance(p);

        if (bal < -1 && key < p->left->key) {
            return _right_rotation(p);
        }

        else if (bal < -1 && key > p->left->key) {
            p->left = _left_rotation(p->left);
            return _right_rotation(p);
        }

        else if (bal > 1 && key > p->right->key) {
            return _left_rotation(p);
        }

        else if (bal > 1 && key < p->right->key) {
            p->right = _right_rotation(p->right);
            return _left_rotation(p);
        }

        return p;
    }

    // Remove uma chave da árvore
    node<type>* _remove(node<type>* n, int key) {
        if (n == nullptr) {
            return nullptr;
        }

        if (key < n->key) {
            n->left = _remove(n->left, key);
        } else if (key > n->key) {
            n->right = _remove(n->right, key);
        } else if (n->right == nullptr) {
            node<type>* temp = n->left;
            delete n;
            _size--;
            return temp;
        } else {
            n->right = _remove_successor(n, n->right);
            _size--;
        }

        n = _fixup_deletion(n);

        return n;
    }

    // Remove o sucessor de um nó
    node<type>* _remove_successor(node<type>* n, node<type>* successor) {
        if (successor->left != nullptr) {
            successor->left = _remove_successor(n, successor->left);
        } else {
            n->key = successor->key;
            node<type>* temp = successor->right;
            delete successor;
            return temp;
        }
        successor = _fixup_deletion(successor);
        return successor;
    }

    // Corrige o balanceamento de um nó após uma remoção
    node<type>* _fixup_deletion(node<type>* n) {
        if (n == nullptr) {
            return nullptr;
        }

        n->height = 1 + std::max(_height(n->left), _height(n->right));

        int bal = _balance(n);

        if (bal > 1 && _balance(n->right) >= 0) {
            return _left_rotation(n);
        }

        if (bal > 1 && _balance(n->right) < 0) {
            n->right = _right_rotation(n->right);
            return _left_rotation(n);
        }

        if (bal < -1 && _balance(n->left) <= 0) {
            return _right_rotation(n);
        }

        if (bal < -1 && _balance(n->left) > 0) {
            n->left = _left_rotation(n->left);
            return _right_rotation(n);
        }

        return n;
    }

    // Busca uma chave na árvore (Busca binária)
    node<type>* _search(node<type>* n, int key) {
        if (n == nullptr) {
            return nullptr;
        }

        if (n->key == key) {
            return n;
        }

        if (key < n->key) {
            return _search(n->left, key);
        }
        return _search(n->right, key);
    }

    // Retorna a menor chave da árvore
    node<type>* _minimum(node<type>* n) {
        if (n == nullptr) {
            return nullptr;
        }

        while (n->left != nullptr) {
            n = n->left;
        }

        return n;
    }

    // Retorna a maior chave da árvore
    node<type>* _maximum(node<type>* n) {
        if (n == nullptr) {
            return nullptr;
        }

        while (n->right != nullptr) {
            n = n->right;
        }

        return n;
    }

    // Retorna o sucessor de uma chave
    node<type>* _successor(node<type>* n, node<type>* succ, int key) {
        if (n == nullptr) {
            if (succ == nullptr) {
                return nullptr;
            }
            return succ;
        }

        if (n->key > key) {
            return _successor(n->left, n, key);
        }
        return _successor(n->right, succ, key);
    }

    // Retorna o predecessor de uma chave
    node<type>* _predecessor(node<type>* n, node<type>* pred, int key) {
        if (n == nullptr) {
            if (pred == nullptr) {
                return nullptr;
            }
            return pred;
        }

        if (n->key < key) {
            return _predecessor(n->right, n, key);
        }
        return _predecessor(n->left, pred, key);
    }

    // Retorna um array contendo as chaves da árvore em ordem
    int* _to_array(node<type>* n, int* arr) {
        if (n == nullptr) {
            return arr;
        }

        arr = _to_array(n->left, arr);
        *arr = n->key;
        arr++;
        arr = _to_array(n->right, arr);

        return arr;
    }

    // Imprime a arvore na tela
    void bshow(node<type>* n, std::string heranca) {
        if (n != nullptr && (n->left != nullptr || n->right != nullptr)) {
            bshow(n->right, heranca + "r");
        }
        for (int i = 0; i < (int)heranca.size() - 1; i++) {
            std::cout << (heranca[i] != heranca[i + 1] ? "│   " : "    ");
        }
        if (heranca != "") {
            std::cout << (heranca.back() == 'r' ? "┌───" : "└───");
        }
        if (n == nullptr) {
            std::cout << "#" << std::endl;
            return;
        }
        std::cout << n->key << std::endl;
        if (n != nullptr && (n->left != nullptr || n->right != nullptr)) {
            bshow(n->left, heranca + "l");
        }
    }

    // Imprime a árvore em ordem
    void _inorder(node<type>* n) {
        if (n == nullptr) {
            return;
        }

        _inorder(n->left);
        std::cout << n->key << " ";
        _inorder(n->right);
    }

    // Imprime a árvore em pré-ordem
    void _preorder(node<type>* n) {
        if (n == nullptr) {
            return;
        }

        std::cout << n->key << " ";
        _preorder(n->left);
        _preorder(n->right);
    }

    // Imprime a árvore em pós-ordem
    void _postorder(node<type>* n) {
        if (n == nullptr) {
            return;
        }

        _postorder(n->left);
        _postorder(n->right);
        std::cout << n->key << " ";
    }

   public:
    // Construtor padrão
    avl_tree() = default;

    // Contrutor de cópia
    avl_tree(const avl_tree& other) {
        _root = _copy(other._root);
        _size = other._size;
    }

    // Destrutor, chama clear para limpar a árvore
    ~avl_tree() { clear(); }

    // Sobrecarga do operador de  atribuição
    avl_tree& operator=(const avl_tree& other) {
        if (this == &other) {
            return *this;
        }

        clear();
        _root = _copy(other._root);
        _size = other._size;

        return *this;
    }

    // Limpa a árvore
    void clear() {
        _clear(_root);
        _root = nullptr;
        _size = 0;
    }

    // Insere uma chave na árvore
    void insert(int key) { _root = _insert(_root, key); }

    // Remove uma chave (se existir) da árvore
    void remove(int key) {
        if (!contains(key)) {
            throw std::runtime_error("Chave nao encontrada");
        }

        _root = _remove(_root, key);
    }

    // Verifica se a árvore está vazia
    bool empty() const { return _size == 0; }

    // Retorna o tamanho da árvore
    unsigned int size() const { return _size; }

    // Retorna a altura da árvore
    unsigned int height() const { return _height(_root); }

    // Verifica se a árvore contém uma chave
    bool contains(int key) { return _search(_root, key) != nullptr; }

    // Retorna a menor chave da árvore
    int minimum() {
        if (empty()) {
            throw std::runtime_error("Empty tree");
        }

        return _minimum(_root)->key;
    }

    // Retorna o maior elemento da árvore
    int maximum() {
        if (empty()) {
            throw std::runtime_error("Empty tree");
        }

        return _maximum(_root)->key;
    }

    // Retorna o sucessor de uma chave (se existir)
    int successor(int key) {
        if (!contains(key)) {
            throw std::runtime_error("Key not found");
        }

        node<type>* s = _successor(_root, nullptr, key);

        if (s == nullptr) {
            throw std::runtime_error("Key has no successor");
        }

        return s->key;
    }

    // Retorna o predecessor de uma chave (se existir)
    int predecessor(int key) {
        if (!contains(key)) {
            throw std::runtime_error("Key not found");
        }

        node<type>* p = _predecessor(_root, nullptr, key);

        if (p == nullptr) {
            throw std::runtime_error("Key has no predecessor");
        }

        return p->key;
    }

    // Retorna um array contendo as chaves da árvore em ordem
    int* to_array() {
        int* arr = new int[_size];
        int* ptr = arr;
        _to_array(_root, ptr);
        return arr;
    }

    // Imprime a árvore em ordem
    void inorder() { _inorder(_root); }

    // Imprime a árvore em pré-ordem
    void preorder() { _preorder(_root); }

    // Imprime a árvore em pós-ordem
    void postorder() { _postorder(_root); }

    // Imprime a árvore na tela
    void print() { bshow(_root, ""); }
};
