#ifndef Deck_HPP
#define Deck_HPP

#include <list>
#include <queue>
#include <set>
#include <bits/stdc++.h> // find()

// Deck class provides card drawing and information (faces, suits, values)
class Deck {
private:
    queue<int> cards;   // The deck to be pulled from during play
    set<int>   used;    // Tracks whether each card has been dealt already

public:
    const int NUMCARDS = 52;

    Deck();              // Constructor: initializes the deck
    void shuffle();        // Resets deck so all cards are usable again
    int dealCard();      // Deals a unique random card
    int value(int n);    // Returns card value for Poker (2–14)
    char face(int n);    // Returns character for face (A, 2, ..., K)
    char suit(int n);    // Returns suit character (S, H, etc.)
    void print();        // Display the full deck
};

#endif
