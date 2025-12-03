#include <iostream>
#include <iomanip>
#include "Dealer.hpp"

using namespace std;

// Constructor: link this dealer to the shared Deck
Dealer::Dealer(Deck* d,int id) : Player(d,id) {}

// Dealer draws cards while hand value is below 17
// void Dealer::takeTurn() {
//     while (getTotal() < 17) {
//         hit();
//     }
// }

// Display dealer's hand
// If revealAll is false, hides first card (mimics real Blackjack)
void Dealer::showHand(bool revealAll) const {
    cout << "Dealer: ";
    if (!revealAll) {
        cout << deck->face(hand.front()) << deck->suit(hand.front()) << "?? " << endl;
        return;
    }

    // int total = getTotal();
    // cout << setw(2) << total << " <-- ";
    for (auto card : hand) {
        cout << deck->face(card) << deck->suit(card) << " ";
    }
    cout << endl;
}