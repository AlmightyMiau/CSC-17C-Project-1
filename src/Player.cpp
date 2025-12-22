#include "Player.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm> // sort

using namespace std;

// Constructor: link this dealer to the shared Deck
Player::Player(Deck* d, string id, int c) {
    deck = d;
    ID = id;
    chips = c;
    reset(); // Clear any existing hand
}

// Resets the dealer's hand for a new round
void Player::reset() {
    hand.clear();
    numCards = 0;
}

// Deal 2 cards to the players's hand
void Player::dealInitial() {
    hit();
    hit();
}

void Player::hit() {
    hand.push_back(deck->dealCard());
}

void Player::hit(int n) {
    for (int i = 0; i < n; i++) {
        hand.push_back(deck->dealCard());
    }
}

// returns the hand as a score based on hand type and card values
// perhaps as map<HANDS, int>
map<HANDS,int>* Player::calcHandScore(list<int> center) {
    // hands are from hands.h
    list<int> cards;
    map<HANDS,int>* total = new map<HANDS,int>;

    // get all cards to consider
    for (int c : hand)
        cards.push_back(c);
    for (int c : center)
        cards.push_back(c);
    
    // parse cards into face and suit
    map<int, int> face;
    map<char, int> suit;
    for (int c : cards)
        face[deck->value(c)]++;
    for (int c : cards)
        suit[deck->suit(c)]++;
    // straights, same cards, and flushes are the broad types of hands
    // same card type hands
    switch (face.size()) {
        case 2: // 11112 || 11122
            for (auto& pair : face)
                if (pair.second == 4)
                    total[0][FourOfAKind] = pair.first; // the face value of all cards in the hand
                else if (pair.second == 3)
                    total[0][FullHouse] += pair.first * 100; // shift the number over by 2 digits to pack both cards into one number -> hand of 88877 would be 0807 
                else if (pair.second == 2)
                    total[0][FullHouse] += pair.first;
            break;
        case 3: // 11123 || 11223
            for (auto& pair : face) { 
                if (pair.second == 3) { // Three of a kind
                    // There's only one number to worry about
                    total[0][ThreeOfAKind] = pair.first;
                    break;
                } else if (pair.second == 2) { // Two Pair
                    // There's two numbers to worry about
                    // make sure to put the bigger number in front
                    if (total[0][TwoPair] > 0 && pair.first < total[0][TwoPair]) {
                        total[0][TwoPair] *= 100;
                        total[0][TwoPair] += pair.first;
                    } else {
                        total[0][TwoPair] += pair.first * 100;
                    }
                }
            }
            break;
        case 4: // 11234
            for (auto& pair : face) {
                if (pair.second == 2) { // Pair
                    total[0][Pair] = pair.first;
                    break;
                }
            }
            break;
        default:
            break;
    }
    // check flush and straight seperately, then check if both occured
    // Flush
    if (suit.size() == 1) {
        int shift = 1;
        for (auto& pair : face) {
            total[0][Flush] += pair.first * shift;
            shift *= 100;
        }
    }
    // Straight
    // Traversing using iterators
    bool straight = true;
    auto next = face.begin();
    auto prev = next;
    for (advance(next, 1); next != face.end(); prev = next, ++next) {
        if (prev->first + 1 != next->first)
            straight = false;
    }
    if (straight)
        total[0][Straight] = face.end()->first; // A [rank]-high straight, only one number needs to be known
    // Straight Flush
    if (total[0][Flush] > 0 && total[0][Straight] > 0) {
        total[0][StraightFlush] = total[0][Straight]; // A [rank]-high straight flush, only one number needs to be known
    }
    if (total[0][Flush] == 0) { // clean up after trying to access total[0][Flush]
        // Deleting by key
        total->erase(Flush);
    }
        
    // High Card
    total[0][HighCard] += (face.end())->first;
    
    return total;
}

// int Player::getTotal() const {
//     int total = 0, aces = 0;
//     for (auto card : hand) {
//         int val = deck->value(card);
//         total += val;
//         if (val == 1) aces++;
//     }
//     while (aces > 0 && total + 10 <= 21) {
//         total += 10;
//         aces--;
//     }
//     return total;
// }

// bool Player::isBust() const {
//     return getTotal() > 21;
// }

void Player::showHand() const {
    cout << "Player " << ID << ": ";
    for (auto card : hand) {
        cout << deck->face(card) << deck->suit(card) << " ";
    }
    cout << endl;
}

int Player::getNumCards() const {
    return numCards;
}

void Player::displayChips() {
    cout << "Player " << ID << " has " << chips << " chips" << endl;
}