#ifndef Deck_HPP
#define Deck_HPP

#include <list>
#include <queue>
#include <set>
#include <bits/stdc++.h> // find()

// Deck class provides card drawing and information (faces, suits, values)
class Deck {
private:
    queue<int> deck;    // The deck to be pulled from during play
    set<int>   used;    // Tracks whether each card has been dealt already

public:
    static const int NUMCARDS = 52;

    Deck();              // Constructor: initializes the deck
    void sort();         // Recursive sort
    void shuffle();      // Resets deck so all cards are usable again
    void newCard();      // Adds a new random card to the deck (recursively)
    int dealCard();      // Deals a unique random card
    static int value(int n);    // Returns card value for Poker (2–14)
    static char face(int n);    // Returns character for face (A, 2, ..., K)
    static char suit(int n);    // Returns suit character (S, H, etc.)
    void print();        // Display the full deck
    int getSize();       // Get number of cards in deck
};

#endif
