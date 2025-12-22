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
#include "hands.hpp"     // enum for possible hand types 
#include "HashTable.hpp" // Hash Table

//Function Prototypes
void clearInput();

// Game Logic
// Menu before/between rounds to look at the deck
void playGame(Deck& deck, list<Player*>& players, Dealer& dealer);
// Choose how to make a new deck
bool newDeckMenu(Deck& deck);
// Play a round of poker
void playRound(list<Player*>& players, Dealer& dealer);
// Demonstrate hash table (of cards)
void hashDemo(Deck& deck);

int main() {
    // Seed rand
    srand(time(0));
    
    Deck deck;              // Deck of cards
    list<Player*> players;  // Player's hand
    int numPlayers = 3;     // Number of people playing poker (not including a dealer)
    for (int i = 0; i < numPlayers; i++) {
        string playerName;
        cout << "Enter player " << i << "'s name: " << endl;
        cin >> playerName;
        players.push_back(new Player(&deck, playerName, 100));
    }
    Dealer dealer(&deck, string("dealer"));   // Cards in center

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

    int choice;
    do {
        cout << "\n==== Welcome ====\n";
        cout << "1. Play as guest\n";
        cout << "2. Get player chips\n";
        cout << "3. Display Hash Table of cards\n";
        // cout << "4. Display Tree of cards\n";
        // cout << "5. Display Graph of hands\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        clearInput();

        switch (choice) {
            case 1:
                playGame(deck, players, dealer);
                break;
            case 2:
                for (auto player : players) player->displayChips();
                break;
            case 3:
                hashDemo(deck);
                break;
            case 6:
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);


    // clean up
    for (auto player : players) delete player;
    // for (int i = 0; i < numPlayers; i++) delete playerScores[i];
    return 0;
}

void playGame(Deck& deck, list<Player*>& players, Dealer& dealer) {
    // choices
    int choice = 0;
    do {
        cout << "\n==== Playing Poker ====\n";
        cout << "1. Play Round\n";
        cout << "2. Display Deck (" << deck.getSize() << " cards)\n";
        cout << "3. Shuffle Deck\n";
        cout << "4. Sort Deck\n";
        cout << "5. Stop Playing\n";
        cout << "Enter your choice: ";
        cin >> choice;
        clearInput();

        bool ready = true;
        switch (choice) {
            case 1: // Play round (with current deck)
                // Ensure deck has enough cards to play
                if (deck.getSize() < 2*players.size() + 3)
                    ready = newDeckMenu(deck);
                if (ready)
                    playRound(players, dealer);
                break;
            case 2: // Display the deck
                cout << "Displaying deck" << endl;
                deck.print();
                break;
            case 3: // Shuffle the deck
                cout << "Suffling deck" << endl;
                deck.shuffle();
                cout << "Deck shuffled" << endl;
                break;
            case 4: // Sort the deck
                cout << "Sorting deck" << endl;
                deck.sort();
                cout << "Deck sorted" << endl;
                break;
            case 5: // Exit 
                cout << "Goodbye!\n";
                break;
            default: 
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);
    // stop playing
    return;
}

// We need to refill the deck, choose how
bool newDeckMenu(Deck& deck) {
    int choice = 0;
    cout << "\nThere weren't enough cards to play this round,";
    do {
        cout << "\nHow would you like to reset deck?\n";
        cout << "1. Don't Play Round\n";
        cout << "2. Display Deck (" << deck.getSize() << " cards)\n";
        cout << "3. Shuffle New Deck\n";
        cout << "4. Sort Deck\n";
        cout << "5. Continue Forth\n";
        cout << "Enter your choice: ";
        cin >> choice;
        clearInput();

        switch (choice) {
            case 1: // Don't play
                cout << "Not playing round" << endl;
                return false;
            case 2: // Display the deck
                cout << "Displaying deck" << endl;
                deck.print();
                break;
            case 3: // Shuffle the deck
                cout << "Suffling deck" << endl;
                deck.shuffle();
                cout << "Deck shuffled" << endl;
                break;
            case 4: // Sort the deck
                cout << "Sorting deck" << endl;
                deck.sort();
                cout << "Deck sorted" << endl;
                break; 
            case 5: // Continue forth
                if (deck.getSize() == deck.NUMCARDS)
                    return true;
                else {
                    cout << "There are not enough cards yet" << endl;
                    break;
                }
            default: 
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 1 && choice != 5);
    return false;
}

void playRound(list<Player*>& players, Dealer& dealer) {
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

    // Clear hands
    for (Player* player : players) {
        player->reset();
    }
    dealer.reset();
}

// Calls cin.clear() and cin.ignore()
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void hashDemo(Deck& deck) {
    // First show what we're putting in
    deck.shuffle();
    deck.print();

    // Create hash table
    HashTable table(13);

    Deck temp(deck);
    // Move of the cards in the deck to the hash table
    for (int i = 0; i < deck.getSize(); i++)
        table.insert(temp.dealCard());

    // Display the hash table
    table.print();

    // Query it for cards and report how many cards it had to look through first
    deck.shuffle();
    cout << "How many other cards are seen before finding a card?" << endl;
    for (int i = 0; i < 8; i++) {
        int card = deck.dealCard();
        cout << Deck::face(card) << Deck::suit(card) << " : " << table.hits(card) << endl;
    }

    return;
}