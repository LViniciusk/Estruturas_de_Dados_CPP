#pragma once

#include <iostream>
#include <string>

template <typename type>
class max_heap {
   private:
    type* _heap = nullptr;
    int _size = 0;
    int _capacity = 0;

    // Move um elemento para cima na heap, garantindo que o pai seja maior que
    // os filhos. O(log n)
    void max_fix_up(int index) {
        // Verifica se o elemento é a raiz, pois se for, não tem pai.
        if (index == 0) {
            return;
        }

        int parent = (index - 1) / 2;

        // Verifica se o pai é menor que seu filho.
        // Se for, troca o pai com o filho e chama a função up para o pai, pois
        // o pai pode ser menor que o seu pai.
        if (_heap[parent] < _heap[index]) {
            std::swap(_heap[parent], _heap[index]);
            max_fix_up(parent);
        }
    }

    // Move um elemento para baixo na heap, garantindo que o filho seja menor
    // que os pais. O(log n)
    void max_fix_down(int index) {
        // Calcula o índice do filho esquerdo, e assume que o filho esquerdo é o
        // maior filho.
        int index_bigger = (index * 2) + 1;

        // Verifica se existe filho esquerdo.
        // Se o filho esquerdo não existir, não tem como descer mais.
        if (index_bigger >= _size) {
            return;
        }

        // Verifica se existe o filho direito.
        // Se existir, verifica se o filho direito é maior que o filho esquerdo
        // e atualiza o índice do maior filho.
        if ((index_bigger + 1) < _size) {
            if (_heap[index_bigger] < _heap[index_bigger + 1]) {
                index_bigger += 1;
            }
        }

        // Verifica se o elemento i (pai) é menor que o maior filho.
        // Se for, troca o elemento i com o maior filho e chama a função down
        // para o maior filho, pois o maior filho pode ser menor que os seus
        // filhos.
        if (_heap[index] < _heap[index_bigger]) {
            std::swap(_heap[index], _heap[index_bigger]);
            max_fix_down(index_bigger);
        }
    }

    // Função que dobra a capacidade da heap. O(n)
    void reserve() {
        _capacity *= 2;
        type* new_heap = new type[_capacity];

        for (int i = 0; i < _size; i++) {
            new_heap[i] = _heap[i];
        }

        delete[] _heap;
        _heap = new_heap;
    }

    // Imprime a heap em forma de árvore.
    void vshow(type* vec, int size, int index, std::string heranca) {
        if (index < size &&
            ((2 * index + 1) < size || (2 * index + 2) < size)) {
            vshow(vec, size, 2 * index + 2, heranca + "r");
        }
        for (int i = 0; i < (int)heranca.size() - 1; i++) {
            std::cout << (heranca[i] != heranca[i + 1] ? "│   " : "    ");
        }
        if (heranca != "") {
            std::cout << (heranca.back() == 'r' ? "┌───" : "└───");
        }
        if (index >= size) {
            std::cout << "#" << std::endl;
            return;
        }
        std::cout << vec[index] << std::endl;
        if (index < size &&
            ((2 * index + 1) < size || (2 * index + 2) < size)) {
            vshow(vec, size, 2 * index + 1, heranca + "l");
        }
    }

   public:
    // Construtor padrão, inicializa a heap com capacidade 1, tamanho 0 e vetor
    // de tipos vazio.
    max_heap() {
        _capacity = 1;
        _size = 0;
        _heap = new type[_capacity];
    }

    // Construtor que inicializa a heap com um vetor de tipos e seu tamanho.
    // O(n)
    max_heap(type* v, int size) {
        // Inicializa a heap com o vetor e tamanho passado.
        _size = size;
        _capacity = size;
        _heap = new type[_capacity];

        // Copia os elementos do vetor para a heap.
        for (int i = 0; i < size; i++) {
            _heap[i] = v[i];
        }

        // Aplica a operação de heapify em todos os nós internos, começando do
        // meio do vetor até a raiz. (Heapify)
        for (int i = (_size / 2) - 1; i >= 0; i--) {
            max_fix_down(i);
        }
    }

    // Destrutor, deleta o vetor de tipos.
    ~max_heap() { 
        delete[] _heap; 
    }

    // Insere um elemento na heap. O(log n)
    void push(type value) {
        // Verifica se a heap está cheia, se estiver, dobra a capacidade.
        if (_size == _capacity) {
            reserve();
        }

        // Insere o elemento na última posição da heap e chama a função
        // max_fix_up para garantir que o pai seja maior que os filhos.
        _heap[_size] = value;
        max_fix_up(_size);
        _size++;
    }

    // Remove e retorna o maior elemento da heap. O(log n)
    type pop() {
        // Verifica se a heap está vazia.
        if (_size == 0) {
            throw "Heap is empty";
        }

        // Troca o elemento da raiz com o último elemento da heap e
        // decrementa o tamanho.
        std::swap(_heap[0], _heap[_size - 1]);
        _size--;

        // Chama a função max_fix_down para colocar o novo elemento da raiz na
        // posição correta. O(log n)
        max_fix_down(0);

        // Retorna o último elemento do vetor(anteriormente a raiz).
        return _heap[_size];
    }

    void print() {
        for (int i = 0; i < _size; i++) {
            std::cout << _heap[i] << " ";
        }
        std::cout << std::endl;
    }

    // Função que imprime a heap em forma de árvore. O(n)
    void print_tree() {
        vshow(_heap, _size, 0, "");
    }
};