#ifndef BTREE_H
#define BTREE_H

#include <utility>
#include <stdexcept>
#include <iostream>
#include <queue>
#include "Bnode.h"

template <typename T>
class Btree
{
private:
    Bnode<T> *m_root;
    int m_degree;

    void clear(Bnode<T> *node)
    {
        if (node->leaf)
        {
            delete node;
        }
        else
        {
            for (int i = 0; i <= node->n; ++i)
            {
                clear(node->child[i]);
            }
            delete node;
        }
    }

    std::pair<Bnode<T> *, int> search(Bnode<T> *x, T k)
    {
        int i = 0;
        while (i < x->n && k > x->key[i].first)
        {
            i++;
        }
        if (i < x->n && k == x->key[i].first)
            return {x, i};
        else if (x->leaf)
            return {nullptr, -1};
        else
            return search(x->child[i], k);
    }

    void split_child(Bnode<T> *x, int i)
    {
        Bnode<T> *y = x->child[i];

        Bnode<T> *z = new Bnode<T>(m_degree, y->leaf);
        z->n = m_degree - 1;

        for (int j = 0; j < m_degree - 1; ++j)
        {
            z->key[j] = y->key[m_degree + j];
        }

        if (!y->leaf)
        {
            for (int j = 0; j < m_degree; ++j)
            {
                z->child[j] = y->child[m_degree + j];
            }
        }

        y->n = m_degree - 1;

        for (int j = x->n; j >= i + 1; --j)
            x->child[j + 1] = x->child[j];

        x->child[i + 1] = z;

        for (int j = x->n - 1; j >= i; --j)
            x->key[j + 1] = x->key[j];

        x->key[i] = y->key[m_degree - 1];

        x->n++;
    }

    void insert_nonfull(Bnode<T> *x, std::pair<T, int> k)
    {
        int i = x->n - 1;

        if (x->leaf)
        {
            while (i >= 0 && k.first < x->key[i].first)
            {
                x->key[i + 1] = x->key[i];
                i = i - 1;
            }
            x->key[i + 1] = k;
            x->n = x->n + 1;
        }
        else
        {
            while (i >= 0 && k.first < x->key[i].first)
            {
                i--;
            }
            i++;
            if (x->child[i]->n == 2 * m_degree - 1)
            {
                split_child(x, i);
                if (k.first > x->key[i].first)
                {
                    i++;
                }
            }
            insert_nonfull(x->child[i], k);
        }
    }

    void concatenate(Bnode<T> *x, int i)
    {
        Bnode<T> *y = x->child[i];
        Bnode<T> *z = x->child[i + 1];

        y->key[y->n] = x->key[i];

        for (int j = 0; j < z->n; ++j)
        {
            y->key[y->n + 1 + j] = z->key[j];
        }

        if (!y->leaf)
        {
            for (int j = 0; j <= z->n; ++j)
            {
                y->child[y->n + 1 + j] = z->child[j];
            }
        }

        y->n += z->n + 1;

        for (int j = i; j < x->n - 1; ++j)
        {
            x->key[j] = x->key[j + 1];
            x->child[j + 1] = x->child[j + 2];
        }

        x->n--;

        delete z;
    }

    void borrowFromLeft(Bnode<T> *x, int i)
    {
        Bnode<T> *y = x->child[i];
        Bnode<T> *z = x->child[i - 1];

        for (int j = y->n - 1; j >= 0; --j)
        {
            y->key[j + 1] = y->key[j];
        }

        if (!y->leaf)
        {
            for (int j = y->n; j >= 0; --j)
            {
                y->child[j + 1] = y->child[j];
            }
        }

        y->key[0] = x->key[i - 1];
        if (!y->leaf)
        {
            y->child[0] = z->child[z->n];
        }

        x->key[i - 1] = z->key[z->n - 1];
        y->n++;
        z->n--;
    }

    void borrowFromRight(Bnode<T> *x, int i)
    {
        Bnode<T> *y = x->child[i];
        Bnode<T> *z = x->child[i + 1];

        y->key[y->n] = x->key[i];
        if (!y->leaf)
        {
            y->child[y->n + 1] = z->child[0];
        }

        x->key[i] = z->key[0];

        for (int j = 1; j < z->n; ++j)
        {
            z->key[j - 1] = z->key[j];
        }

        if (!z->leaf)
        {
            for (int j = 1; j <= z->n; ++j)
            {
                z->child[j - 1] = z->child[j];
            }
        }

        y->n++;
        z->n--;
    }

    void remove_from_leaf(Bnode<T> *x, int i)
    {
        for (int j = i + 1; j < x->n; j++)
        {
            x->key[j - 1] = x->key[j];
        }
        x->n--;
    }

