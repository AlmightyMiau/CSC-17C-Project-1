#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <list>
#include "deck.hpp"
using namespace std;

// Hash function used for hashing data
unsigned int DJBHash(char* str, int length)
{
   unsigned int hash = 5381;

   for(std::size_t i = 0; i < length; i++)
   {
      hash = ((hash << 5) + hash) + str[i];
   }

   return hash;
}

class HashTable{
private:
    // Number of buckets to store elements
    int buckets;

    // Pointer to said buckets
    list<string> *table; // int because its the hash values

public:
    // Simple constructor
    HashTable(int b){
        // Allocate space for the number of buckets asked for 
        buckets = b;
        table = new list<string>[buckets];
    }

    // insert into hash table
    void insert(int card);

    // find hash in table
    bool contains(int card);

    // How many cards are looked at to find the queried one 
    int hits(int card);

    // Print out the hash table
    void print();
    void print(int bucket);
};


void HashTable::insert(int card){
    // Simple hash function is just modulo number of buckets
    string str = {Deck::face(card), Deck::suit(card)};
    int bucket = card % buckets;

    // Insert into hash table bucket
    table[bucket].push_back(str);
}

bool HashTable::contains(int card) {
    for (auto item : table[card % buckets])
        if (string({Deck::face(card), Deck::suit(card)}) == item)
            return true;
    return false;
}

int HashTable::hits(int card) {
    int i = 0;
    for (auto item : table[card % buckets]) {
        if (string({Deck::face(card), Deck::suit(card)}) == item)
            break;
        i++;
    }
    return i;
}

void HashTable::print(){
    for(int i = 0; i < buckets; i++){
        cout << "| Bucket " << i << " | ";
        for(auto j : table[i]){
            cout << "-> | " << j << " | ";
        }
        cout << endl;
    }
}

void HashTable::print(int bucket){
    cout << "| Bucket " << bucket << " | ";
    for(auto j : table[bucket]){
        cout << "-> | " << j << " | ";
    }
    cout << endl;
}


#endif