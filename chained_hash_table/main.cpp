#include <iostream>

#include "chained_hash_table.hpp"

using namespace std;

int main() {
    hash_table<string, int> ht;
    ht.add("um", 1);
    ht.add("dois", 2);
    ht.add("três", 3);
    ht.add("quatro", 4);
    ht["cinco"] = 5;
    ht["seis"] = 6;
    ht["sete"] = 7;
    ht["oito"] = 8;
    ht["nove"] = 9;
    ht["dez"] = 10;

    cout << "A chave \"dois\" está na tabela hash? "
         << (ht.contains("dois") ? "Sim" : "Não") << endl;

    ht.remove("dois");

    cout << "A chave \"dois\" está na tabela hash? "
         << (ht.contains("dois") ? "Sim" : "Não") << endl;

    cout << "O valor associado à chave \"um\" é: " << ht.at("um") << endl;

    for (auto it = ht.begin(); it != ht.end(); ++it) {
        cout << it->first << " -> " << it->second << endl;
    }
}