    void remove_sucessor(Bnode<T> *x, int i, Bnode<T> *y)
    {
        if (y->leaf)
        {
            x->key[i] = y->key[0];
            remove_from_leaf(y, 0);
            return;
        }

        remove_sucessor(x, i, y->child[0]);
        Bnode<T> *L = y->child[0];
        Bnode<T> *R = y->child[1];
        if (L->n < m_degree - 1)
        {
            if (R->n > m_degree - 1)
            {
                borrowFromRight(y, 0);
            }
            else
            {
                concatenate(y, 0);
            }
        }
        else if (L->n > m_degree - 1)
        {
            if (R->n < m_degree - 1)
            {
                borrowFromLeft(y, 0);
            }
            else
            {
                concatenate(y, 0);
            }
        }
    }

    void remove_key(Bnode<T> *x, T k)
    {
        int i = 0;

        while (i < x->n && k > x->key[i].first)
        {
            i++;
        }

        if (i < x->n && k == x->key[i].first)
        {
            if (x->leaf)
            {
                remove_from_leaf(x, i);
            }
            else
            {
                Bnode<T> *y = x->child[i];
                Bnode<T> *z = x->child[i + 1];

                remove_sucessor(x, i, z);
                if (z->n < m_degree - 1)
                {
                    if (i > 0 && y->n >= m_degree)
                    {
                        borrowFromLeft(x, i);
                    }
                    else
                    {
                        concatenate(x, i);
                    }
                }
            }
        }
        else if (!x->leaf)
        {
            Bnode<T> *child = x->child[i];

            remove_key(child, k);
            if (child->n < m_degree - 1)
            {
                if (i > 0 && x->child[i - 1]->n >= m_degree)
                {
                    borrowFromLeft(x, i);
                }
                else if (i < x->n && x->child[i + 1]->n >= m_degree)
                {
                    borrowFromRight(x, i);
                }
                else
                {
                    if (i < x->n)
                    {
                        concatenate(x, i);
                    }
                    else
                    {
                        concatenate(x, i - 1);
                    }
                }
            }
        }
    }

    void _select(Bnode<T> *x, T k, std::vector<std::pair<T, int>> &result)
    {
        int i = 0;
        while (i < x->n)
        {
            if (x->key[i].first == k)
            {
                result.push_back(x->key[i]);
            }
            if (!x->leaf)
            {
                _select(x->child[i], k, result);
            }
            i++;
        }
        if (!x->leaf)
        {
            _select(x->child[i], k, result);
        }
    }

    void print_keys(Bnode<T> *node)
    {
        for (int i = 0; i < node->n; ++i)
        {
            if (!node->leaf)
            {
                print_keys(node->child[i]);
            }
            std::cout << node->key[i].first << " ";
        }
        if (!node->leaf)
        {
            print_keys(node->child[node->n]);
        }
    }

public:
    Btree(int d)
    {
        if (d < 2)
        {
            throw std::runtime_error("wrong degree");
        }
        m_degree = d;
        m_root = new Bnode<T>(m_degree, true);
    }

    ~Btree()
    {
        clear(m_root);
    }

    bool contains(T k)
    {
        return search(m_root, k).first != nullptr;
    }

    std::vector<std::pair<T, int>> select(T k)
    {
        std::vector<std::pair<T, int>> result;
        _select(m_root, k, result);

        return result;
    }

    void insert(T k, int offset)
    {
        std::pair<T, int> key = {k, offset};
        if (m_root->n == 2 * m_degree - 1)
        {
            Bnode<T> *newnode = new Bnode<T>(m_degree, false);
            newnode->child[0] = m_root;
            m_root = newnode;
            split_child(m_root, 0);
            insert_nonfull(m_root, key);
        }
        else
        {
            insert_nonfull(m_root, key);
        }
    }

    void print_keys()
    {
        print_keys(m_root);
    }

    void remove(T k)
    {
        remove_key(m_root, k);

        if (m_root->n < 1 && !m_root->leaf)
        {
            Bnode<T> *aux = m_root;
            m_root = m_root->child[0];
            delete aux;
        }
    }

    void printNodesByLevels()
    {
        std::queue<Bnode<T> *> fila;
        fila.push(m_root);
        int levelEnded = 1;
        while (!fila.empty())
        {
            if (levelEnded == 0)
            {
                levelEnded = fila.size();
                std::cout << "\n"
                          << std::endl;
            }
            Bnode<T> *node = fila.front();
            fila.pop();
            std::cout << "[";
            int i = 0;
            for (; i < node->n; i++)
            {
                std::cout << node->key[i].first << " (" << node->key[i].second << ")";
                if (i < node->n - 1)
                    std::cout << ",";
                if (!node->leaf)
                    fila.push(node->child[i]);
            }
            if (!node->leaf)
                fila.push(node->child[i]);
            std::cout << "] ";
            levelEnded--;
        }
        std::cout << std::endl;
    }
};

#endif
