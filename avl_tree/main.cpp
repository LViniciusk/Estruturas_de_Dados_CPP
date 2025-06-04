#include <iostream> 

#include "avl_tree.h"

using namespace std;

int main() {
    // Criando uma árvore AVL de inteiros
    avl_tree<int> tree;

    // Inserindo elementos na árvore
    tree.insert(50);
    tree.insert(40);
    tree.insert(30);
    tree.insert(20);
    tree.insert(10);
    tree.insert(5);

    // Imprimindo a árvore
    tree.print();

    // Removendo um elemento da árvore
    tree.remove(20);
    tree.print();
    cout << endl;

    // Verificando o tamanho e a altura da árvore
    cout << "Tree size: " << tree.size() << endl;
    cout << "Tree height: " << tree.height() << endl;
    cout << endl;

    // Verificando se a árvore contém um elemento
    if (tree.contains(20)) {
        cout << "Tree contains 20" << endl;
    } else {
        cout << "Tree does not contain 20" << endl;
    }

    // Transformando a árvore em um array
    cout << "Tree to array: ";
    int* arr = tree.to_array();
    for (unsigned int i = 0; i < tree.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    delete[] arr;

    // Imprimindo a árvore em ordem, pré-ordem e pós-ordem
    cout << endl;
    cout << "Tree in order: ";
    tree.inorder();
    cout << endl;
    cout << "Tree pre order: ";
    tree.preorder();
    cout << endl;
    cout << "Tree post order: ";
    tree.postorder();
    cout << endl << endl;

    // Verificando sucessor e antecessor de um elemento
    cout << "Successor of 30: " << tree.successor(30) << endl;
    cout << "Predecessor of 30: " << tree.predecessor(30) << endl;
    cout << endl;

    // Faz uma cópia da árvore
    avl_tree<int> tree_copy = tree;
    tree_copy.print();
    cout << endl;

    // Limpa a árvore
    tree.clear();
    tree.print();



    return 0;
}