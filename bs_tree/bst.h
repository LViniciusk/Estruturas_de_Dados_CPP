#pragma once

#include <iostream>

class node {
   public:
    int key;
    node* left;
    node* right;

    node(int key) {
        this->key = key;
        this->left = nullptr;
        this->right = nullptr;
    }
};

class bst {
   private:
    node* root;

    void clear(node* current) {
        if (current != nullptr) {
            clear(current->left);
            clear(current->right);
            delete current;
        }
    }

   public:
    bst() { 
        this->root = nullptr;
    }

    ~bst() {
        clear(this->root);
    }

    void _add(node*& current, int value) {
        if (current == nullptr) {
            current = new node(value);
        } else if (value < current->key) {
            _add(current->left, value);
        } else if (value > current->key) {
            _add(current->right, value);
        }
    }

    void add(int value) {
        _add(this->root, value); 
    }

    void _preorder(node* current) {
        if (current != nullptr) {
            std::cout << current->key << " ";
            _preorder(current->left);
            _preorder(current->right);
        }
    }

    void preorder() { 
        _preorder(this->root); 
    }

    void _inorder(node* current) {
        if (current != nullptr) {
            _inorder(current->left);
            std::cout << current->key << " ";
            _inorder(current->right);
        }
    }

    void inorder() {
        _inorder(this->root);
    }

    void _postorder(node* current) {
        if (current != nullptr) {
            _postorder(current->left);
            _postorder(current->right);
            std::cout << current->key << " ";
        }
    }

    void postorder() {
        _postorder(this->root); 
    }
};