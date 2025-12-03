#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "deck.hpp"
#include "hands.hpp"

class Player {
protected:
    list<int> hand;   // Holds card IDs for player's hand (probably two cards)
    int numCards;     // Number of cards currently in hand

    Deck* deck;       // Pointer to the shared Deck

public:
    int ID;           // The player's ID to keep track of them
    HANDS handType;   // Type of hand present (considering dealer hand)
    int handScore;    // Score of hand (differs based on handType)

    Player(Deck* d, int id);          // Constructor links to deck
    void reset();             // Clears player's hand
    void dealInitial();       // Deals 2 initial cards
    void hit();               // Player takes a card
    void hit(int n);          // Player takes some cards
    map<HANDS, int>* calcHandScore(list<int> center); 
    // int getTotal() const;     // Calculates hand total
    // bool isBust() const;      // True if total > 21
    void showHand() const;    // Print player hand
    int getNumCards() const;  // Return number of cards
    list<int> getCards() {return hand;}
};

#endif