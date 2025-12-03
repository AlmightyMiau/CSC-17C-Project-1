#include <cstdlib>
#include <ctime>
#include "deck.hpp"

using namespace std;

// Constructor: clears used card tracking
Deck::Deck() {
    shuffle();
}

// Remove all cards from queue, then put them all back in a random order
void Deck::shuffle() {
    while (!cards.empty()) {
        cards.pop();
    }
    used.clear();
    for (int i = 0; i < NUMCARDS; i++) {
        int card = 0;
        do {
            card = rand() % NUMCARDS;
        } while(used.find(card) != used.end());
        
        cards.push(card);
        used.insert(card);
    }
}

// Return Blackjack value: A=1, 2–10=face, J/Q/K=10 (1–10)
int Deck::value(int n) {
    n = (n >= 0) ? n % 13 : 0;
    if (n == 0) n = 13;
    n+=2;
    return n;
}

// Get card face character (A, 2, ..., K)
char Deck::face(int n) {
    char faces[] = { 'A','2','3','4','5','6','7','8','9','T','J','Q','K' };
    return faces[n % 13];
}

// Get card suit character (S, H, C, D)
char Deck::suit(int n) {
    char suits[] = { 'S','D','C','H' };
    return suits[n / 13];
}

// Return a random, unused card and mark it as used
int Deck::dealCard() {
    int card = cards.front();
    cards.pop();
    return card;
}

// Display the full deck
void Deck::print() {
    // Psuedo Traversal
    queue<int> temp(cards); // Create a copy
    while (!temp.empty())
    {
        cout << temp.front() << " ";
        temp.pop();
    }
    cout << endl;
}