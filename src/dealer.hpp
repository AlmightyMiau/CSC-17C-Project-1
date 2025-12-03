#ifndef DEALER_HPP
#define DEALER_HPP

#include "deck.hpp"
#include "Player.hpp"

// Dealer class handles all dealer-related Blackjack behavior
class Dealer : public Player {
public:
    Dealer(Deck* d, int id);          // Constructor: links dealer to deck
    // void takeTurn();          // Dealer hits until total >= 17
    void showHand(bool revealAll = true) const;  // Displays hand (optionally hides first card)
};

#endif