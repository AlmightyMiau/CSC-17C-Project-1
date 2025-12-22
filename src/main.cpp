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
#include "tree.h"        // AVL Tree
#include "EdgeWeightedAdjacencyListGraph.h"
#include "MinimumSpanningTree.h"

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
// Demonstrate AVL Tree
void treeDemo(Deck& deck);
// Demonstrate Graph
void graphDemo();

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
        cout << "4. Display Tree of cards\n";
        cout << "5. Display Graph of hands\n";
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
            case 4:
                treeDemo(deck);
                break;
            case 5:
                graphDemo();
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
    cout << "\n===== Hash Table Demonstration =====\n";
    // First show what we're putting in
    deck.shuffle();
    cout << "Deck being inserted: " << endl;
    deck.print();

    // Create hash table
    HashTable table(13);

    Deck temp(deck);
    // Move of the cards in the deck to the hash table
    for (int i = 0; i < deck.getSize(); i++)
        table.insert(temp.dealCard());

    // Display the hash table
    cout << "\n----- Hash Table -----\n";
    table.print();

    // Query it for cards and report how many cards it had to look through first
    deck.shuffle();
    cout << "\nHow many other cards are seen before finding a card?" << endl;
    for (int i = 0; i < 8; i++) {
        int card = deck.dealCard();
        cout << Deck::face(card) << Deck::suit(card) << " : " << table.hits(card) << endl;
    }
    
    cout << "\n| Ending Hash Table demonstration |\n";
}

void treeDemo(Deck& deck) {
    cout << "\n===== AVL Tree Demonstration =====\n";
    // First show what we're putting in
    deck.shuffle();
    cout << "Deck being inserted: " << endl;
    deck.print();

    // Create tree
    Tree<string> tree;

    Deck temp(deck);
    // Move deck into tree
    for (int i = 0; i < deck.getSize(); i++)
        tree.insert(Deck::str(temp.dealCard()));

    // display tree
    int choice;
    do {
        cout << "\nHow to display the tree?\n";
        cout << "1. Full Display\n";
        cout << "2. In-Order\n";
        cout << "3. Pre-Order\n";
        cout << "4. Post-Order\n";
        cout << "5. Stop\n";
        cout << "Enter your choice: ";
        cin >> choice;
        clearInput();

        cout << endl;
        switch (choice) {
            case 1: // Full Display
                cout << "----- Full Tree Display -----" << endl;
                tree.display(tree.head, 0);
                break;
            case 2: // In order
                cout << "----- In-Order Display -----" << endl;
                tree.printIn(tree.head);
                break;
            case 3: // Pre order
                cout << "----- Pre-Order Display -----" << endl;
                tree.printPre(tree.head);
                break;
            case 4: // Post order
                cout << "----- Post-Order Display -----" << endl;
                tree.printPost(tree.head);
                break;
            case 5: 
                cout << "| Ending AVL Tree demonstration |" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while(choice != 5);
}

void graphDemo() {
    cout << "\n===== Graph Demonstration =====\n";
    cout << "   Using types of poker hands\n" << endl;
    // Graph
    /*
        High Card -- One Pair
        High Card -- Straight
        High Card -- Flush
        High Card -- Straight Flush
        One Pair -- Two Pair
        One Pair -- Three of a Kind
        One Pair -- Straight
        One Pair -- Flush
        One Pair -- Straight Flush
        Two Pair -- Full House
        Three of a Kind -- Full House
        Three of a Kind -- Four of a Kind
        Full House -- Four of a Kind
        Straight -- Flush
        Straight -- Straight Flush
        Flush -- Straight Flush
    */
   cout << "Key for hands:" << endl;
   cout << "0 : Straight Flush" << endl;
   cout << "1 : Four of a Kind" << endl;
   cout << "2 : Full House" << endl;
   cout << "3 : Flush" << endl;
   cout << "4 : Straight" << endl;
   cout << "5 : Three of a Kind" << endl;
   cout << "6 : Two Pair" << endl;
   cout << "7 : One Pair" << endl;
   cout << "8 : High Card\n" << endl;

    // Construct the graph
    EdgeWeightedAdjacencyListGraph graph(9);
    // Adapting a weighted graph to an unweighted one by only using weights of one
    graph.addEdge(Edge(HighCard, OnePair, 1)); 
    graph.addEdge(Edge(HighCard, Straight, 1)); 
    graph.addEdge(Edge(HighCard, Flush, 1)); 
    graph.addEdge(Edge(HighCard, StraightFlush, 1));
    graph.addEdge(Edge(OnePair, TwoPair, 1)); 
    graph.addEdge(Edge(OnePair, ThreeOfAKind, 1));
    graph.addEdge(Edge(OnePair, Straight, 1)); 
    graph.addEdge(Edge(OnePair, Flush, 1)); 
    graph.addEdge(Edge(OnePair, StraightFlush, 1));
    graph.addEdge(Edge(TwoPair, FullHouse, 1));
    graph.addEdge(Edge(ThreeOfAKind, FullHouse, 1));
    graph.addEdge(Edge(ThreeOfAKind, FourOfAKind, 1));
    graph.addEdge(Edge(FullHouse, FourOfAKind, 1));
    graph.addEdge(Edge(Straight, Flush, 1));
    graph.addEdge(Edge(Straight, StraightFlush, 1));
    graph.addEdge(Edge(Flush, StraightFlush, 1));

    // display graph
    cout << graph;

    // Distances between hands
    cout << "\nMinimum Spanning Tree:\n";
    MinimumSpanningTree mst{graph};
    for (const auto& edge : mst.edges()) {
        cout << edge << "\n";
    }
    cout << "Weight: " << mst.weight() << "\n";

    cout <<   "|     Ending Graph Demonstration       |\n";
    cout << "\n----- See Image of Graph in Folder -----n";
}