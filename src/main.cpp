//System Libraries
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>  // rand() for picking cards
#include <ctime>    // time() for seeding random
#include <limits>   // numeric_limits<streamsize>::max() for clearing input
using namespace std;

// User Defined Libraries
#include "deck.cpp"
#include "Dealer.cpp"
#include "Player.cpp"
#include "hands.hpp"    // enum for possible hand types 

//Function Prototypes
void clearInput();

// Game Logic

int firstEmpty(const int* hand, int num);
int playBlackJack();

int main() {
    // Seed rand
    srand(time(0));
    
    Deck deck;              // Deck of cards
    list<Player*> players;  // Player's hand
    int numPlayers = 3;     // Number of people playing poker (not including a dealer)
    for (int i = 0; i < numPlayers; i++) players.push_back(new Player(&deck, i));
    Dealer dealer(&deck, -1);   // Cards in center

    // cout << "Deck:   "; deck.print();
    for (Player* player : players) {
        player->hit(2);
    }
    dealer.hit(3);
    for (auto player : players) {
        player->showHand();
    }
    dealer.showHand();

    Player*               winner;
    map<HANDS,int>** playerScores;
    stack<Player*> playersInGame;
    int i = 0;
    for (Player* player : players) {
        map<HANDS,int>* playerScore = player->calcHandScore(dealer.getCards());
        // find each player's best hand type
        player->handType  = playerScore->begin()->first;
        player->handScore = playerScore->begin()->second;
        i++;
    }

    // find highest enum
    HANDS bestHand = HighCard; // starts with worst for comparison
    for (Player* player : players) {
        if (player->handType < bestHand) {
            bestHand = player->handType;
        }
    }
    for (Player* player : players) {
        if (player->handType == bestHand) {
            playersInGame.push(player);
        }
    }
    // compare scores of same enum
    if (playersInGame.size() == 1) { // Player has won
        winner = playersInGame.top();
    }
    // Psuedo Traversal
    stack<Player*> temp(playersInGame); // Create a copy
    int bestScore = 0;
    while (!temp.empty())
    {
        if (temp.top()->handScore > bestScore) {
            bestScore = temp.top()->handScore;
            winner = temp.top();
        }
        temp.pop();
    }

    // Anounce Winner
    for (Player* player : players) {
        cout << "Player " << player->ID << " had a ";
        switch (player->handType) {
            case 0:
                cout << "Straight Flush";
                break;
            case 1:
                cout << "Four of a Kind";
                break;
            case 2:
                cout << "Full House";
                break;
            case 3:
                cout << "Flush";
                break;
            case 4:
                cout << "Straight";
                break;
            case 5:
                cout << "Three of a Kind";
                break;
            case 6:
                cout << "Two Pair";
                break;
            case 7:
                cout << "Pair";
                break;
            case 8:
                cout << "High Card";
                break;
            default:
                break;
        }
        cout << endl;
    }
    cout << "Player " << winner->ID << " is the winner !" << endl;


    // now we can see what hand the player has and can compare to olther players
    // BUT need more cards dealt to dealer

    // deal player their cards
    // find any valid hands
    // deal first three dealer cards
    // find any valid hands
    // deal dealer card
    // find any valid hands
    // deal last dealer card
    // find any valid hands
    // this is what the player wins/loses with

    // int choice;
    // do {
    //     cout << "\n==== Welcome ====\n";
    //     cout << "1. Play as guest\n";
    //     cout << "2. Exit\n";
    //     cout << "Enter your choice: ";
    //     cin >> choice;
    //     clearInput();

    //     switch (choice) {
    //     case 1:
    //         playBlackJack();
    //         break;
    //     case 2:
    //         cout << "Goodbye!\n";
    //         break;
    //     default:
    //         cout << "Invalid choice. Please try again.\n";
    //     }
    // } while (choice != 2);


    // clean up
    for (auto player : players) delete player;
    // for (int i = 0; i < numPlayers; i++) delete playerScores[i];
    return 0;
}

// Calls cin.clear() and cin.ignore()
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Find the first empty slot of a given hand
int firstEmpty(const int* hand, int num) {
    // Check every slot
    for (int i = 0; i < num; i++) {
        // If slot is empty (-1 means empty) return the slot number
        if (hand[i] == -1) {
            return i;
        }
    }
    return 0;
}

// Play a round of Blackjack and return the results
// 0 is loss, 1 is tie, 2 is win
// int playBlackJack() {
//     //Declare Variables
//     Deck deck;              // Deck of cards
//     Dealer dealer(&deck);   // Dealer object connected to deck
//     Player player(&deck);   // Player object connected to deck
//     int dealerTotal = 0;    // Total value of dealer's cards
//     int playerTotal = 0;    // Total value of player's cards

//     // Deal initial hands
//     dealer.dealInitial();
//     player.dealInitial();

//     // Show dealer's hand (one card hidden)
//     dealer.showHand(false);

//     // Display player's cards
//     player.showHand();

//     char choice;
//     // Player makes their move: hit or stand
//     cout << "Hit? (y/n): ";
//     cin >> choice;

//     // Process player input
//     while (choice == 'y') {
//         // Assign random card id to the first empty slot in playerCards' hand
//         player.hit();

//         // Display player's cards
//         player.showHand();

//         // Check for bust
//         if (player.isBust()) {
//             cout << "You went bust!\nYou lost :[" << endl;
//             return 0;
//         }

//         // Player makes their move: hit or stand
//         cout << "Hit? (y/n): ";
//         cin >> choice;
//     }

//     // Dealer hits while hand value less than 17
//     dealer.takeTurn(); 

//     // Check for dealer bust
//     if (dealer.isBust()) {
//         dealer.showHand(true);
//         cout << "Dealer went bust!\nYou won!" << endl;
//         return 1;
//     }

//     // output hand total and card faces + suits
//     dealer.showHand(true); // Reveal dealer's full hand
//     player.showHand();

//     // Get hand totals
//     dealerTotal = dealer.getTotal();
//     playerTotal = player.getTotal();

//     // Select winner based on highest hand value
//     if (playerTotal > dealerTotal) {
//         cout << "You won!" << endl;
//         return 1;
//     } else if (dealerTotal > playerTotal) {
//         cout << "You lost :[" << endl;
//         return 0;
//     } else { // Equal hand values; Compare hand sizes
//         int playerCount = player.getNumCards();
//         int dealerCount = dealer.getNumCards();

//         // Figure out whose is smaller and award winner
//         if (dealerCount > playerCount) { // Dealer has more cards than player; Player wins
//             cout << "You won by least cards!" << endl;
//             return 1;
//         } else if (playerCount > dealerCount) { // Player has more cards than dealer; Player loses
//             cout << "You lost by least cards :[" << endl;
//             return 0;
//         } else { // Equal card counts
//             cout << "It's a draw :/" << endl;
//             return 2;
//         }
//     }
// }