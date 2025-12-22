#include <cstdlib>
#include <ctime>
#include "deck.hpp"

using namespace std;

// Constructor: clears used card tracking
Deck::Deck() {
    shuffle();
}

// struct for use in recursive sorting
struct Data{
    int size;
    int *sortit;
    int *working;
};
Data *makeData(int n){
    //Allocate memory
    Data *data=new Data;
    data->size=n;
    data->sortit=new int[n];
    data->working=new int[n];
    return data;
}
void destroyData(Data* data){
    delete data->sortit;
    delete data->working;
    delete data;
}
void merge(Data *a, int beg, int nlow, int nhigh){ // Borrowed from Dr. Mark E. Lehr
    //Create a merged array
    int span=nhigh-beg;    //Span the range to merge
    int cntl=beg,cnth=nlow;//Independent counters to merge the split
    //Fill the array
    for(int i=0;i<span;i++){
        if(cntl==nlow){//Low done fill with the higher end of array
            a->working[i]=a->sortit[cnth++];
        }else if(cnth==nhigh){//High done fill with lower end of array
            a->working[i]=a->sortit[cntl++];
        }else if(a->sortit[cntl]<a->sortit[cnth]){
            a->working[i]=a->sortit[cntl++];//Fill with lower end
        }else{
            a->working[i]=a->sortit[cnth++];//Fill with higher end
        }
    }
    //Copy back from the working array to the sorted array
    for(int i=0;i<span;i++){
        a->sortit[beg+i]=a->working[i];
    }
}
// Merge sort
void mrgSort(Data *a, int beg, int end) { // Borrowed from Dr. Mark E. Lehr
    int center=(beg+end)/2;//Split the task down the middle
    if((center-beg)>1)mrgSort(a, beg, center);//Got to be an array to split
    if((end-center)>1)mrgSort(a, center, end);//Got to be an array to split
    merge(a, beg, center, end);//Merge the sorted arrays into 1 larger sorted array
}
// Recursively sort the cards currently in the deck
void Deck::sort() {
    Data* decks = makeData(deck.size());

    // Copy deck to array
    queue<int> temp = deck;
    int size = 0;
    while (!temp.empty()) {
        decks->sortit[size++] = temp.front();
        temp.pop();
    }

    // Sort the array
    mrgSort(decks, 0, deck.size());

    // Empty original queue
    while (!deck.empty()) {
        deck.pop();
    }
    // Copy array to queue
    for (int j = 0; j < size; j++) {
        deck.push(decks->sortit[j]);
    }

    // Clean up
    destroyData(decks);
}

// Remove all cards from queue, then put them all back in a random order
void Deck::shuffle() {
    while (!deck.empty()) { 
        deck.pop();
    }
    used.clear();

    newCard(); // It's recursive
}

// It's recursive
void Deck::newCard() { 
    if (used.size() < NUMCARDS) {
        int card = 0;
        do {
            card = rand() % NUMCARDS;
        } while(used.find(card) != used.end());
        
        deck.push(card);
        used.insert(card);
        
        newCard();
    } else {
        return;
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

string Deck::str(int n) {
    return string{face(n), suit(n)};
}

// Return a random, unused card and mark it as used
int Deck::dealCard() {
    int card = deck.front();
    deck.pop();
    return card;
}

// Display the full deck
void Deck::print() {
    // Psuedo Traversal
    queue<int> temp(deck); // Create a copy
    int i = 0;
    while (!temp.empty())
    {
        if (i == 13) { // cards displayed per line: 13
            cout << endl;
            i = 0;
        }
        cout << face(temp.front()) << suit(temp.front()) << " ";
        temp.pop();

        i++;
    }
    cout << endl;
}

int Deck::getSize() {
    return deck.size();
}